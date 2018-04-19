#include <iostream>
#include "types.h"
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <fstream>
#include <string.h>

#ifdef WINDOWS_BUILD
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <windows.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
#endif

#include "game.h"

/* To be done:
	- CODE CLEANUP: OK
	- SDL GRAPHICS: MOVE TO SDL2
	- SCORE TABLES AND SAVING: SEMI-DONE (READING TO BE IMPLEMENTED)
	- HARD AI */

int circles[9];
int crosses[9];
int circles_num = 0;
int crosses_num = 0;

int windowx = 0;
int windowy = 0;


void print_array(int *array, int size, std::string array_name)
{
	std::cout << array_name << ": ";
	int i = 0;
	while(i < size - 1)
	{
		std::cout << array[i] << ", ";
		i++;
	}
	std::cout << array[i] << std::endl;
}

coords2d fieldtocoords2d(int fld)
{
	coords2d buf;
//	errno = 0; /* Clear errno we want only know the errno set by strtol() */
//	int field = strtol(str.c_str(), NULL, 10);
//	if(errno != 0)
//	{
//		buf.x = -1;
//		buf.y = -1;
//	}
//	else
//	{
//		switch(field)
		switch(fld)
		{
			case 1: buf.x = 0; buf.y = 2; break;
			case 2: buf.x = 1; buf.y = 2; break;
			case 3: buf.x = 2; buf.y = 2; break;
			case 4: buf.x = 0; buf.y = 1; break;
			case 5: buf.x = 1; buf.y = 1; break;
			case 6: buf.x = 2; buf.y = 1; break;
			case 7: buf.x = 0; buf.y = 0; break;
			case 8: buf.x = 1; buf.y = 0; break;
			case 9: buf.x = 2; buf.y = 0; break;
			default: buf.x = -2; buf.y = -2; break;
		}
//	}
	return buf;
}

bool field_used(int field, int *fields_array, int arraysz)
{
	int i = 0;
	while(i < arraysz)
	{
		if(fields_array[i] == field)
		{
			return true;
		}
		i++;
	}
	return false;
}



#if 0
void draw_board(int *circles, int circles_count, int *crosses, int crosses_count)
{
	std::string board[3];
	board[2] = " 1|2|3";
	board[1] = " 4|5|6";
	board[0] = " 7|8|9";

	/* Draw circles */
	int i = 0;
	while(i < circles_count)
	{
		char str[2];
		sprintf(str, "%d", circles[i]);
		coords2d tmp = fieldtocoords2d(str);
		board[tmp.y][(tmp.x * 2) + 1] = 'O';
		std::cout << board[tmp.y][(tmp.x * 2) + 1] << std::endl;
		i++;
	}
	i = 0;
	/* Draw crosses */
	while(i < crosses_count)
	{
		char str[2];
		sprintf(str, "%d", crosses[i]);
		coords2d tmp = fieldtocoords2d(str);
		board[tmp.y][(tmp.x * 2) + 1] = 'X';
		i++;
	}

	std::cout << board[0] << std::endl;
	std::cout << board[1] << std::endl;
	std::cout << board[2] << std::endl;
}
#endif

typedef struct scene {
	SDL_Texture *background;
	SDL_Texture *cross_texture;
	SDL_Texture *circle_texture;
	SDL_Texture *empty_field_texture;
	SDL_Rect	*rectangles;
	SDL_Renderer	*renderer;
} scene;

typedef struct texture_info {
	int width;
	int height;
} texture_info;

typedef struct scene_info {
//	int num_textures;
	int num_rectangles;
	int new_cross; /* Field number where new cross is */
	int new_circle; /* Field number where new circle is */
} scene_info;

void draw_board(int *circles, int circles_count, int *crosses, int crosses_count, SDL_Texture *bg, SDL_Texture *empty, SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Renderer *wrenderer)
{

	/* Simple data inspection */
	/* Check textures */
	SDL_RenderClear(wrenderer);
	SDL_RenderPresent(wrenderer);


	/* Set up textures */
	/* 1 is cross, 2 is circle, 0 is empty */
	int tex_indices[9];
	/* Initialize board with zeros */
	int i = 0;
	while(i < 9)
	{
		tex_indices[i] = 0;
		i++;
	}
	i = 0;
	/* Fill circles */
	while(i < circles_count)
	{
		tex_indices[circles[i] - 1] = 2;
		i++;
	}
	i = 0;
	/* Fill crosses */
	while(i < crosses_count)
	{
		tex_indices[crosses[i] - 1] = 1;
		i++;
	}

	/* Prepare SDL_Rects */
	SDL_Rect r[9];
	r[6].x = 0.125 * windowx;
	r[6].y = 0.064 * windowy;
	r[6].h = windowy / 4;
	r[6].w = windowx / 4;

	r[7].x = 0.125 * windowx + r[6].w;
	r[7].y = 0.064 * windowy;
	r[7].w = windowx / 4;
	r[7].h = windowy / 4;

	r[8].x = 0.125 * windowx + r[6].w * 2;
	r[8].y = 0.064 * windowy;
	r[8].w = windowx / 4;
	r[8].h = windowx / 4;

	r[3].x = 0.125 * windowx;
	r[3].y = 0.064 * windowy + r[6].h;
	r[3].h = windowy / 4;
	r[3].w = windowx / 4;

	r[4].x = 0.125 * windowx + r[6].w;
	r[4].y = 0.064 * windowy + r[6].h;
	r[4].h = windowy / 4;
	r[4].w = windowx / 4;

	r[5].x = 0.125 * windowx + r[6].w * 2;
	r[5].y = 0.064 * windowy + r[6].h;
	r[5].h = windowy / 4;
	r[5].w = windowx / 4;

	r[0].x = 0.125 * windowx;
	r[0].y = 0.064 * windowy + r[6].h * 2;
	r[0].h = windowy / 4;
	r[0].w = windowx / 4;

	r[1].x = 0.125 * windowx + r[6].w;
	r[1].y = 0.064 * windowy + r[6].h * 2;
	r[1].h = windowy / 4;
	r[1].w = windowx / 4;

	r[2].x = 0.125 * windowx + r[6].w * 2;
	r[2].y = 0.064 * windowy + r[6].h * 2;
	r[2].h = windowy / 4;
	r[2].w = windowx / 4;

	/* Render everything */
	/* Maybe faster: */
	SDL_Texture *texture_array[3];
	texture_array[0] = empty;
	texture_array[1] = cross_texture;
	texture_array[2] = circle_texture;
	SDL_RenderClear(wrenderer);


	SDL_RenderCopy(wrenderer, bg, NULL, NULL);
	i = 0;

	while(i < 9)
	{
		SDL_RenderCopy(wrenderer, texture_array[tex_indices[i]], NULL, &r[i]);
		i++;
	}

	SDL_RenderPresent(wrenderer);
}

//void draw_board(int*, int, int*, int)
//{
//}

bool if_array_has_value(int *array, int size, int value)
{
	int i = 0;
	while(i < size)
	{
		if(array[i] == value)
		{
			return true;
		}
		i++;
	}
	return false;
}

