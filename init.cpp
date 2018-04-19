#include "types.h"
#include "game.h"
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <iostream>

bool graphics_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Failed to initialize SDL" << std::endl;
		return false;
	}

	int flags = IMG_INIT_PNG;
	int initialized_support = IMG_Init(flags);
	if(initialized_support != flags)
	{
		std::cout << "SDL2_image initialization error" << std::endl;
		return false;
	}

	if(TTF_Init() < 0)
	{
		std::cout << "Error initializing TTF" << std::endl;
		return false;
	}

	/* Create window */
	window = SDL_CreateWindow("Kółko i krzyżyk: technik informatyk w Ekonomiku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowx, windowy, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		std::cout << "Window creation failed." << std::endl;
		return false;
	}

	/* Create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		std::cout << "Failed to initialize renderer!!!" << std::endl;
		return false;
	}
	return true;
}

bool graphics_data_init()
{
	background = loadimage("data/background.png", renderer);
	if(background == NULL)
	{
		std::cout << "Failed to load image data/background.png" << std::endl;
		return false;
	}

	board = loadimage("data/board.png", renderer);
	if(board == NULL)
	{
		std::cout << "Failed to load image data/board.png" << std::endl;
		return false;
	}

	font = TTF_OpenFont("data/font.ttf", 0.056 * windowx/*28*/);
	if(font == NULL)
	{
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}

