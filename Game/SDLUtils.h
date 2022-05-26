//
// Created by rjd
//

#ifndef SDL2GAME_SDLUTILS_H
#define SDL2GAME_SDLUTILS_H

#include <SDL2/SDL.h>
#include "GameCommon.h"

MoveDirection GetInputDirection()
{
	const uint8_t* key_state = SDL_GetKeyboardState(nullptr);
	if (key_state[SDL_SCANCODE_RIGHT])
	{
		return RIGHT;
	}
	else if (key_state[SDL_SCANCODE_LEFT])
	{
		return LEFT;
	}
	else if (key_state[SDL_SCANCODE_UP])
	{
		return UP;
	}
	else if (key_state[SDL_SCANCODE_DOWN])
	{
		return DOWN;
	}
	return NONE;
}

#endif //SDL2GAME_SDLUTILS_H
