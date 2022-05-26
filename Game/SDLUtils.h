//
// Created by rjd
//

#ifndef SDL2GAME_SDLUTILS_H
#define SDL2GAME_SDLUTILS_H

#include <SDL2/SDL.h>
#include "GameCommon.h"

static MoveDirection GetInputDirection()
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

static TTF_Font* default_font = nullptr;
static void PrintText(SDL_Renderer* renderer, const std::string& text, const SDL_Point& draw_pos,
		const SDL_Color& color = {255, 255, 255, 0},
		TTF_Font* font = default_font)
{
	if (font == nullptr)
	{
		TTF_Init();
		default_font = TTF_OpenFont("../Resource/wryh.ttf", 20);
	}

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	// 注意初始化 不然 x y 可能为脏数据
	SDL_Rect font_Rect{draw_pos.x, draw_pos.y, 0, 0};
	SDL_QueryTexture(text_texture, nullptr, nullptr, &font_Rect.w, &font_Rect.h);
	SDL_RenderCopy(renderer, text_texture, nullptr, &font_Rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

#endif //SDL2GAME_SDLUTILS_H
