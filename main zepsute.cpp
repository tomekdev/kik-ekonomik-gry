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
	- SCORE TABLES AND SAVING: SEMI-DONE (READING TO BE IMPLEMENTED)
	- HARD AI */

int circles[9];
int crosses[9];
int circles_num = 0;
int crosses_num = 0;

int windowx = 0;
int windowy = 0;

SDL_Surface *window_screen = NULL;
SDL_Window *window = NULL;
SDL_Texture *board = NULL;
SDL_Texture *background = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Surface *msgsurf = NULL;
SDL_Texture *msgtex = NULL;

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

	if(!graphics_init())
	{
		std::cout << "Initialization failed. Exiting..." << std::endl;
		return -1;
	}

	if(!graphics_data_init())
	{
		std::cout << "Failed to load graphics. Exiting..." << std::endl;
		return -1;
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
//	SDL_FreeSurface(msgsurf);
//	SDL_FreeSurface(shadowsurf);

	SDL_Color text_color = { 255, 255, 255 };
	SDL_Color shadow_color = { 0, 0, 0 };

	SDL_Surface *msgsurf = NULL;
	SDL_Surface *shadowsurf = NULL;
	SDL_Texture *msgtex = NULL;
	SDL_Texture *shadowtex = NULL;
	
	gen_text_dual_color(font, "Wybierz tryb gry: ", &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

	SDL_Rect msgrect;
	msgrect.x = (windowx / 2) - (msgsurf->w / 2);
	msgrect.y = 0;
	msgrect.w = msgsurf->w;
	msgrect.h = msgsurf->h;

	SDL_Rect msgshadowrect;
	msgshadowrect.x = (windowx / 2) - (msgsurf->w / 2) + (0.002 * windowx);
	msgshadowrect.y = 1;
	msgshadowrect.w = msgsurf->w;
	msgshadowrect.h = msgsurf->h;


	/* Render it all */

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
	SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
	SDL_RenderCopy(renderer, humanplaybuttontex, NULL, &game_mode_rects[1]);
	SDL_RenderCopy(renderer, AIplaybuttontex, NULL, &game_mode_rects[0]);

	SDL_Event events;
	bool quit = false;


	while(playing_mode == 0 && quit == false)
	{
		while(SDL_PollEvent(&events))
		{
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
				quit = true;
				break;
			}
		}
	}

	std::cout << "Playing mode: " << playing_mode << std::endl;

	SDL_DestroyTexture(msgtex);
	SDL_DestroyTexture(shadowtex);
	SDL_FreeSurface(shadowsurf);
	SDL_FreeSurface(msgsurf);
	SDL_DestroyTexture(humanplaybuttontex);
	SDL_DestroyTexture(AIplaybuttontex);

	/* NULL the pointers */
	msgtex = NULL;
	shadowtex = NULL;
	shadowsurf = NULL;
	msgsurf = NULL;
	humanplaybuttontex = NULL;
	AIplaybuttontex = NULL;

	/* If decided to quit game */
	if(quit == true)
	{
		/* Do clean up */
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_CloseFont(font);
		return 0;
	}

	/* Prepare text */

	std::string first_player;
	std::string second_player;

	SDL_Texture *boxshadow = loadimage("data/boxshadow.png", renderer);

	if(playing_mode == 2)
	{

//		msgsurf = TTF_RenderUTF8_Solid(font, "Imię pierwszego gracza: ", text_color);
//		msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
//		shadowsurf = TTF_RenderUTF8_Solid(font, "Imię pierwszego gracza: ", text_color);
//		shadowtex = SDL_CreateTextureFromSurface(renderer, shadowsurf);

		gen_text_dual_color(font, "Imię pierwszego gracza: ", &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

		
		msgrect.x = (windowx / 2) - (msgsurf->w / 2);
		msgrect.y = 0;
		msgrect.w = msgsurf->w;
		msgrect.h = msgsurf->h;

		msgshadowrect.x = (windowx / 2) - (msgsurf->w / 2) + (1/windowx);
		msgshadowrect.y = 1;
		msgshadowrect.w = msgsurf->w;
		msgshadowrect.h = msgsurf->h;

		/* Render name input */
		boxshadow = loadimage("data/boxshadow.png", renderer);
		if(boxshadow == NULL)
		{
			std::cout << "Failed to load data/boxshadow.png" << std::endl;
		}

		SDL_Surface *inputbox = TTF_RenderUTF8_Blended_Wrapped(font, "_", text_color, windowx - (windowx * 0.083));
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
		SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
		SDL_RenderPresent(renderer);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
		SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
		SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);

		SDL_RenderPresent(renderer);

		std::string text;
		bool enter_pressed = false;
		SDL_StartTextInput();

		SDL_Event last_keydown;

		while(enter_pressed == false && quit == false)
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
			if(events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RETURN)
			{
				enter_pressed = true;
			}
			if(events.type == SDL_QUIT)
			{
				/* Do clean up */
				SDL_DestroyTexture(msgtex);
				SDL_DestroyTexture(shadowtex);
				SDL_DestroyTexture(inputboxtex);
				SDL_DestroyTexture(texture_from_screen);
				SDL_DestroyTexture(boxshadow);
				SDL_FreeSurface(msgsurf);
				SDL_FreeSurface(shadowsurf);
				SDL_FreeSurface(inputbox);

				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);

				return 0;
			}
			SDL_RenderCopy(renderer, texture_from_screen, NULL, NULL);
			SDL_RenderCopy(renderer, boxshadow, NULL, &inputboxrect);
			SDL_RenderCopy(renderer, inputboxtex, NULL, &inputboxrect);
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();

		/* Avoid accidental ENTER key press */
		while(events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RETURN)
		{
			SDL_PollEvent(&events);
		}
		
		first_player = text;
		text = "";

		SDL_DestroyTexture(msgtex);
		SDL_DestroyTexture(shadowtex);
		SDL_DestroyTexture(texture_from_screen);
		SDL_FreeSurface(msgsurf);
		SDL_FreeSurface(shadowsurf);

		/* Second player's name */
		gen_text_dual_color(font, "Imię drugiego gracza: ", &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

		msgrect.x = (windowx / 2) - (msgsurf->w / 2);
		msgrect.y = 0;
		msgrect.w = msgsurf->w;
		msgrect.h = msgsurf->h;

		msgshadowrect.x = (windowx / 2) - (msgsurf->w / 2) + (1/windowx);
		msgshadowrect.y = 1;
		msgshadowrect.w = msgsurf->w;
		msgshadowrect.h = msgsurf->h;
		
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
		SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
		
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
			if(events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RETURN)
			{
				enter_pressed = true;
			}
			if(events.type == SDL_QUIT)
			{
				/* Do clean up */
				SDL_DestroyTexture(msgtex);
				SDL_DestroyTexture(shadowtex);
				SDL_DestroyTexture(inputboxtex);
				SDL_DestroyTexture(texture_from_screen);
				SDL_DestroyTexture(boxshadow);
				SDL_FreeSurface(msgsurf);
				SDL_FreeSurface(shadowsurf);
				SDL_FreeSurface(inputbox);

				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);

				return 0;
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
		SDL_DestroyTexture(msgtex);
		SDL_DestroyTexture(shadowtex);
		SDL_FreeSurface(msgsurf);
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
		}		gen_text_dual_color(font, str.c_str(), &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

		gen_text_dual_color(font, str.c_str(), &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

	}
	else
	{


		gen_text_dual_color(font, "Wybierz swój znak: ", &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer);

		std::cout << shadowsurf->w << ", " << shadowsurf->h << std::endl;

	}
	/* Clear screen */
	SDL_RenderClear(renderer);

	/* Render textures */

//	SDL_Rect msgrect;
	msgrect.x = (windowx / 2) - (msgsurf->w / 2);
	msgrect.y = 0;
	msgrect.w = msgsurf->w;
	msgrect.h = msgsurf->h;

//	SDL_Rect msgshadowrect;
	msgshadowrect.x = (windowx / 2) - (msgsurf->w / 2) + (1/windowx);
	msgshadowrect.y = 1;
	msgshadowrect.w = msgsurf->w;
	msgshadowrect.h = msgsurf->h;

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
		player2_turntex = SDL_CreateTextureFromSurface(renderer, player2_turn);
	}
	else
	{
		player1_turn = TTF_RenderUTF8_Solid(font, "Twój ruch", text_color);
		player1_turntex = SDL_CreateTextureFromSurface(renderer, player1_turn);
	}

	std::string player1_sign;
	std::string player2_sign;

#ifdef ANIMATIONS
	render_fade_in_all_special(crosstex, circletex, background, msgtex, shadowtex, renderer, msgrect, msgshadowrect, crossrect, circlerect);
#else
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
	SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
	SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
	SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
	SDL_RenderPresent(renderer);
#endif	


	/* Select the sign */
	std::string AI_sign;
	if(playing_mode == 1)
	{
		while(AI_sign == "")
		{
			SDL_PollEvent(&events);
			if(events.type == SDL_QUIT)
			{
				/* Clean up */
				SDL_DestroyTexture(msgtex);
				SDL_DestroyTexture(shadowtex);
				SDL_DestroyTexture(crosstex);
				SDL_DestroyTexture(circletex);
				SDL_DestroyTexture(background);
				SDL_FreeSurface(msgsurf);
				SDL_FreeSurface(shadowsurf);
				SDL_DestroyTexture(player1_turntex);
				SDL_FreeSurface(player1_turn);
				return 0;
			}
			if(events.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				/* Decide which field player clicked */
				if(x < (crossrect.x + crossrect.w) && x > crossrect.x && y < (crossrect.y + crossrect.h) && y > crossrect.y)
				{
					player1_sign = "X";
					std::cout << "CROSS SELECTED" << std::endl;
					AI_sign = "O";

#ifdef ANIMATIONS
					render_fade_out_all_special(crosstex, circletex, background, msgtex, shadowtex, renderer, msgrect, msgshadowrect, crossrect, circlerect);
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif					
				}
				if(x < (circlerect.x + circlerect.w) && x > circlerect.x && y < (circlerect.y + circlerect.h) && y > circlerect.y)
				{
					player1_sign = "O";
					std::cout << "CIRCLE SELECTED" << std::endl;
					AI_sign = "X";
#ifdef ANIMATIONS
					render_fade_out_all_special(circletex, crosstex, background, msgtex, shadowtex, renderer, msgrect, msgshadowrect, circlerect, crossrect);
#else
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderPresent(renderer);
#endif					
				}
			}
		}
	}
		std::cout << "AI_sign: " << AI_sign << std::endl;
		std::cout << "Your sign: " << player1_sign << std::endl;
		bool finish = false;
#ifdef ANIMATIONS
	/* Revert alpha values of textures */
	SDL_SetTextureAlphaMod(crosstex, 255);
	SDL_SetTextureAlphaMod(circletex, 255);
#endif

		std::cout << "player1_sign: " << player1_sign << std::endl;
		std::cout << "player2 sign: " << player2_sign << std::endl;
		finish = false;
		double player1_time_secs = 0;
		double player2_time_secs = 0;
		int num_player1_turns = 0; /* Number of player turns */
		int num_player2_turns = 0; /* Number of player turns */
		int who = -1; /* Who won the game */

#ifndef ANIMATIONS
		SDL_Delay(1000); /* Avoid accidental click */
#endif
		/* Prepare data */
		std::string str;
		if(player_id == 1)
		{
			str = first_player + " pierwszy wybierał znak więc " + second_player + " zaczyna\n\nKliknij by kontynuować";
		}
		else
		{
			str = second_player + " pierwszy wybierał znak więc " + first_player + " zaczyna\n\nKliknij by kontynuować";
		}

		SDL_Surface *info = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
		SDL_Texture *infotex = SDL_CreateTextureFromSurface(renderer, info);
		SDL_Surface *infoshadow = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), shadow_color, windowx - (windowx * 0.083));
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

		bool stop = false;

		while(stop == false)
		{
			if(SDL_PollEvent(&events))
			{
				if(events.type == SDL_MOUSEBUTTONDOWN)
				{
					stop = true;
				}
			}
		}