int who_won(int *crosses_array, int crosses_count, int *circles_array, int circles_count, std::string AI_sign, std::string player1_sgn) /* Returns 0 when player won, 1 when AI and -1 when no one */
{																	/* Or 0 when player 1 won, 1 when player 2 if arg AI_sign was "NaN" */
	int cross_value = -1;
	int circle_value = -1;

	if(AI_sign == "NaN")
	{
		if(player1_sgn == "X")
		{
			cross_value = 0;
			circle_value = 1;
		}
		else
		{
			cross_value = 1;
			circle_value = 0;
		}
	}

	if(AI_sign == "X")
	{
		cross_value = 1;
		circle_value = 0;
	}
	if(AI_sign == "O")
	{
		cross_value = 0;
		circle_value = 1;
	}

	/* Check if there are three crosses across or vertically or horizontally */
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 4) &&
	   if_array_has_value(crosses_array, crosses_count, 7) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 9) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 2) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 7) &&
	   if_array_has_value(crosses_array, crosses_count, 8) &&
	   if_array_has_value(crosses_array, crosses_count, 9) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 9) &&
	   if_array_has_value(crosses_array, crosses_count, 6) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 2) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 8) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 4) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 6) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 7) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	/* Check if there are three circles across or vertically or horizontally */
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 4) &&
	   if_array_has_value(circles_array, circles_count, 7) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 9) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 2) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 7) &&
	   if_array_has_value(circles_array, circles_count, 8) &&
	   if_array_has_value(circles_array, circles_count, 9) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 9) &&
	   if_array_has_value(circles_array, circles_count, 6) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 2) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 8) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 4) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 6) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 7) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}

	return -1;
}

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

int which_field_player_clicked(int posx, int posy)
{
	/* Fields' dimensions and positions */
	SDL_Rect fields[9];
	fields[6].x = 0.125 * windowx;
	fields[6].y = 0.064 * windowy;
	fields[6].h = windowy / 4;
	fields[6].w = windowx / 4;

	fields[7].x = 0.125 * windowx + fields[6].w;
	fields[7].y = 0.064 * windowy;
	fields[7].w = windowx / 4;
	fields[7].h = windowy / 4;

	fields[8].x = 0.125 * windowx + fields[6].w * 2;
	fields[8].y = 0.064 * windowy;
	fields[8].w = windowx / 4;
	fields[8].h = windowx / 4;

	fields[3].x = 0.125 * windowx;
	fields[3].y = 0.064 * windowy + fields[6].h;
	fields[3].h = windowy / 4;
	fields[3].w = windowx / 4;

	fields[4].x = 0.125 * windowx + fields[6].w;
	fields[4].y = 0.064 * windowy + fields[6].h;
	fields[4].h = windowy / 4;
	fields[4].w = windowx / 4;

	fields[5].x = 0.125 * windowx + fields[6].w * 2;
	fields[5].y = 0.064 * windowy + fields[6].h;
	fields[5].h = windowy / 4;
	fields[5].w = windowx / 4;

	fields[0].x = 0.125 * windowx;
	fields[0].y = 0.064 * windowy + fields[6].h * 2;
	fields[0].h = windowy / 4;
	fields[0].w = windowx / 4;

	fields[1].x = 0.125 * windowx + fields[6].w;
	fields[1].y = 0.064 * windowy + fields[6].h * 2;
	fields[1].h = windowy / 4;
	fields[1].w = windowx / 4;

	fields[2].x = 0.125 * windowx + fields[6].w * 2;
	fields[2].y = 0.064 * windowy + fields[6].h * 2;
	fields[2].h = windowy / 4;
	fields[2].w = windowx / 4;

	int field_no = -1; /* By default clicked outside any field */
	int i = 0; /* Check for all fields */
	while(i < 9 && field_no == -1)
	{
		if(posx > fields[i].x && posx < (fields[i].x + fields[i].w) && posy > fields[i].y && posy < (fields[i].y + fields[i].h))
		{
			field_no = i + 1;
		}
		i++;
	}
	return field_no;
}

#ifdef WINDOWS_BUILD
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	/* Get window X and Y setting */
	std::fstream config;
	config.open("config.txt", std::ios::in);
	if(config.good())
	{
		std::string linebuf;
		getline(config, linebuf);
		windowx = atoi(linebuf.c_str());
		getline(config, linebuf);
		windowy = atoi(linebuf.c_str());
	}
	else
	{
		windowx = 600;
		windowy = 600;
	}
	config.close();
	/* Initialize SDL */
	SDL_Surface *window_screen = NULL;
	SDL_Window *window = NULL;
	SDL_Texture *board = NULL;
	SDL_Texture *background = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font *font = NULL;
	SDL_Surface *message = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Failed to initialize SDL" << std::endl;
		return -1;
	}

	int flags = IMG_INIT_PNG;
	int initialized_support = IMG_Init(flags);
	if(initialized_support != flags)
	{
		std::cout << "SDL2_image initialization error" << std::endl;
	}

	if(TTF_Init() < 0)
	{
		std::cout << "Error initializing TTF" << std::endl;
		return -1;
	}

	/* Create window */
	window = SDL_CreateWindow("Kółko i krzyżyk: technik informatyk w Ekonomiku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowx, windowy, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		std::cout << "Window creation failed." << std::endl;
		return -1;
	}

	/* Create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		std::cout << "Failed to initialize renderer!!!" << std::endl;
		return -1;
	}

	background = loadimage("data/background.png", renderer);
	if(background == NULL)
	{
		std::cout << "Failed to load image data/background.bmp" << std::endl;
	}

	board = loadimage("data/board.png", renderer);
	if(board == NULL)
	{
		std::cout << "Failed to load image data/board.png" << std::endl;
	}

	font = TTF_OpenFont("data/font.ttf", 0.056 * windowx/*28*/);
	if(font == NULL)
	{
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
	}

	int playing_mode = 0; /* 1 is playing against AI, 2 is playing agains second human player, 0 is unselected */

	/* Load button textures */
	SDL_Texture *humanplaybuttontex = loadimage("data/humanplay.png", renderer);
	if(humanplaybuttontex == NULL)
	{
		std::cout << "Failed to load data/humanplay.png" << std::endl;
	}

	SDL_Texture *AIplaybuttontex = loadimage("data/AIplay.png", renderer);
	if(AIplaybuttontex == NULL)
	{
		std::cout << "Failed to load data/AIplay.png" << std::endl;
	}

	/* Position rectangles */
	SDL_Rect game_mode_rects[2]; /* index 0 is for AI, 1 is for human */
	game_mode_rects[0].w = windowx / 3;
	game_mode_rects[0].h = windowy / 3;
	game_mode_rects[0].x = (windowx / 2) + (0.04 * windowx);
	game_mode_rects[0].y = (windowy / 2);

	game_mode_rects[1].w = windowx / 3;
	game_mode_rects[1].h = windowy / 3;
	game_mode_rects[1].x = (windowx / 2) - game_mode_rects[1].w - (0.04 * windowx);
	game_mode_rects[1].y = (windowy / 2);


	/* Prepare text */
