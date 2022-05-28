//
// Created by rjd
//

#ifndef SDL2GAME_PLAYER_H
#define SDL2GAME_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Object.h"
#include "GameClient/GameController.h"

class Player : public Object
{
public:
	Player(SDL_Renderer* renderer, const std::string& filepath,
		const SDL_Rect& player_pos, int move_length, int player_id);
	~Player() = default;

	void Move(MoveDirection direction);

	void Update(uint32_t delta_time_ms) override;

	void PrintPos(const SDL_Point& draw_pos);

	void SetMoveLength(int move_length);

	int GetPlayerId() const;

private:

	int player_id_;

	int move_length_;

	int frame_width_;
	int frame_height_;

	std::array<int, 4> rect_y_;

	const int UPDATE_FRAME = 3;
	int frame_count;

	uint32_t GetMoveLength() const;
};


#endif //SDL2GAME_PLAYER_H