//		SDL_Texture *player1screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
//		SDL_Texture *player2screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowx, windowy);
//		SDL_SetRenderTarget(renderer, player1screentex);
//		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_Rect turnrect1;
		turnrect1.x = (windowx / 2) - (player1_turn->w / 2);
		turnrect1.y = 0;
		turnrect1.w = player1_turn->w;
		turnrect1.h = player1_turn->h;

//		SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect);
//		SDL_RenderCopy(renderer, player1_turntex, NULL, &turnrect);
//		SDL_RenderPresent(renderer);
		
//		SDL_SetRenderTarget(renderer, NULL);
		
//		SDL_SetRenderTarget(renderer, player2screentex);
		SDL_Rect turnrect2;
		turnrect2.x = (windowx / 2) - (player2_turn->w / 2);
		turnrect2.y = 0;
		turnrect2.w = player2_turn->w;
		turnrect2.h = player2_turn->h;

//		SDL_RenderCopy(renderer, background, NULL, NULL);
//		SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect);
//		SDL_RenderCopy(renderer, player2_turntex, NULL, &turnrect);
//		SDL_RenderPresent(renderer);
	
//		SDL_SetRenderTarget(renderer, NULL);
//		SDL_RenderClear(renderer);
//		SDL_RenderCopy(renderer, player2screentex, NULL, NULL);
//		SDL_RenderPresent(renderer);
//		SDL_Delay(2000);

	SDL_RenderClear(renderer);

	while(finish == false && (circles_num + crosses_num) < 9)
	{

		struct timeval player1_turn_start;
		struct timeval player2_turn_start;
		struct timeval player2_turn_end;
		struct timeval player1_turn_end;

		/* Generate 2 backgrounds */

		bool player1_turn_done = false;
		bool player2_turn_done = false;
		quit = false;
		int who = -1; /* Who won the game */
		int field_clicked = -1;

		std::cout << "While loop()" << std::endl;

		if(playing_mode == 2) /* If play 2 human players */
		{
			if(player_id == 1)
			{
				std::cout << "player_id: " << player_id << std::endl;
				SDL_RenderCopy(renderer, background, NULL, NULL);
				SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect1);
				SDL_RenderCopy(renderer, player1_turntex, NULL, &turnrect1);
				draw_board_nobg_noclear(circles, circles_num, crosses, crosses_num, board, crosstex, circletex, renderer);
//				SDL_Delay(2000);
				gettimeofday(&player1_turn_start, NULL);
				while(player1_turn_done == false && quit == false && (crosses_num + circles_num) < 9 && who == -1)
				{
					std::cout << "Player1 loop" << std::endl;
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect1);
					SDL_RenderCopy(renderer, player1_turntex, NULL, &turnrect1);
					draw_board_nobg_noclear(circles, circles_num, crosses, crosses_num, board, crosstex, circletex, renderer);
					if(SDL_PollEvent(&events))
					{
						if(events.type == SDL_QUIT)
						{
							/* TODO: Cleanup calls */
							quit = true;
						}
						if(events.type == SDL_MOUSEBUTTONDOWN)
						{
							int mousex = 0;
							int mousey = 0;
							SDL_GetMouseState(&mousex, &mousey);
							field_clicked = which_field_player_clicked(mousex, mousey);
							std::cout << "Player clicked field " << field_clicked << std::endl;
						}
					}
					/* Check if clicked field isn't already used */
					if(field_clicked != -1)
					{
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
							/* Next turn is player2's */
							player_id = 2;
							gettimeofday(&player1_turn_end, NULL);
							num_player1_turns++;
							draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
						}
						else
						{
							std::cout << "This field is already used" << std::endl;
						}
						/* Calculate time */
						player1_time_secs = player1_time_secs + (player1_turn_end.tv_sec - player1_turn_start.tv_sec) + ((player1_turn_end.tv_usec - player1_turn_start.tv_usec)/1000000.0);
					}
				}
				/* Check if smoeone has won */
				who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				if(who == 0)
				{
					std::cout << "Player1 won!!!!" << std::endl;
					/* Render text */
					char player_turns_str[3];
					sprintf(player_turns_str, "%d", num_player1_turns);
					char player_time_str[128];
					sprintf(player_time_str, "%'.3f", player1_time_secs);
					std::string str = "Gratulacje " + first_player + ", wygrałeś/aś! czas gry: ";
					str += player_time_str;
					str += " s, liczba ruchów: ";
					str += player_turns_str;
					str += "\n\nKliknij aby wyjść z gry";
					msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;
					
					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;
					
					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
						/* Calculate time */
						player1_time_secs = player1_time_secs + (player1_turn_end.tv_sec - player1_turn_start.tv_sec) + ((player1_turn_end.tv_usec - player1_turn_start.tv_usec)/1000000.0);
					}
					SDL_DestroyTexture(msgtex);
					msgtex = NULL;
				}
				if(who == 1)
				{
					std::cout << "Player2 won!!!!" << std::endl;
					/* Render text */
					char player_turns_str[3];
					sprintf(player_turns_str, "%d", num_player2_turns);
					char player_time_str[128];
					sprintf(player_time_str, "%'.3f", player2_time_secs);
					std::string str = "Gratulacje " + second_player + ", wygrałeś/aś! czas gry: ";
					str += player_time_str;
					str += " s, liczba ruchów: ";
					str += player_turns_str;
					str += "\n\nKliknij aby wyjść z gry";
					msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;
					
					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;
					
					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
					}
					SDL_DestroyTexture(msgtex);
					msgtex = NULL;

				}
				if(who == -1 && (circles_num + crosses_num) == 9)
				{
					/* No one won */
					msgsurf = TTF_RenderUTF8_Solid(font, "Niestety nikt nie wygrał. Kliknij aby wyjść", text_color);
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;

					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;

					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
					}
				}
			}
			if(player_id == 2)
			{
				std::cout << "player_id: " << player_id << std::endl;
				SDL_RenderCopy(renderer, background, NULL, NULL);
				SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect2);
				SDL_RenderCopy(renderer, player2_turntex, NULL, &turnrect2);
				draw_board_nobg_noclear(circles, circles_num, crosses, crosses_num, board, crosstex, circletex, renderer/*, textures_infos*/);	