//	SDL_FreeSurface(message);
//	SDL_FreeSurface(shadowsurf);

	SDL_Color text_color = { 255, 255, 255 };
	SDL_Color shadow_color = { 0, 0, 0 };

	message = TTF_RenderUTF8_Solid(font, "Wybierz tryb gry:", text_color);
	SDL_Texture *msgtext = SDL_CreateTextureFromSurface(renderer, message);

	SDL_Surface *shadowsurf = TTF_RenderUTF8_Solid(font, "Wybierz tryb gry:", shadow_color);
	SDL_Texture *shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);

	SDL_Rect msgrect;
	msgrect.x = (windowx / 2) - (message->w / 2);
	msgrect.y = 0;
	msgrect.w = message->w;
	msgrect.h = message->h;

	SDL_Rect msgshadowrect;
	msgshadowrect.x = (windowx / 2) - (message->w / 2) + (0.002 * windowx);
	msgshadowrect.y = 1;
	msgshadowrect.w = message->w;
	msgshadowrect.h = message->h;


	/* Render it all */

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
	SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
	SDL_RenderCopy(renderer, humanplaybuttontex, NULL, &game_mode_rects[1]);
	SDL_RenderCopy(renderer, AIplaybuttontex, NULL, &game_mode_rects[0]);

	SDL_Event events;

	while(playing_mode == 0)
	{
		SDL_PollEvent(&events);
		SDL_RenderPresent(renderer);
		int x = 0;
		int y = 0;
		if(events.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&x, &y);
			/* Check which playing mode was selected if any */
			if(x > game_mode_rects[1].x && x < (game_mode_rects[1].x + game_mode_rects[1].w) && y > game_mode_rects[1].y && y < (game_mode_rects[1].y + game_mode_rects[1].h))
			{
				playing_mode = 2; /* Playing with human player */
			}
			if(x > game_mode_rects[0].x && x < (game_mode_rects[0].x + game_mode_rects[0].w) && y > game_mode_rects[0].y && y < (game_mode_rects[0].y + game_mode_rects[1].h))
			{
				playing_mode = 1; /* Playing with AI */
			}
		}
		if(events.type == SDL_QUIT)
		{
			return 0;
		}
	}

	SDL_Delay(1000);

	std::cout << "Playing mode: " << playing_mode << std::endl;

	SDL_FreeSurface(message);
	SDL_FreeSurface(shadowsurf);
	SDL_DestroyTexture(humanplaybuttontex);
	SDL_DestroyTexture(AIplaybuttontex);
	SDL_DestroyTexture(msgtext);
	SDL_DestroyTexture(shadowtex);

	std::string first_player;
	std::string second_player;

	SDL_Texture *boxshadow = NULL;

	if(playing_mode == 2)
	{

		message = TTF_RenderUTF8_Solid(font, "Imię pierwszego gracza: ", text_color);
		msgtext = SDL_CreateTextureFromSurface(renderer, message);
		shadowsurf = TTF_RenderUTF8_Solid(font, "Imię pierwszego gracza: ", text_color);
		shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);

		msgrect.x = (windowx / 2) - (message->w / 2);
		msgrect.y = 0;
		msgrect.w = message->w;
		msgrect.h = message->h;

		msgshadowrect.x = (windowx / 2) - (message->w / 2) + (1/windowx);
		msgshadowrect.y = 1;
		msgshadowrect.w = message->w;
		msgshadowrect.h = message->h;

		/* Render name input */
		boxshadow = loadimage("data/boxshadow.png", renderer);
		if(boxshadow == NULL)
		{
			std::cout << "Failed to load data/boxshadow.png" << std::endl;
		}

		SDL_Surface *inputbox = TTF_RenderUTF8_Solid(font, "_", text_color);
		SDL_Texture *inputboxtex = SDL_CreateTextureFromSurface(renderer, inputbox);
		SDL_Rect inputboxrect;
		inputboxrect.w = inputbox->w;
		inputboxrect.h = inputbox->h;
		inputboxrect.x = windowx / 2;
		inputboxrect.y = windowy / 2;
		SDL_Texture *texture_from_screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
		SDL_SetRenderTarget(renderer, texture_from_screen);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
		SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
		SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
		SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);
		SDL_RenderPresent(renderer);
		std::string text;
		bool enter_pressed = false;
		SDL_StartTextInput();
		std::string prev;
		struct timeval start;
		struct timeval stop;
		struct timeval zero; /* For quick zeroing */

		SDL_Event last_keydown;

		while(enter_pressed == false)
		{
			if(SDL_PollEvent(&events))
			{
				if(events.type == SDL_TEXTINPUT)
				{
					text+=events.text.text;
					/* Update inputbox */
					inputbox = TTF_RenderUTF8_Solid(font, text.c_str(), text_color);
					inputboxtex = SDL_CreateTextureFromSurface(renderer, inputbox);
					inputboxrect.w = inputbox->w;
					inputboxrect.h = inputbox->h;
					inputboxrect.x = (windowx / 2) - (inputboxrect.w / 2);
				}
			}
			if(events.key.keysym.sym == SDLK_RETURN)
			{
				std::cout << "Enter pressed" << std::endl;
				std::cout << "Text: " << text << std::endl;
				enter_pressed = true;
			}
			SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
			SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
			SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();
		while(events.key.keysym.sym == SDLK_RETURN)
		{
			SDL_PollEvent(&events);
		}
		first_player = text;
		text = "";

		/* Second player's name */
		message = TTF_RenderUTF8_Solid(font, "Imię drugiego gracza: ", text_color);
		msgtext = SDL_CreateTextureFromSurface(renderer, message);
		shadowsurf = TTF_RenderUTF8_Solid(font, "Imię drugiego gracza: ", text_color);
		shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);

		msgrect.x = (windowx / 2) - (message->w / 2);
		msgrect.y = 0;
		msgrect.w = message->w;
		msgrect.h = message->h;

		msgshadowrect.x = (windowx / 2) - (message->w / 2) + (1/windowx);
		msgshadowrect.y = 1;
		msgshadowrect.w = message->w;
		msgshadowrect.h = message->h;
		
		inputbox = TTF_RenderUTF8_Solid(font, "_", text_color);
		inputboxtex = SDL_CreateTextureFromSurface(renderer, inputbox);
		
		inputboxrect.w = inputbox->w;
		inputboxrect.h = inputbox->h;
		inputboxrect.x = windowx / 2;
		inputboxrect.y = windowy / 2;
		texture_from_screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
		SDL_SetRenderTarget(renderer, texture_from_screen);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
		SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
		SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
		SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);
		SDL_RenderPresent(renderer);
		enter_pressed = false;
		SDL_StartTextInput();

		while(enter_pressed == false)
		{
			if(SDL_PollEvent(&events))
			{
				if(events.type == SDL_TEXTINPUT)
				{
					text+=events.text.text;
					/* Update inputbox */
					inputbox = TTF_RenderUTF8_Solid(font, text.c_str(), text_color);
					inputboxtex = SDL_CreateTextureFromSurface(renderer, inputbox);
					inputboxrect.w = inputbox->w;
					inputboxrect.h = inputbox->h;
					inputboxrect.x = (windowx / 2) - (inputboxrect.w / 2);
				}
			}
			if(events.key.keysym.sym == SDLK_RETURN)
			{
				std::cout << "Enter pressed" << std::endl;
				std::cout << "Text: " << text << std::endl;
				enter_pressed = true;
			}
			SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
			SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
			SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();
		second_player = text;
		/* Clean up */
		SDL_DestroyTexture(texture_from_screen);
		SDL_DestroyTexture(inputboxtex);
		SDL_DestroyTexture(boxshadow);
		SDL_DestroyTexture(msgtext);
		SDL_DestroyTexture(shadowtex);
		SDL_FreeSurface(message);
		SDL_FreeSurface(shadowsurf);

	}
	int player_id = 0;

	if(playing_mode == 2)
	{
		/* Select random player to choose sign and don't allow to get random number 0 */
		while(player_id == 0)
		{
			struct timeval cur_time;
			gettimeofday(&cur_time, NULL);
			srand(cur_time.tv_usec);
			player_id = rand() % 3;
		}
		std::string str;
		if(player_id == 1)
		{
			str = first_player + ", wybierz swój znak: ";
		}
		else
		{
			str = second_player + ", wybierz swój znak: ";
		}

		message = TTF_RenderUTF8_Solid(font, str.c_str(), text_color);
		shadowsurf = TTF_RenderUTF8_Solid(font, str.c_str(), shadow_color);
		msgtext = SDL_CreateTextureFromSurface(renderer, message);
		shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);
	}
	else
	{

		message = TTF_RenderUTF8_Solid(font, "Wybierz swój znak:", text_color);

//	SDL_FreeSurface(message);

		shadowsurf = TTF_RenderUTF8_Solid(font, "Wybierz swój znak:", shadow_color);
		msgtext = SDL_CreateTextureFromSurface(renderer, message);

		std::cout << shadowsurf->w << ", " << shadowsurf->h << std::endl;

		shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);

