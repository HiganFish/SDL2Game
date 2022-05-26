#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <unordered_map>

#include <GameClient/GameClient.h>
#include <Utils/TimeUtils.h>

#include "Game/Player.h"
#include "Game/Controller.h"
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

// main 函数被声明为了宏，所以argc和argv必须要有
// 这个函数会被SDL中内置的main函数调用
int main(int argc, char** argv)
{
	PingMessagePtr ping_message_ptr = std::make_shared<PingMessage>();
	ping_message_ptr->timestamp = NOW_MS;
	ping_message_ptr->role_id = 2000;

	GameClient client("foo game client");
	client.SetMsgCallback(MessageType::PING,
			[](ROLE_ID role_id, const BaseMessagePtr& ping_msg_ptr)
			{
				std::cout << ping_msg_ptr->DebugMessage();
			});

	client.Connect("1", "127.0.0.1", "4000");
	client.SendMsg("1", ping_message_ptr);

	ROLE_ID main_player_id;
	std::cin >> main_player_id;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_Window*  window = SDL_CreateWindow("Test Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			windows_length, windows_length, SDL_WINDOW_SHOWN);

	// SDL_Renderer感觉是一个图层，index代表图层的下标从-1开始
	// SDL_RENDERER_ACCELERATED 硬件加速
	SDL_Renderer* render_target = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(render_target, 31, 199, 240, 0);

	TTF_Font* font = TTF_OpenFont("../Resource/wryh.ttf", 20);
	SDL_Color color{255, 255, 255, 0};

	PlayerPtr main_player;
	LoadPlayer(render_target, main_player_id, main_player);
	if (!main_player)
	{
		abort();
	}

	Controller game_controller;

	bool is_running = true;
	SDL_Event ev;

	FrameStatus frame_status;

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
		}

		while (frame_status.sum_time_ms >= FRAME_MS)
		{
			frame_status.sum_time_ms -= FRAME_MS;

			auto in = GetInputDirection();
			if (in != NONE)
			{
				game_controller.PushInput(main_player->GetPlayerId(),
						frame_status.logic_frame_count, in);
			}

			SDL_RenderClear(render_target);

			int print_y = 0;
			for (auto& player : player_map)
			{
				player.second->Update(FRAME_MS);
				player.second->Draw();
				player.second->PrintPos(font, color, { 0, print_y});

				print_y += 20;
			}
			SDL_RenderPresent(render_target);
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render_target);
	SDL_Quit();
	return 0;
}