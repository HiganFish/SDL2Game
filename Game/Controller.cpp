//
// Created by rjd
//
#include <iostream>
#include "Controller.h"

Controller::Controller()
{

}

void Controller::PushInput(int player_id, int logic_frame, MoveDirection direction)
{
	input_deque_.push_back({ player_id, logic_frame, direction});
}

void Controller::PopInput(int current_logic_frame, std::vector<Input>& result)
{
	while (!input_deque_.empty())
	{
		auto& input = input_deque_.front();
		if (input_deque_.front().logic_frame + 1 == current_logic_frame)
		{
			result.push_back(input);
		}
		else
		{
			std::cout << "input some error" << std::endl;
		}
		input_deque_.pop_front();
	}
}