//	SDL_FreeSurface(shadowsurf);
	}
	/* Clear screen */
	SDL_RenderClear(renderer);

	/* Render textures */

//	SDL_Rect msgrect;
	msgrect.x = (windowx / 2) - (message->w / 2);
	msgrect.y = 0;
	msgrect.w = message->w;
	msgrect.h = message->h;

//	SDL_Rect msgshadowrect;
	msgshadowrect.x = (windowx / 2) - (message->w / 2) + (1/windowx);
	msgshadowrect.y = 1;
	msgshadowrect.w = message->w;
	msgshadowrect.h = message->h;

	Uint8 text_alpha = 0;
	Uint8 signs_alpha = 0;

	/* Render sign choosing menu */

	/* Load cross and circle textures */
	/* First get info */
	SDL_Surface *crosssurf = NULL;
	SDL_Surface *circlesurf = NULL;
	SDL_Texture *crosstex = NULL;
	SDL_Texture *circletex = NULL;
	SDL_Rect crossrect;
	SDL_Rect circlerect;
	crosssurf = IMG_Load("data/cross.png");
	if(crosssurf == NULL)
	{
		std::cout << "Failed to load data/cross.png" << std::endl;
	}
	crossrect.w = windowx / 3; 
	crossrect.h = windowy / 3;
	crossrect.x = (windowx / 2) + (0.04 * windowx)/*20*/;
	crossrect.y = (windowy / 2);
	crosstex = SDL_CreateTextureFromSurface(renderer, crosssurf);
	SDL_FreeSurface(crosssurf);

	if(crosstex == NULL)
	{
		std::cout << "Failed to load data/cross.png" << std::endl;
	}
	circlesurf = IMG_Load("data/circle.png");

	if(circlesurf == NULL)
	{
		std::cout << "Failed to load data/cross.png" << std::endl;
	}
	circlerect.w = windowx / 3; 
	circlerect.h = windowy / 3;
	circlerect.x = (windowx / 2) - circlerect.w - (0.04 * windowx)/*20*/;
	circlerect.y = (windowy / 2);
	circletex = SDL_CreateTextureFromSurface(renderer, circlesurf);
	SDL_FreeSurface(circlesurf);

	if(circletex == NULL)
	{
		std::cout << "Failed to load data/circle.png" << std::endl;
	}

	SDL_Surface *player1_turn = NULL;
	SDL_Texture *player1_turntex = NULL;
	SDL_Surface *player2_turn = NULL;
	SDL_Texture *player2_turntex = NULL;
	SDL_Rect player_rect;

	if(playing_mode == 2)
	{
		/* Texts "players' turn" */
		std::string str = "Ruch gracza " + first_player;
		player1_turn = TTF_RenderUTF8_Solid(font, str.c_str(), text_color);
		player1_turntex = SDL_CreateTextureFromSurface(renderer, player1_turn);
		str = "Ruch gracza " + second_player;
		player2_turn = TTF_RenderUTF8_Solid(font, str.c_str(), text_color);
		player1_turntex = SDL_CreateTextureFromSurface(renderer, player2_turn);
	}
	else
	{
		player1_turn = TTF_RenderUTF8_Solid(font, "Twój ruch", text_color);
		player1_turntex = SDL_CreateTextureFromSurface(renderer, player1_turn);
	}

	std::string player1_sign;
	std::string player2_sign;

