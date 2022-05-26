//
// Created by rjd
//

#ifndef SDL2GAME_CONTROLLER_H
#define SDL2GAME_CONTROLLER_H

#include <deque>
#include <vector>

#include "GameCommon.h"

struct Input
{
	ROLE_ID player_id;
	int logic_frame;
	MoveDirection direction;
};
class Controller
{
public:
	Controller();
	~Controller() = default;
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;

	void PushInput(int player_id, int logic_frame, MoveDirection direction);

	void PopInput(int current_logic_frame, std::vector<Input>& result);

private:
	std::deque<Input> input_deque_;

};

#endif //SDL2GAME_CONTROLLER_H
