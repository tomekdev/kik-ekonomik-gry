
#ifdef WINDOWS_BUILD
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL.h>
#endif
#include "game.h"
#include <iostream>
#include <string>

std::string choose_sign_menu(SDL_Texture *bg, SDL_Texture *cross, SDL_Texture *circle, SDL_Rect cross_rect, SDL_Rect circle_rect)
{
	/* Prepare text */
	SDL_Color text_color = {255, 255, 255};
	SDL_Surface *textsurf = TTF_RenderUTF8_Solid(font, "Wybierz swój znak:", text_color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textsurf);
	
	SDL_Color shadow_color = {0, 0, 0};
	SDL_Surface *shadow_surf = TTF_RenderUTF8_Solid(font, "Wybierz swój znak:", shadow_color);
	SDL_Texture *shadow = SDL_CreateTextureFromSurface(renderer, shadow_surf);
	
	SDL_Rect text_rect;
	text_rect.w = textsurf->w;
	text_rect.h = textsurf->h;
	text_rect.x = (windowx / 2) - (text_rect.w / 2);
	text_rect.y = 0;
	
	/* Free surface */
	SDL_FreeSurface(textsurf);

	SDL_Rect shadow_rect;
	shadow_rect.w = shadow_surf->w;
	shadow_rect.h = shadow_surf->h;
	shadow_rect.x = text_rect.x + (windowx * 0.002);
	shadow_rect.y = windowx * 0.002;
	/* Free surface */
	SDL_FreeSurface(shadow_surf);
	
	/* Render it all */
	std::string choice;
	SDL_Event events;
	while(choice == "")
	{
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopy(renderer, shadow, NULL, &shadow_rect);
		SDL_RenderCopy(renderer, text, NULL, &text_rect);
		SDL_RenderCopy(renderer, cross, NULL, &cross_rect);
		SDL_RenderCopy(renderer, circle, NULL, &circle_rect);
		SDL_RenderPresent(renderer);
		if(SDL_PollEvent(&events))
		{
			if(events.type == SDL_QUIT)
			{
				choice = "QUIT";
			}
			if(events.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				if(x > cross_rect.x && x < (cross_rect.x + cross_rect.w) && y > cross_rect.y && y < (cross_rect.y + cross_rect.h))
				{
					choice = "X";
				}
				if(x > circle_rect.x && x < (circle_rect.x + circle_rect.w) && y > circle_rect.y && y < (circle_rect.y + circle_rect.h))
				{
					choice = "O";
				}
			}
		}
		
	}
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(shadow);
	
	return choice;

}
#if 0
std::string choose_sign_multi_menu(SDL_Texture *bg, SDL_Texture *cross, SDL_Texture *circle, &nplayer1, &player2) /* Returns player1 sign and sets players' names */
{
	std::string choice;
	/* Prepare texts for name input */
	SDL_Surface *message;
	SDL_Surface *shadow;
	SDL_Texture *tmessage;
	SDL_Texture *tshadow;
	SDL_Color shadow_color = {0 ,0 ,0};
	SDL_Color text_color = {255, 255, 255};
	
	gen_text_dual_color(font, "Imię pierwszego gracza:", &tmessage, &message, &tshadow, &shadow, text_color, shadow_color, renderer);
	
	SDL_Rect messagerect;
	messagerect.w = message->w;
	messagerect.h = message->h;
	messagerect.x = (windowx / 2) - (messagerect.w / 2);
	messagerect.y = 0;

	SDL_FreeSurface(message);

	SDL_Rect shadowrect;
	shadowrect.w = shadow->w;
	shadowrect.h = shadow->h;
	shadowrect.x = (windowx / 2) - (shadowrect.w / 2) + 1;
	shadowrect.y = 1;
		
	SDL_FreeSurface(shadow);
		
	/* Render menu */
	SDL_Event events;
	while(nplayer1 == "")
	{
		SDL_RenderCopy(renderer, background, NULL, NULL);
		SDL_RenderCopy(renderer, tshadow, NULL, &shadowrect);
		SDL_RenderCopy(renderer, tmessage, NULL, &messagerect);
		SDL_RenderCopy(renderer, crosstex, NULL, &crossrect);
		SDL_RenderCopy(renderer, circletex, NULL, &circlerect);
		SDL_RenderPresent(renderer);
		if(SDL_PollEvent(&events))
		{
			if(events.type == SDL_QUIT)
			{
				SDL_FreeSurface(message);
				SDL_FreeSurface(shadow);
				SDL_DestroyTexture(tmessage);
				SDL_DestroyTexture(tshadow);
				player1_sign = "QUIT";
			}
		}
	}
}
#endif
std::string choose_sign_multi_menu()
{

}
