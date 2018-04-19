#include "game.h"

void draw_board(int *circles, int circles_count, int *crosses, int crosses_count, SDL_Texture *bg, SDL_Texture *empty, SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Renderer *wrenderer)
{

	/* Simple data inspection */
	/* Check textures */
//	SDL_RenderClear(wrenderer);
//	SDL_RenderPresent(wrenderer);


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
//	SDL_RenderClear(wrenderer);


	SDL_RenderCopy(wrenderer, bg, NULL, NULL);
	i = 0;

	while(i < 9)
	{
		SDL_RenderCopy(wrenderer, texture_array[tex_indices[i]], NULL, &r[i]);
		i++;
	}

	SDL_RenderPresent(wrenderer);
}

void draw_board_nobg_noclear(int *circles, int circles_count, int *crosses, int crosses_count, SDL_Texture *empty, SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Renderer *wrenderer)
{

	/* Simple data inspection */
	/* Check textures */
//	SDL_RenderClear(wrenderer);
//	SDL_RenderPresent(wrenderer);


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
//	SDL_RenderClear(wrenderer);


//	SDL_RenderCopy(wrenderer, bg, NULL, NULL);
	i = 0;

	while(i < 9)
	{
		SDL_RenderCopy(wrenderer, texture_array[tex_indices[i]], NULL, &r[i]);
		i++;
	}

	SDL_RenderPresent(wrenderer);
}

