#include "game.h"

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <iostream>

void gen_text_dual_color(TTF_Font *font, const char *text, SDL_Texture **uppertextbuf, SDL_Surface **uppertextsurf, SDL_Texture **lowertextbuf, SDL_Surface **lowertextsurf, SDL_Color uppercolor, SDL_Color lowercolor, SDL_Renderer *r)
{
	*uppertextsurf = TTF_RenderUTF8_Solid(font, text, uppercolor);
	*uppertextbuf  = SDL_CreateTextureFromSurface(r, *uppertextsurf);
	*lowertextsurf = TTF_RenderUTF8_Solid(font, text, lowercolor);
	*lowertextbuf = SDL_CreateTextureFromSurface(r, *lowertextsurf);
}

void gen_text_dual_color_wrapped(TTF_Font *font, const char *text, SDL_Texture **uppertextbuf, SDL_Surface **uppertextsurf, SDL_Texture **lowertextbuf, SDL_Surface **lowertextsurf, SDL_Color uppercolor, SDL_Color lowercolor, SDL_Renderer *r, int windowx)
{
	*uppertextsurf = TTF_RenderUTF8_Blended_Wrapped(font, text, uppercolor, windowx - 0.083 * windowx);
	*uppertextbuf  = SDL_CreateTextureFromSurface(r, *uppertextsurf);
	*lowertextsurf = TTF_RenderUTF8_Blended_Wrapped(font, text, lowercolor, windowx - 0.083 * windowx);
	*lowertextbuf = SDL_CreateTextureFromSurface(r, *lowertextsurf);
}

void gen_text_shadowed(TTF_Font *font, const char *text, SDL_Texture **text_texture, SDL_Surface **text_surf, SDL_Rect &text_rect, SDL_Texture **shadow_texture, SDL_Surface **shadow_surf, SDL_Rect &shadow_rect, SDL_Color color, SDL_Renderer *r)
{
	*text_surf = TTF_RenderUTF8_Solid(font, text, color);
	*text_texture = SDL_CreateTextureFromSurface(r, *text_surf);
	/* Set up info, because text should not be scaled */
	text_rect.w = (*text_surf)->w;
	text_rect.h = (*text_surf)->h;
	
	shadow_rect.w = (*text_surf)->w;
	shadow_rect.h = (*text_surf)->h;
}

#ifdef ANIMATIONS
void render_fade_in_all_special(SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Texture *background, SDL_Texture *message_texture, SDL_Texture *shadow_texture, SDL_Renderer *r, SDL_Rect message_rect, SDL_Rect shadow_rect, SDL_Rect cross_rect, SDL_Rect circle_rect)
{	
	Uint8 text_alpha = 0;
	Uint8 signs_alpha = 0;
	/* Set alpha blending */
	SDL_SetTextureBlendMode(message_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(shadow_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(cross_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(circle_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(message_texture, text_alpha);
	SDL_SetTextureAlphaMod(shadow_texture, text_alpha);
	SDL_SetTextureAlphaMod(cross_texture, signs_alpha);
	SDL_SetTextureAlphaMod(circle_texture, signs_alpha);
	

	
	while(text_alpha < 255)
	{
		SDL_RenderCopy(r, background, NULL, NULL);
		SDL_SetTextureAlphaMod(message_texture, text_alpha);
		SDL_SetTextureAlphaMod(shadow_texture, text_alpha);
		if(text_alpha > 100)
		{
			SDL_SetTextureAlphaMod(cross_texture, text_alpha);
			SDL_SetTextureAlphaMod(circle_texture, text_alpha);
			SDL_RenderCopy(r, cross_texture, NULL, &cross_rect);
			SDL_RenderCopy(r, circle_texture, NULL, &circle_rect);
			signs_alpha+=((100 / 255) / 155);
		}
		SDL_RenderCopy(r, shadow_texture, NULL, &shadow_rect);
		SDL_RenderCopy(r, message_texture, NULL, &message_rect);
		SDL_RenderPresent(r);
		text_alpha++;
	}

}

void render_fade_out_all_special(SDL_Texture *selected_texture, SDL_Texture *remaining_texture, SDL_Texture *background, SDL_Texture *message_texture, SDL_Texture *shadow_texture, SDL_Renderer *r, SDL_Rect message_rect, SDL_Rect shadow_rect, SDL_Rect selected_rect, SDL_Rect remaining_rect)
{
	Uint8 selected_alpha = 255;
	Uint8 text_alpha = 255;
	SDL_SetTextureAlphaMod(message_texture, text_alpha);
	SDL_SetTextureAlphaMod(shadow_texture, text_alpha);
	while(selected_alpha > 0)
	{
		SDL_RenderCopy(r, background, NULL, NULL);
		SDL_RenderCopy(r, shadow_texture, NULL, &shadow_rect);
		SDL_RenderCopy(r, message_texture, NULL, &message_rect);
		SDL_RenderCopy(r, remaining_texture, NULL, &remaining_rect);
		SDL_SetTextureAlphaMod(selected_texture, selected_alpha);
		SDL_RenderCopy(r, selected_texture, NULL, &selected_rect);
		SDL_RenderPresent(r);
		selected_alpha--;
	}
	/* Fade out ramining things */
	while(text_alpha > 0)
	{
		SDL_SetTextureAlphaMod(shadow_texture, text_alpha);
		SDL_SetTextureAlphaMod(message_texture, text_alpha);
		SDL_SetTextureAlphaMod(remaining_texture, text_alpha);

		SDL_RenderCopy(r, background, NULL, NULL);
		SDL_RenderCopy(r, shadow_texture, NULL, &shadow_rect);
		SDL_RenderCopy(r, message_texture, NULL, &message_rect);
		SDL_RenderCopy(r, remaining_texture, NULL, &remaining_rect);
		SDL_RenderPresent(r);
		text_alpha-=5;
	}

}

#endif
