#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <unordered_map>

#include <GameClient/GameClient.h>
#include <GameClient/GameController.h>
#include <Utils/TimeUtils.h>

#include "Game/Player.h"
#include "Game/SDLUtils.h"

int windows_length = 600;
int box_length = 30;

const int FPS = 30;
const int LOGIC_FPS = 10;
const int FRAME_MS = 1000 / FPS;
const int LOGIC_FRAME_MS = 1000 / LOGIC_FPS;

using PlayerPtr = std::shared_ptr<Player>;
std::unordered_map<int, PlayerPtr> player_map;
std::vector<ROLE_ID> players_id{1, 2, 3, 4};

PlayerPtr CreateDefaultPlayer(SDL_Renderer* renderer, ROLE_ID player_id)
{
	return std::make_shared<Player>(renderer,
			"../Resource/sprite-sheet.png",
			SDL_Rect{0, 0, box_length, box_length},
			5, player_id);
}

struct FrameStatus
{
	uint32_t prev_time = 0;
	uint32_t current_time = 0;
	uint32_t delta_time_ms = 0;
	uint32_t sum_time_ms = 0;
	uint32_t sum_logic_time_ms = 0;
	int logic_frame_count = 0;
};

void LoadPlayer(SDL_Renderer* render_target,
		ROLE_ID main_player_id, PlayerPtr& main_player)
{
	for (ROLE_ID player_id : players_id)
	{
		auto other_player =
				CreateDefaultPlayer(render_target, player_id);
		player_map.insert({player_id, other_player});
		if (main_player_id == player_id)
		{
			main_player = other_player;
		}
	}
	if (!main_player)
	{
		std::cout << "create main player error" << std::endl;
	}
}

void Init()
{

}

// main 函数被声明为了宏，所以argc和argv必须要有
// 这个函数会被SDL中内置的main函数调用
int main(int argc, char** argv)
{
	ROLE_ID main_player_id = 1;
	std::cin >> main_player_id;

	GameClientPtr client = std::make_shared<GameClient>("foo game client");
	bool ret = client->Connect(main_player_id, "127.0.0.1", "4000");
	if (!ret)
	{
		ret = client->Connect(main_player_id, "101.35.118.229", "4000");
	}
	if (!ret)
	{
		std::cout << "connect error" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "connect success" << std::endl;
	}
	GameController game_controller(client);
	bool enter_room_ret = client->EnterRoom(main_player_id, 2);
	if (enter_room_ret)
	{
		std::cout << "Enter Room success" << std::endl;
	}
	else
	{
		std::cout << "Enter Room Failed" << std::endl;
	}
	client->TestDelay(main_player_id);
	auto server_rtt = client->GetDelayMs();
	uint64_t server_time_start = client->WaitForGameStart(main_player_id);
	printf("rtt: %lu, server_time: %zu, local_time: %zu\r\n",
			server_rtt, server_time_start, NOW_MS);

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window*  window = SDL_CreateWindow("Test Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			windows_length, windows_length, SDL_WINDOW_SHOWN);

	// SDL_Renderer感觉是一个图层，index代表图层的下标从-1开始
	// SDL_RENDERER_ACCELERATED 硬件加速
	SDL_Renderer* render_target = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(render_target, 31, 199, 240, 0);

	PlayerPtr main_player;
	LoadPlayer(render_target, main_player_id, main_player);
	if (!main_player)
	{
		abort();
	}

	bool is_running = true;
	SDL_Event ev;
	FrameStatus frame_status{};
	// add server delay
	frame_status.sum_logic_time_ms = server_rtt / 2;

	std::vector<Input> pop_input_vec;
	while (is_running)
	{
		frame_status.prev_time = frame_status.current_time;
		frame_status.current_time = SDL_GetTicks();
		frame_status.delta_time_ms = frame_status.current_time - frame_status.prev_time;
		frame_status.sum_time_ms += frame_status.delta_time_ms;
		frame_status.sum_logic_time_ms += frame_status.delta_time_ms;

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
			{
				is_running = false;
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
				case SDLK_RIGHT:
					game_controller.PushInput(main_player->GetPlayerId(),
							frame_status.logic_frame_count, MoveDirection::RIGHT);
					break;
				case SDLK_LEFT:
					game_controller.PushInput(main_player->GetPlayerId(),
							frame_status.logic_frame_count, MoveDirection::LEFT);
					break;
				case SDLK_DOWN:
					game_controller.PushInput(main_player->GetPlayerId(),
							frame_status.logic_frame_count, MoveDirection::DOWN);
					break;
				case SDLK_UP:
					game_controller.PushInput(main_player->GetPlayerId(),
							frame_status.logic_frame_count, MoveDirection::UP);
					break;
				default:
					break;
				}
			}
		}

		while (frame_status.sum_logic_time_ms >= LOGIC_FRAME_MS)
		{
			frame_status.sum_logic_time_ms -= LOGIC_FRAME_MS;
			frame_status.logic_frame_count++;

			game_controller.PopInput(frame_status.logic_frame_count, pop_input_vec);
			for (const Input& input : pop_input_vec)
			{
				player_map[input.player_id]->Move(input.direction);
			}
			pop_input_vec.clear();

			client->TestDelay(main_player_id);
		}

		while (frame_status.sum_time_ms >= FRAME_MS)
		{
			frame_status.sum_time_ms -= FRAME_MS;

			SDL_RenderClear(render_target);

			int print_y = 0;
			for (auto& player : player_map)
			{
				player.second->Update(FRAME_MS);
				player.second->Draw();
				player.second->PrintPos({ 0, print_y});
				print_y += 20;
			}

			PrintText(render_target,
					std::to_string(client->GetDelayMs()) + " ms",
					{500, 0});

			SDL_RenderPresent(render_target);
		}

		auto frame_time = SDL_GetTicks() - frame_status.current_time;
		if (frame_time < FRAME_MS)
		{
			SDL_Delay(FRAME_MS - frame_time);
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render_target);
	SDL_Quit();

	std::chrono::milliseconds(20);

	return 0;
}