#ifdef ANIMATIONS /* If animations are enabled */
	/* Set alpha blending */
	SDL_SetTextureBlendMode(msgtext, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(shadowtex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(crosstex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(circletex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(msgtext, text_alpha);
	SDL_SetTextureAlphaMod(shadowtex, text_alpha);
	SDL_SetTextureAlphaMod(crosstex, signs_alpha);
	SDL_SetTextureAlphaMod(circletex, signs_alpha);
	while(text_alpha < 255)
	{
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_SetTextureAlphaMod(msgtext, text_alpha);
		SDL_SetTextureAlphaMod(shadowtex, text_alpha);
		if(text_alpha > 100)
		{
			SDL_SetTextureAlphaMod(crosstex, text_alpha);
			SDL_SetTextureAlphaMod(circletex, text_alpha);
			SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
			SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
			signs_alpha+=((100 / 255) / 155);
		}
		SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
		SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
		SDL_RenderPresent(renderer);
		text_alpha++;
	}
#else
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
	SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
	SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
	SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
	SDL_RenderPresent(renderer);
#endif
	/* Event handling */

	bool quit = false;
	std::string AI_sign;
	/* Wait for choice */
	if(playing_mode == 1)
	{
		while(!quit && player1_sign.length() == 0)
		{
			SDL_PollEvent(&events);
			if(events.type == SDL_QUIT)
			{
				quit = true;
			}
			if(events.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				/* Decide which field was clicked */
				if(x < (crossrect.x + crossrect.w) && x > crossrect.x && y < (crossrect.y + crossrect.h) && y > crossrect.y)
				{
					player1_sign = "X";
					std::cout << "CROSS SELECTED" << std::endl;
					if(playing_mode == 1) AI_sign = "O";
				
#ifdef ANIMATIONS
					Uint8 selected_alpha = 255;
					text_alpha = 255;
					SDL_SetTextureAlphaMod(msgtext, text_alpha);
					SDL_SetTextureAlphaMod(shadowtex, text_alpha);
	
					while(selected_alpha > 0)
					{
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_SetTextureAlphaMod(crosstex, selected_alpha);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_RenderPresent(renderer);
						selected_alpha--;
					}
					/* Fade out ramining things */
					while(text_alpha > 0)
					{
						SDL_SetTextureAlphaMod(shadowtex, text_alpha);
						SDL_SetTextureAlphaMod(msgtext, text_alpha);
						SDL_SetTextureAlphaMod(circletex, text_alpha);

						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_RenderPresent(renderer);
						text_alpha-=5;
					}
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif
					quit = true;
				}
				if(x < (circlerect.x + circlerect.w) && x > circlerect.x && y < (circlerect.y + circlerect.h) && y > circlerect.y)
				{
					player1_sign = "O";
					std::cout << "CIRCLE SELECTED" << std::endl;
					if(playing_mode == 1) AI_sign = "X";
#ifdef ANIMATIONS
					Uint8 selected_alpha = 255;
					text_alpha = 255;
					SDL_SetTextureAlphaMod(msgtext, text_alpha);
					SDL_SetTextureAlphaMod(shadowtex, text_alpha);

					while(selected_alpha > 0)
					{
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_SetTextureAlphaMod(circletex, selected_alpha);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_RenderPresent(renderer);
						selected_alpha--;
					}
					/* Fade out remaining things */
					while(text_alpha > 0)
					{
						SDL_SetTextureAlphaMod(shadowtex, text_alpha);
						SDL_SetTextureAlphaMod(msgtext, text_alpha);
						SDL_SetTextureAlphaMod(crosstex, text_alpha);
	
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_RenderPresent(renderer);
						text_alpha-=5;
					}
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif
					quit = true;
				}
			}
		}

#ifdef ANIMATIONS
		/* Revert alpha values of textures */
		SDL_SetTextureAlphaMod(crosstex, 255);
		SDL_SetTextureAlphaMod(circletex, 255);
#endif
/*
	if(circle_or_cross == "X")
	{
		AI_sign = "O";
	} else if(circle_or_cross == "O") {
		AI_sign = "X";
	} else {
		std::cout << "Bad answer" << std::endl;
		return -1;
	}
*/
		std::cout << "AI_sign: " << AI_sign << std::endl;
		std::cout << "Your sign: " << player1_sign << std::endl;
		bool finish = false;
		double player_time_secs = 0;
		int num_player_turns = 0; /* Number of player turns */
		int who = 0; /* Who won the game */

#ifndef ANIMATIONS
		SDL_Delay(1000); /* Avoid accidental click */
#endif

		while(finish == false && (circles_num + crosses_num) < 9)
		{
			struct timeval player_turn_start;
			/* Draw board */
			draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer/*, textures_infos*/);
			gettimeofday(&player_turn_start, NULL);
			struct timeval player_turn_end;
			bool player_turn_done = false;
			while(player_turn_done == false)
			{
				int field_clicked = -1;
				while(field_clicked < 1)
				{
					SDL_PollEvent(&events);
					if(events.type == SDL_QUIT)
					{
						finish = true;
						player_turn_done = true;
					}
					if(events.type == SDL_MOUSEBUTTONDOWN)
					{
						int mousex = 0;
						int mousey = 0;
						SDL_GetMouseState(&mousex, &mousey);
						field_clicked = which_field_player_clicked(mousex, mousey);
	//					std::cout << "Player clicked field number " << field_clicked << std::endl;
					}
				}
				std::cout << "Field clicked going further " << field_clicked << std::endl;
				coords2d coords = fieldtocoords2d(field_clicked);
				if(coords.x < 0)
				{
					if(coords.x == -1)
					{
						std::cout << "Error: Not a number: " << strerror(errno) << std::endl;
					} else if(coords.x == -2) {
						std::cout << "Error: value out of range " << field_clicked << std::endl;
					}
				}
				else
				{
	//				int field_number = strtol(str.c_str(), NULL, 10);
					if(!field_used(field_clicked, crosses, crosses_num) && !field_used(field_clicked, circles, circles_num))
					{
						if(player1_sign == "X")
						{
							crosses[crosses_num] = field_clicked;
							crosses_num++;
						}
						if(player1_sign == "O")
						{
							circles[circles_num] = field_clicked;
							circles_num++;
						}
						player_turn_done = true;
						num_player_turns++;
						draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
					}
					else
					{
						std::cout << "This field is already used" << std::endl;
					}
				}
			}
			gettimeofday(&player_turn_end, NULL);
			player_time_secs = player_time_secs + (player_turn_end.tv_sec - player_turn_start.tv_sec) + ((player_turn_end.tv_usec - player_turn_start.tv_usec)/1000000.0);
			who = who_won(crosses, crosses_num, circles, circles_num, AI_sign, player1_sign);
			if(who == -1)
			{
				AI_turn(circles, circles_num, crosses, crosses_num, false, AI_sign);
				who = who_won(crosses, crosses_num, circles, circles_num, AI_sign, player1_sign);
			}
			draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
			if(who == 0)
			{
				std::cout << "CONGRATULATIONS!!! PLAYER WON!!!" << std::endl;
				finish = true;
			}
			if(who == 1)
			{
				std::cout << "YOU LOST!!!! YOU LOOSER!!!" << std::endl;
				std::string wait;
				std::cin >> wait;
				finish = true;
			}
		}
		std::cout << "Player total time: " << player_time_secs << " (secs)" << std::endl;
		if(who == 0)
		{
			std::string outstr;
			std::string player_name;
			std::cout << "Your name: ";
			std::cin >> player_name;
			outstr = player_name + " #";
			char player_time_secs_str[2048];
			sprintf(player_time_secs_str, "%'.3f", player_time_secs);
			outstr = outstr + player_time_secs_str;
			outstr = outstr + " #";
			char num_player_turns_str[10];
			sprintf(num_player_turns_str, "%d", num_player_turns);
			outstr = outstr + num_player_turns_str;
			std::fstream file;
			file.open("score.txt", std::ios::in | std::ios::out);
			if(!file.good()) /* Maybe file does not exist ? */
			{
				file.open("score.txt", std::ios::out);
				if(!file.good())
				{
					std::cout << "Could not write score.txt: " << strerror(errno) << std::endl;
				}
			}
			std::string linebuf; /* Append to file */
			std::string filebuf;
			/* Get file size */
			file.seekg(0, std::ios::end);
			ssize_t filesz = file.tellg();
			while(file.tellg() != filesz)
			{
				getline(file, linebuf);
				filebuf = filebuf + linebuf;
			}
			filebuf = filebuf + outstr;
			file << filebuf << std::endl;
			file.close();
		}
	}
	else
	{
		while(!quit && player1_sign.length() == 0)
		{
			SDL_PollEvent(&events);
			if(events.type == SDL_QUIT)
			{
				quit = true;
			}
			if(events.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				/* Decide which field was clicked */
				if(x < (crossrect.x + crossrect.w) && x > crossrect.x && y < (crossrect.y + crossrect.h) && y > crossrect.y)
				{
					player1_sign = "X";
					std::cout << "CROSS SELECTED" << std::endl;
					if(playing_mode == 2)
					{
						if(player_id == 1)
						{
							player1_sign = "X";
							player2_sign = "O";
						}
						else
						{
							player1_sign = "O";
							player2_sign = "X";
						}
					}
				
#ifdef ANIMATIONS
					Uint8 selected_alpha = 255;
					text_alpha = 255;
					SDL_SetTextureAlphaMod(msgtext, text_alpha);
					SDL_SetTextureAlphaMod(shadowtex, text_alpha);
	
					while(selected_alpha > 0)
					{
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_SetTextureAlphaMod(crosstex, selected_alpha);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_RenderPresent(renderer);
						selected_alpha--;
					}
					/* Fade out ramining things */
					while(text_alpha > 0)
					{
						SDL_SetTextureAlphaMod(shadowtex, text_alpha);
						SDL_SetTextureAlphaMod(msgtext, text_alpha);
						SDL_SetTextureAlphaMod(circletex, text_alpha);

						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_RenderPresent(renderer);
						text_alpha-=5;
					}
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif
					quit = true;
				}
				if(x < (circlerect.x + circlerect.w) && x > circlerect.x && y < (circlerect.y + circlerect.h) && y > circlerect.y)
				{
					player1_sign = "O";
					std::cout << "CIRCLE SELECTED" << std::endl;
					if(playing_mode == 2)
					{
						if(player_id == 1)
						{
							player1_sign = "X";
							player2_sign = "O";
						}
						else
						{
							player1_sign = "O";
							player2_sign = "X";
						}
					}
#ifdef ANIMATIONS
					Uint8 selected_alpha = 255;
					text_alpha = 255;
					SDL_SetTextureAlphaMod(msgtext, text_alpha);
					SDL_SetTextureAlphaMod(shadowtex, text_alpha);

					while(selected_alpha > 0)
					{
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_SetTextureAlphaMod(circletex, selected_alpha);
						SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
						SDL_RenderPresent(renderer);
						selected_alpha--;
					}
					/* Fade out remaining things */
					while(text_alpha > 0)
					{
						SDL_SetTextureAlphaMod(shadowtex, text_alpha);
						SDL_SetTextureAlphaMod(msgtext, text_alpha);
						SDL_SetTextureAlphaMod(crosstex, text_alpha);
	
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
						SDL_RenderCopy(renderer, msgtext, NULL, &msgrect);
						SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
						SDL_RenderPresent(renderer);
						text_alpha-=5;
					}
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif
					quit = true;
				}
			}
		}
	}

#ifdef ANIMATIONS
	/* Revert alpha values of textures */
	SDL_SetTextureAlphaMod(crosstex, 255);
	SDL_SetTextureAlphaMod(circletex, 255);
#endif
/*
	if(circle_or_cross == "X")
	{
		AI_sign = "O";
	} else if(circle_or_cross == "O") {
		AI_sign = "X";
	} else {
		std::cout << "Bad answer" << std::endl;
		return -1;
	}
*/
		std::cout << "player1_sign: " << player1_sign << std::endl;
		std::cout << "player2 sign: " << player2_sign << std::endl;
		bool finish = false;
		double player1_time_secs = 0;
		double player2_time_secs = 0;
		int num_player1_turns = 0; /* Number of player turns */
		int num_player2_turns = 0; /* Number of player turns */
		int who = 0; /* Who won the game */

#ifndef ANIMATIONS
		SDL_Delay(1000); /* Avoid accidental click */
#endif
		/* Prepare data */
		std::string str;
		if(player_id == 1)
		{
			str = first_player + " pierwszy wybierał znak więc " + second_player + " zaczyna";
		}
		else
		{
			str = second_player + " pierwszy wybierał znak więc " + first_player + " zaczyna";
		}

		SDL_Surface *info = TTF_RenderUTF8_Solid(font, str.c_str(), text_color);
		SDL_Texture *infotex = SDL_CreateTextureFromSurface(renderer, info);
		SDL_Surface *infoshadow = TTF_RenderUTF8_Solid(font, str.c_str(), shadow_color);
		SDL_Texture *shadowinfotex = SDL_CreateTextureFromSurface(renderer, infoshadow);

		SDL_Rect inforect;
		inforect.x = (windowx / 2) - (info->w / 2);
		inforect.y = 0;
		inforect.w = info->w;
		inforect.h = info->h;
		SDL_Rect inforect_shadow = inforect;
		inforect_shadow.x = inforect_shadow.x + (0.002 * windowx);
		
		/* Display info */
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_RenderCopy(renderer, shadowinfotex, NULL, &inforect_shadow);
		SDL_RenderCopy(renderer, infotex, NULL, &inforect);
		SDL_RenderPresent(renderer);

		SDL_Delay(1000);

		SDL_Texture *player1screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
		SDL_Texture *player2screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
		SDL_SetRenderTarget(renderer, player1screentex);
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_Rect turnrect;
		turnrect.x = (windowx / 2) - (player1_turn->w / 2);
		turnrect.y = 0;
		turnrect.w = player1_turn->w;
		turnrect.h = player2_turn->h;

		SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect);
		SDL_RenderCopy(renderer, player1_turntex, NULL, &turnrect);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, player2screentex);

		turnrect.x = (windowx / 2) - (player2_turn->w / 2);
		turnrect.w = player2_turn->w;
		turnrect.h = player2_turn->h;

		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect);
		SDL_RenderCopy(renderer, player1_turntex, NULL, &turnrect);
		SDL_RenderPresent(renderer);
	
		SDL_SetRenderTarget(renderer, NULL);


	while(finish == false && (circles_num + crosses_num) < 9 && playing_mode == 2)
	{

		struct timeval player1_turn_start;
		struct timeval player2_turn_start;
		struct timeval player2_turn_end;
		struct timeval player1_turn_end;

		/* Generate 2 backgrounds */

		if(playing_mode == 2) /* If play 2 human players */
		{
			/* Draw board */
			if(player_id == 1)
			{
				draw_board(circles, circles_num, crosses, crosses_num, player2screentex, board, crosstex, circletex, renderer/*, textures_infos*/);
				SDL_Delay(2000);
			}
			else
			{
				draw_board(circles, circles_num, crosses, crosses_num, player1screentex, board, crosstex, circletex, renderer/*, textures_infos*/);	
				SDL_Delay(2000);
			}
			
			bool player2_turn_done = false;
			bool player1_turn_done = false;
			if(player_id == 1)
			{
				gettimeofday(&player2_turn_start, NULL);
			}
			else
			{
				gettimeofday(&player1_turn_start, NULL);
			}
			if(player_id == 1)
			{
				while(player2_turn_done == false)
				{
					int field_clicked = -1;
					while(field_clicked < 1)
					{
						SDL_PollEvent(&events);
						if(events.type == SDL_QUIT)
						{
							finish = true;
							player2_turn_done = true;
						}
						if(events.type == SDL_MOUSEBUTTONDOWN)
						{
							int mousex = 0;
							int mousey = 0;
							SDL_GetMouseState(&mousex, &mousey);
							field_clicked = which_field_player_clicked(mousex, mousey);
		//					std::cout << "Player clicked field number " << field_clicked << std::endl;
						}
					}
					std::cout << "Field clicked going further " << field_clicked << std::endl;
					coords2d coords = fieldtocoords2d(field_clicked);
					if(coords.x < 0)
					{
						if(coords.x == -1)
						{
							std::cout << "Error: Not a number: " << strerror(errno) << std::endl;
						} else if(coords.x == -2) {
							std::cout << "Error: value out of range " << field_clicked << std::endl;
						}
					}
					else
					{
		//				int field_number = strtol(str.c_str(), NULL, 10);
						if(!field_used(field_clicked, crosses, crosses_num) && !field_used(field_clicked, circles, circles_num))
						{
							if(player2_sign == "X")
							{
								crosses[crosses_num] = field_clicked;
								crosses_num++;
							}
							if(player2_sign == "O")
							{
								circles[circles_num] = field_clicked;
								circles_num++;
							}
							player2_turn_done = true;
							gettimeofday(&player2_turn_end, NULL);
							num_player2_turns++;
							draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
						}
						else
						{
							std::cout << "This field is already used" << std::endl;
						}
					}
				}
				player2_time_secs = player2_time_secs + (player2_turn_end.tv_sec - player2_turn_start.tv_sec) + ((player2_turn_end.tv_usec - player2_turn_start.tv_usec)/1000000.0);
				
				who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				if(who == -1)
				{
//						AI_turn(circles, circles_num, crosses, crosses_num, false, AI_sign);
					who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				}
//				draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
				if(who == 0)
				{
					std::cout << "CONGRATULATIONS!!! PLAYER1 WON!!!" << std::endl;
					finish = true;
				}
				if(who == 1)
				{
					std::cout << "CONGRATULATIONS!!! PLAYER2 WON!!!" << std::endl;
					std::string wait;
					std::cin >> wait;
					finish = true;
				}
				std::cout << "Player2 total time: " << player2_time_secs << " (secs)" << std::endl;
				if(who == 0)
				{
					std::string outstr;
					std::string player_name;
					std::cout << "Your name: ";
					std::cin >> player_name;
					outstr = player_name + " #";
					char player_time_secs_str[2048];
					sprintf(player_time_secs_str, "%'.3f", player2_time_secs);
					outstr = outstr + player_time_secs_str;
					outstr = outstr + " #";
					char num_player_turns_str[10];
					sprintf(num_player_turns_str, "%d", num_player2_turns);
					outstr = outstr + num_player_turns_str;
					std::fstream file;
					file.open("score.txt", std::ios::in | std::ios::out);
					if(!file.good()) /* Maybe file does not exist ? */
					{
						file.open("score.txt", std::ios::out);
						if(!file.good())
						{
							std::cout << "Could not write score.txt: " << strerror(errno) << std::endl;
						}
					}
					std::string linebuf; /* Append to file */
					std::string filebuf;
					/* Get file size */
					file.seekg(0, std::ios::end);
					ssize_t filesz = file.tellg();
					while(file.tellg() != filesz)
					{
						getline(file, linebuf);
						filebuf = filebuf + linebuf;
					}
					filebuf = filebuf + outstr;
					file << filebuf << std::endl;
					file.close();
				}
				if(who == -1) /* No one won */
				{
					draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
					while(player1_turn_done == false)
					{
						int field_clicked = -1;
						while(field_clicked < 1)
						{
							SDL_PollEvent(&events);
							if(events.type == SDL_QUIT)
							{
								finish = true;
								player1_turn_done = true;
							}
							if(events.type == SDL_MOUSEBUTTONDOWN)
							{
								int mousex = 0;
								int mousey = 0;
								SDL_GetMouseState(&mousex, &mousey);
								field_clicked = which_field_player_clicked(mousex, mousey);
			//					std::cout << "Player clicked field number " << field_clicked << std::endl;
							}
						}
						std::cout << "Field clicked going further " << field_clicked << std::endl;
						coords2d coords = fieldtocoords2d(field_clicked);
						if(coords.x < 0)
						{
							if(coords.x == -1)
							{
								std::cout << "Error: Not a number: " << strerror(errno) << std::endl;
							} else if(coords.x == -2) {
								std::cout << "Error: value out of range " << field_clicked << std::endl;
							}
						}
						else
						{
			//				int field_number = strtol(str.c_str(), NULL, 10);
							if(!field_used(field_clicked, crosses, crosses_num) && !field_used(field_clicked, circles, circles_num))
							{
								if(player1_sign == "X")
								{
									crosses[crosses_num] = field_clicked;
									crosses_num++;
								}
								if(player1_sign == "O")
								{
									circles[circles_num] = field_clicked;
									circles_num++;
								}
								player1_turn_done = true;
								gettimeofday(&player1_turn_end, NULL);
								num_player1_turns++;
								draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
							}
							else
							{
								std::cout << "This field is already used" << std::endl;
							}
						}
					}
					player1_time_secs = player1_time_secs + (player1_turn_end.tv_sec - player1_turn_start.tv_sec) + ((player1_turn_end.tv_usec - player1_turn_start.tv_usec)/1000000.0);
					who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
					/* Check once more */
					if(who == 0)
					{
						std::cout << "CONGRATULATIONS!!! PLAYER1 WON!!!" << std::endl;
						finish = true;
					}
					if(who == 1)
					{
						std::cout << "CONGRATULATIONS!!! PLAYER2 WON!!!" << std::endl;
						std::string wait;
						std::cin >> wait;
						finish = true;
					}
					std::cout << "Player1 total time: " << player1_time_secs << " (secs)" << std::endl;
					if(who == 0)
					{
						std::string outstr;
						std::string player_name;
						std::cout << "Your name: ";
						std::cin >> player_name;
						outstr = player_name + " #";
						char player_time_secs_str[2048];
						sprintf(player_time_secs_str, "%'.3f", player1_time_secs);
						outstr = outstr + player_time_secs_str;
						outstr = outstr + " #";
						char num_player_turns_str[10];
						sprintf(num_player_turns_str, "%d", num_player1_turns);
						outstr = outstr + num_player_turns_str;
						std::fstream file;
						file.open("score.txt", std::ios::in | std::ios::out);
						if(!file.good()) /* Maybe file does not exist ? */
						{
							file.open("score.txt", std::ios::out);
							if(!file.good())
							{
								std::cout << "Could not write score.txt: " << strerror(errno) << std::endl;
							}
						}
						std::string linebuf; /* Append to file */
						std::string filebuf;
						/* Get file size */
						file.seekg(0, std::ios::end);
						ssize_t filesz = file.tellg();
						while(file.tellg() != filesz)
						{
							getline(file, linebuf);
							filebuf = filebuf + linebuf;
						}
						filebuf = filebuf + outstr;
						file << filebuf << std::endl;
						file.close();
					}
				}
			}
			else
			{
				while(player1_turn_done == false)
				{
					int field_clicked = -1;
					while(field_clicked < 1)
					{
						SDL_PollEvent(&events);
						if(events.type == SDL_QUIT)
						{
							finish = true;
							player2_turn_done = true;
						}
						if(events.type == SDL_MOUSEBUTTONDOWN)
						{
							int mousex = 0;
							int mousey = 0;
							SDL_GetMouseState(&mousex, &mousey);
							field_clicked = which_field_player_clicked(mousex, mousey);
		//					std::cout << "Player clicked field number " << field_clicked << std::endl;
						}
					}
					std::cout << "Field clicked going further " << field_clicked << std::endl;
					coords2d coords = fieldtocoords2d(field_clicked);
					if(coords.x < 0)
					{
						if(coords.x == -1)
						{
							std::cout << "Error: Not a number: " << strerror(errno) << std::endl;
						} else if(coords.x == -2) {
							std::cout << "Error: value out of range " << field_clicked << std::endl;
						}
					}
					else
					{
		//				int field_number = strtol(str.c_str(), NULL, 10);
						if(!field_used(field_clicked, crosses, crosses_num) && !field_used(field_clicked, circles, circles_num))
						{
							if(player1_sign == "X")
							{
								crosses[crosses_num] = field_clicked;
								crosses_num++;
							}
							if(player1_sign == "O")
							{
								circles[circles_num] = field_clicked;
								circles_num++;
							}
							player1_turn_done = true;
							gettimeofday(&player1_turn_end, NULL);
							num_player1_turns++;
							draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
						}
						else
						{
							std::cout << "This field is already used" << std::endl;
						}
					}
				}
				player1_time_secs = player1_time_secs + (player1_turn_end.tv_sec - player1_turn_start.tv_sec) + ((player1_turn_end.tv_usec - player1_turn_start.tv_usec)/1000000.0);
				
				who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				if(who == -1)
				{
//						AI_turn(circles, circles_num, crosses, crosses_num, false, AI_sign);
					who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				}
//				draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
				if(who == 0)
				{
					std::cout << "CONGRATULATIONS!!! PLAYER1 WON!!!" << std::endl;
					finish = true;
				}
				if(who == 1)
				{
					std::cout << "CONGRATULATIONS!!! PLAYER2 WON!!!" << std::endl;
					std::string wait;
					std::cin >> wait;
					finish = true;
				}
				std::cout << "Player1 total time: " << player1_time_secs << " (secs)" << std::endl;
				if(who == 0)
				{
					std::string outstr;
					std::string player_name;
					std::cout << "Your name: ";
					std::cin >> player_name;
					outstr = player_name + " #";
					char player_time_secs_str[2048];
					sprintf(player_time_secs_str, "%'.3f", player1_time_secs);
					outstr = outstr + player_time_secs_str;
					outstr = outstr + " #";
					char num_player_turns_str[10];
					sprintf(num_player_turns_str, "%d", num_player1_turns);
					outstr = outstr + num_player_turns_str;
					std::fstream file;
					file.open("score.txt", std::ios::in | std::ios::out);
					if(!file.good()) /* Maybe file does not exist ? */
					{
						file.open("score.txt", std::ios::out);
						if(!file.good())
						{
							std::cout << "Could not write score.txt: " << strerror(errno) << std::endl;
						}
					}
					std::string linebuf; /* Append to file */
					std::string filebuf;
					/* Get file size */
					file.seekg(0, std::ios::end);
					ssize_t filesz = file.tellg();
					while(file.tellg() != filesz)
					{
						getline(file, linebuf);
						filebuf = filebuf + linebuf;
					}
					filebuf = filebuf + outstr;
					file << filebuf << std::endl;
					file.close();
				}
				if(who == -1) /* No one won */
				{
					draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
					while(player2_turn_done == false)
					{
						int field_clicked = -1;
						while(field_clicked < 1)
						{
							SDL_PollEvent(&events);
							if(events.type == SDL_QUIT)
							{
								finish = true;
								player2_turn_done = true;
							}
							if(events.type == SDL_MOUSEBUTTONDOWN)
							{
								int mousex = 0;
								int mousey = 0;
								SDL_GetMouseState(&mousex, &mousey);
								field_clicked = which_field_player_clicked(mousex, mousey);
			//					std::cout << "Player clicked field number " << field_clicked << std::endl;
							}
						}
						std::cout << "Field clicked going further " << field_clicked << std::endl;
						coords2d coords = fieldtocoords2d(field_clicked);
						if(coords.x < 0)
						{
							if(coords.x == -1)
							{
								std::cout << "Error: Not a number: " << strerror(errno) << std::endl;
							} else if(coords.x == -2) {
								std::cout << "Error: value out of range " << field_clicked << std::endl;
							}
						}
						else
						{
			//				int field_number = strtol(str.c_str(), NULL, 10);
							if(!field_used(field_clicked, crosses, crosses_num) && !field_used(field_clicked, circles, circles_num))
							{
								if(player2_sign == "X")
								{
									crosses[crosses_num] = field_clicked;
									crosses_num++;
								}
								if(player2_sign == "O")
								{
									circles[circles_num] = field_clicked;
									circles_num++;
								}
								player2_turn_done = true;
								gettimeofday(&player2_turn_end, NULL);
								num_player2_turns++;
								draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
							}
							else
							{
								std::cout << "This field is already used" << std::endl;
							}
						}
					}
					player2_time_secs = player2_time_secs + (player2_turn_end.tv_sec - player2_turn_start.tv_sec) + ((player2_turn_end.tv_usec - player2_turn_start.tv_usec)/1000000.0);
					who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
					/* Check once more */
					if(who == 0)
					{
						std::cout << "CONGRATULATIONS!!! PLAYER1 WON!!!" << std::endl;
						finish = true;
					}
					if(who == 1)
					{
						std::cout << "CONGRATULATIONS!!! PLAYER2 WON!!!" << std::endl;
						std::string wait;
						std::cin >> wait;
						finish = true;
					}
					std::cout << "Player2 total time: " << player2_time_secs << " (secs)" << std::endl;
					if(who == 0)
					{
						std::string outstr;
						std::string player_name;
						std::cout << "Your name: ";
						std::cin >> player_name;
						outstr = player_name + " #";
						char player_time_secs_str[2048];
						sprintf(player_time_secs_str, "%'.3f", player2_time_secs);
						outstr = outstr + player_time_secs_str;
						outstr = outstr + " #";
						char num_player_turns_str[10];
						sprintf(num_player_turns_str, "%d", num_player2_turns);
						outstr = outstr + num_player_turns_str;
						std::fstream file;
						file.open("score.txt", std::ios::in | std::ios::out);
						if(!file.good()) /* Maybe file does not exist ? */
						{
							file.open("score.txt", std::ios::out);
							if(!file.good())
							{
								std::cout << "Could not write score.txt: " << strerror(errno) << std::endl;
							}
						}
						std::string linebuf; /* Append to file */
						std::string filebuf;
						/* Get file size */
						file.seekg(0, std::ios::end);
						ssize_t filesz = file.tellg();
						while(file.tellg() != filesz)
						{
							getline(file, linebuf);
							filebuf = filebuf + linebuf;
						}
						filebuf = filebuf + outstr;
						file << filebuf << std::endl;
						file.close();
					}
				}
			}
		}
	}
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(board);
	SDL_DestroyTexture(circletex);
	SDL_DestroyTexture(crosstex);
	SDL_DestroyTexture(shadowtex);
	SDL_DestroyTexture(msgtext);
	SDL_DestroyRenderer(renderer);
	
	SDL_DestroyWindow(window);
	SDL_Quit;

return 0;
}

