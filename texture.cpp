#include "game.h"
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Texture *loadimage(const char *path, SDL_Renderer *sdl_renderer)
{
	SDL_Surface *non_optimized_image = NULL;
	non_optimized_image = IMG_Load(path);
	if(non_optimized_image == NULL)
	{
		std::cout << "loadBMP(): Failed to load image " << path << ", error: " << SDL_GetError() << std::endl;
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, non_optimized_image);
	if(texture == NULL)
	{
		std::cout << "Failed to create texture from surface" << std::endl;
	}
	SDL_FreeSurface(non_optimized_image);
	return texture;
}
