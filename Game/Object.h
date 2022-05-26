//
// Created by rjd
//

#ifndef SDL2GAME_OBJECT_H
#define SDL2GAME_OBJECT_H

#include <SDL2/SDL.h>
#include <string>

class Object
{
public:
	Object(SDL_Renderer* renderer, const std::string& filepath,
			const SDL_Rect& obj_rect,
			const SDL_Rect& obj_pos);

	Object(SDL_Renderer* renderer, const std::string& filepath,
			const SDL_Rect& obj_pos);

	~Object();

	virtual void Update(uint32_t delta_time_ms);

	void Draw();

protected:
	SDL_Rect obj_rect_;
	SDL_Rect obj_pos_;

	SDL_Renderer* renderer_;
	SDL_Texture* texture_;

	int texture_width_;
	int texture_height_;
};


#endif //SDL2GAME_OBJECT_H