//				SDL_Delay(2000);
				gettimeofday(&player1_turn_start, NULL);
				while(player2_turn_done == false && quit == false && (crosses_num + circles_num) < 9 && who == -1)
				{
					std::cout << "Player2 loop" << std::endl;

					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderCopy(renderer, boxshadow, NULL, &turnrect2);
					SDL_RenderCopy(renderer, player2_turntex, NULL, &turnrect2);
					draw_board_nobg_noclear(circles, circles_num, crosses, crosses_num, board, crosstex, circletex, renderer/*, textures_infos*/);	
					if(SDL_PollEvent(&events))
					{
						if(events.type == SDL_QUIT)
						{
							/* TODO: Cleanup calls */
							quit = true;
						}
						if(events.type == SDL_MOUSEBUTTONDOWN)
						{
							int mousex = 0;
							int mousey = 0;
							SDL_GetMouseState(&mousex, &mousey);
							field_clicked = which_field_player_clicked(mousex, mousey);
						}
					}
					/* Check if clicked field isn't already used */
					if(field_clicked != -1)
					{
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
							/* Next turn is player1's */
							player_id = 1;
							gettimeofday(&player2_turn_end, NULL);
							num_player2_turns++;
							draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
						}
						else
						{
							std::cout << "This field is already used" << std::endl;
						}
						/* Calculate time */
						player2_time_secs = player2_time_secs + (player2_turn_end.tv_sec - player2_turn_start.tv_sec) + ((player2_turn_end.tv_usec - player2_turn_start.tv_usec)/1000000.0);
					}
				}
				/* Check if smoeone has won */
				who = who_won(crosses, crosses_num, circles, circles_num, "NaN", player1_sign);
				if(who == 0)
				{
					std::cout << "Player1 won!!!!" << std::endl;
					/* Render text */
					char player_turns_str[3];
					sprintf(player_turns_str, "%d", num_player1_turns);
					char player_time_str[128];
					sprintf(player_time_str, "%'.3f", player1_time_secs);
					std::string str = "Gratulacje " + first_player + ", wygrałeś/aś! czas gry: ";
					str += player_time_str;
					str += " s, liczba ruchów: ";
					str += player_turns_str;
					str += "\n\nKliknij aby wyjść z gry";
					msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;
					
					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;
					
					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
					}
					SDL_DestroyTexture(msgtex);
					msgtex = NULL;
				}
				if(who == 1)
				{
					std::cout << "Player2 won!!!!" << std::endl;
					/* Render text */
					char player_turns_str[3];
					sprintf(player_turns_str, "%d", num_player2_turns);
					char player_time_str[128];
					sprintf(player_time_str, "%'.3f", player2_time_secs);
					std::string str = "Gratulacje " + second_player + ", wygrałeś/aś! czas gry: ";
					str += player_time_str;
					str += " s, liczba ruchów: ";
					str += player_turns_str;
					str += "\n\nKliknij aby wyjść z gry";
					msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;
					
					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;
					
					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
					}
					SDL_DestroyTexture(msgtex);
					msgtex = NULL;
				}
				if(who == -1 && (circles_num + crosses_num) == 9)
				{
					/* No one won */
					msgsurf = TTF_RenderUTF8_Solid(font, "Niestety nikt nie wygrał. Kliknij aby wyjść", text_color);
					msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
					msgrect.w = msgsurf->w;
					msgrect.h = msgsurf->h;
					msgrect.x = (windowx / 2) - (msgsurf->w / 2);
					msgrect.y = 0;

					SDL_FreeSurface(msgsurf);
					msgsurf = NULL;

					/* Display it all */
					quit = false;
					SDL_Delay(1000);
					while(quit == false)
					{
						if(SDL_PollEvent(&events))
						{
							if(events.type == SDL_QUIT || events.type == SDL_MOUSEBUTTONDOWN)
							{
								quit = true;
								finish = true;
								player_id = 0;
							}
						}
						/* TODO: animations, score table */
						SDL_RenderCopy(renderer, background, NULL, NULL);
						SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
						SDL_RenderPresent(renderer);
					}
				}
			}
		}
		if(playing_mode == 1)
		{
			/* Sign choosing menu */
			msgsurf = TTF_RenderUTF8_Solid(font, "wybierz swój znak:", text_color);
			msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
			msgrect.w = msgsurf->w;
			msgrect.h = msgsurf->h;
			msgrect.x = (windowx / 2) - (msgsurf->w / 2);
			msgrect.y = 0;

			while(quit == false && player1_sign == "")
			{
				SDL_RenderCopy(renderer, background, NULL, NULL);
				SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
				SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
				SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
				SDL_RenderPresent(renderer);
				if(SDL_PollEvent(&events))
				{
					if(events.type == SDL_QUIT)
					{
						quit = true;
					}
					if(events.type == SDL_MOUSEBUTTONDOWN)
					{
						int x = 0;
						int y = 0;
						SDL_GetMouseState(&x, &y);
						if(x > crossrect.x && x < (crossrect.x + crossrect.w) && y > crossrect.y && y < (crossrect.y + crossrect.h))
						{
							player1_sign = "X";
							AI_sign = "O";
						}
						if(x > circlerect.x && x < (circlerect.x + circlerect.w) && y > circlerect.y && y < (circlerect.y + circlerect.h))
						{
							player1_sign = "O";
							AI_sign = "X";
						}
					}
				}
			}
			SDL_FreeSurface(msgsurf);
			SDL_DestroyTexture(msgtex);
			msgsurf = TTF_RenderUTF8_Solid(font, "Twój ruch", text_color);
			msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
			msgrect.w = msgsurf->w;
			msgrect.h = msgsurf->h;
			msgrect.x = (windowx / 2) - (msgsurf->w / 2);
			msgrect.y = 0;
			while(quit == false)
			{
				gettimeofday(&player1_turn_start, NULL);
				while(player1_turn_done == false)
				{
					SDL_RenderCopy(renderer, background, NULL, NULL);
					SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
					draw_board_nobg_noclear(circles, circles_num, crosses, crosses_num, board, crosstex, circletex, renderer);
					if(SDL_PollEvent(&events))
					{
						if(events.type == SDL_QUIT)
						{
							quit = true;
						}
						if(events.type == SDL_MOUSEBUTTONDOWN)
						{
							int mousex = 0;
							int mousey = 0;
							SDL_GetMouseState(&mousex, &mousey);
							field_clicked = which_field_player_clicked(mousex, mousey);
							if(field_clicked != -1)
							{
								/* Check if field isn't already used */
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
									num_player1_turns++;
									gettimeofday(&player1_turn_end, NULL);
									/* Calculate time */
									player1_time_secs = player1_time_secs + (player1_turn_end.tv_sec - player1_turn_start.tv_sec) + ((player1_turn_end.tv_usec - player1_turn_start.tv_usec)/1000000.0);
									draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
								}
								who = who_won(crosses, crosses_num, circles, circles_num, AI_sign, player1_sign);
								if(who == 0)
								{
									std::string str = "Gratulacje! Wygrałeś/aś!\nCzas gry: ";
									char player_time_str[128];
									sprintf(player_time_str, "%'.3f", player1_time_secs);
									char player_turns_str[3];
									sprintf(player_turns_str, "%d", num_player1_turns);
									str += player_time_str;
									str += "\nLiczba ruchów: ";
									str += player_turns_str;
									str += "\nPodaj swoje imię i wciśnij ENTER lub kliknij aby wyjść";
									SDL_DestroyTexture(msgtex);
									msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), text_color, windowx - (windowx * 0.083));
									msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
									SDL_Surface *inputbox = TTF_RenderUTF8_Solid(font, "_", text_color);
									SDL_Texture *inputtex = SDL_CreateTextureFromSurface(renderer, inputbox);
									
									SDL_Rect inputrect;
									inputrect.x = (windowx / 2) - (inputbox->w / 2);
									inputrect.y = (windowy / 2);
									inputrect.w = inputbox->w;
									inputrect.h = inputbox->h;
									
									msgrect.w = msgsurf->w;
									msgrect.h = msgsurf->h;
									msgrect.x = (windowx / 2) - (msgsurf->w / 2);
									msgrect.y = 0;
									quit = false;
									SDL_StartTextInput();
									std::string name;
									while(quit == false)
									{
										SDL_RenderCopy(renderer, background, NULL, NULL);
										SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
										SDL_RenderCopy(renderer, inputtex, NULL, &inputrect);
										SDL_RenderPresent(renderer);
										if(SDL_PollEvent(&events))
										{
											if(events.type == SDL_QUIT)
											{
												quit = true;
											}
											if(events.type == SDL_MOUSEBUTTONDOWN)
											{
												quit = true;
											}
											if(events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RETURN)
											{
												quit = true;
											}
											if(events.type == SDL_TEXTINPUT)
											{
												name += events.text.text;
												/* Update text */
												inputbox = TTF_RenderUTF8_Solid(font, name.c_str(), text_color);
												inputtex = SDL_CreateTextureFromSurface(renderer, inputbox);
												inputrect.x = (windowx / 2) - (inputbox->w / 2);
												inputrect.y = (windowy / 2);
												inputrect.w = inputbox->w;
												inputrect.h = inputbox->h;
											}
										}
									}
									SDL_StopTextInput();
									SDL_DestroyTexture(inputtex);
									SDL_DestroyTexture(msgtex);
									SDL_FreeSurface(msgsurf);
									SDL_FreeSurface(inputbox);
								}
								if(who == 1)
								{
									/* Clean up */
									SDL_DestroyTexture(msgtex);
									SDL_DestroyTexture(shadowtex);
									SDL_DestroyTexture(crosstex);
									SDL_DestroyTexture(circletex);
									SDL_FreeSurface(msgsurf);
									SDL_FreeSurface(shadowsurf);

									std::string str = "Niestety przegrałeś/aś. Ale to nic, następnym razem napewno pójdzie lepiej (Wciśnij ENTER lub kliknij aby wyjść)";
									gen_text_dual_color_wrapped(font, str.c_str(), &msgtex, &msgsurf, &shadowtex, &shadowsurf, text_color, shadow_color, renderer, windowx);

									msgrect.w = msgsurf->w;
									msgrect.h = msgsurf->h;
									msgrect.x = (windowx / 2) - (msgsurf->w / 2);
									msgrect.y = 0;
			
									msgshadowrect.w = shadowsurf->w;
									msgshadowrect.h = shadowsurf->h;
									msgshadowrect.x = (windowx / 2) - (msgsurf->w / 2) + (1/windowx);
									msgshadowrect.y = 1;
								
									while(quit == false)
									{
										if(SDL_PollEvent(&events))
										{
											if(events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RETURN)
											{
												quit = true;
											}
											if(events.type == SDL_MOUSEBUTTONDOWN)
											{
												quit = true;
											}
											if(events.type == SDL_QUIT || quit == true)
											{
												/* Do clean up */
												SDL_DestroyTexture(msgtex);
												SDL_DestroyTexture(shadowtex);
												SDL_FreeSurface(msgsurf);
												SDL_FreeSurface(shadowsurf);
			
												SDL_DestroyRenderer(renderer);
												SDL_DestroyWindow(window);
				
												return 0;
											}
											SDL_RenderCopy(renderer, background, NULL, NULL);
											SDL_RenderCopy(renderer, shadowtex, NULL, &msgshadowrect);
											SDL_RenderCopy(renderer, msgtex, NULL, &msgrect);
											SDL_RenderPresent(renderer);
										}
									}
								}
								if(who == -1)
								{
									AI_turn(circles, circles_num, crosses, crosses_num, false, AI_sign);
									draw_board(circles, circles_num, crosses, crosses_num, background, board, crosstex, circletex, renderer);
								}
								if(who == -1 && (crosses_num + circles_num) == 9)
								{
									SDL_DestroyTexture(msgtex);
									SDL_DestroyTexture(shadowtex);
									SDL_FreeSurface(msgsurf);
									SDL_FreeSurface(shadowsurf);
									msgsurf = TTF_RenderUTF8_Blended_Wrapped(font, "Niestety nikt nie wygrał. Kliknij aby wyjść.", text_color, windowx - (windowx * 0.083));
									msgtex = SDL_CreateTextureFromSurface(renderer, msgsurf);
							
									quit = false;
									while(quit == false)
									{
										if(SDL_PollEvent(&events))
										{
											if(events.type == SDL_MOUSEBUTTONDOWN)
											{
												SDL_DestroyTexture(msgtex);
												SDL_FreeSurface(msgsurf);
												SDL_DestroyRenderer(renderer);
												SDL_DestroyWindow(window);
												quit = true;
											}
										}
									}
									return 0;

								}
							}
						}
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
	SDL_DestroyTexture(msgtex);
	SDL_DestroyRenderer(renderer);
	
	SDL_DestroyWindow(window);
	SDL_Quit;

return 0;
}

