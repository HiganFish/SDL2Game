//
// Created by rjd
//

#include <cstdint>
#include <array>
#include "Player.h"
#include "SDLUtils.h"

Player::Player(SDL_Renderer* renderer, const std::string& filepath, const SDL_Rect& player_pos,
		int move_speed, int player_id):
		Object(renderer, filepath, player_pos),
		move_length_(move_speed),
		frame_count(0),
		rect_y_({}),
		player_id_(player_id)
{
	frame_width_ = texture_width_ / 3;
	frame_height_ = texture_height_ / 4;

	rect_y_ = {0, frame_width_, frame_width_ * 2, frame_width_ * 3};

	obj_rect_.x = 0;
	obj_rect_.y = 0;
	obj_rect_.h = frame_height_;
	obj_rect_.w = frame_width_;
}

void Player::Update(uint32_t delta_time_ms)
{
	Object::Update(delta_time_ms);

	frame_count++;
	if (frame_count >= UPDATE_FRAME)
	{
		frame_count = 0;
		obj_rect_.x += frame_width_;
		if (obj_rect_.x >= texture_width_)
		{
			obj_rect_.x = 0;
		}
	}
}

void Player::PrintPos(const SDL_Point& draw_pos)
{
	std::string pos = std::to_string(player_id_) + ": "
			+ std::to_string(obj_pos_.x)  + ", " + std::to_string(obj_pos_.y);
	PrintText(renderer_, pos, draw_pos);
}

void Player::SetMoveLength(int move_length)
{
	move_length_ = move_length;
}

uint32_t Player::GetMoveLength() const
{
	return move_length_;
}

void Player::Move(MoveDirection direction)
{
	switch (direction)
	{
	case MoveDirection::UP:
		obj_pos_.y -= static_cast<int>(GetMoveLength());
		obj_rect_.y = rect_y_[3];
		break;
	case MoveDirection::DOWN:
		obj_pos_.y += static_cast<int>(GetMoveLength());
		obj_rect_.y = rect_y_[0];
		break;
	case MoveDirection::LEFT:
		obj_pos_.x -= static_cast<int>(GetMoveLength());
		obj_rect_.y = rect_y_[1];
		break;
	case MoveDirection::RIGHT:
		obj_pos_.x += static_cast<int>(GetMoveLength());
		obj_rect_.y = rect_y_[2];
		break;
	default:
		break;
	}
}

int Player::GetPlayerId() const
{
	return player_id_;
}
