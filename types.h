#ifndef _TYPES_H_
#define _TYPES_H_
#ifdef WINDOWS_BUILD
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

typedef struct coords2d {
	int x;
	int y;
} coords2d;

#define BG_TEXTURE_INDEX	0 /* Background texture index in scene->(*textures) */
#define CROSS_TEXTURE_INDEX	1
#define CIRCLE_TEXTURE_INDEX	2

#define CROSS_RECT_INDEX	0
#define CIRCLE_RECT_INDEX	1

typedef struct scene {
	SDL_Texture *textures;
	int num_textures;
	SDL_Surface *surfaces;
	int num_surfaces;
	SDL_Rect *rectangles;
	int num_rectangles;
} scene;

#endif /* _TYPES_H_ */
