//
// Created by rjd
//

#include "Object.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <assert.h>

Object::Object(SDL_Renderer* renderer, const std::string& filepath, const SDL_Rect& obj_rect,
		const SDL_Rect& obj_pos):
		renderer_(renderer),
		texture_(nullptr),
		obj_rect_(obj_rect),
		obj_pos_(obj_pos)
{
	auto surface = IMG_Load(filepath.c_str());
	if (!surface)
	{
		std::cout << "Load file error: " << filepath << ", " << SDL_GetError() << std::endl;
		abort();
	}
	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	assert(texture_);

	SDL_QueryTexture(texture_, nullptr, nullptr,
			&texture_width_, &texture_height_);
}

Object::Object(SDL_Renderer* renderer, const std::string& filepath,
		const SDL_Rect& obj_pos):
		Object(renderer, filepath, {}, obj_pos)
{

}

Object::~Object()
{
	SDL_DestroyTexture(texture_);
}

void Object::Update(uint32_t delta_time_ms)
{

}

void Object::Draw()
{
	SDL_RenderCopy(renderer_, texture_,
			&obj_rect_, &obj_pos_);
}