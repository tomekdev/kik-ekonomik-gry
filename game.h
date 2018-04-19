#ifdef WINDOWS_BUILD
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
#endif
#include <string>
#include "types.h"

extern int circles[9];
extern int crosses[9];
extern int crosses_num;
extern int circles_num;

extern int windowx;
extern int windowy;

extern SDL_Surface *window_screen;
extern SDL_Window *window;
extern SDL_Texture *board;
extern SDL_Texture *background;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern SDL_Surface *msgsurf;
extern SDL_Texture *msgtex;


void AI_turn(int *circles, int &circles_count, int *crosses, int &crosses_count, bool hard, std::string AI_sign);
void draw_board(int *circles, int circles_count, int *crosses, int crosses_count, SDL_Texture *bg, SDL_Texture *empty, SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Renderer *wrenderer);
void draw_board_nobg_noclear(int *circles, int circles_count, int *crosses, int crosses_count, SDL_Texture *empty, SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Renderer *wrenderer);
coords2d fieldtocoords2d(int fld);
bool field_used(int field, int *fields_array, int arraysz);
bool if_array_has_value(int *array, int size, int value);
int who_won(int *crosses_array, int crosses_count, int *circles_array, int circles_count, std::string AI_sign, std::string player1_sgn); /* Returns 0 when player won, 1 when AI and -1 when no one */
int which_field_player_clicked(int posx, int posy);
SDL_Texture *loadimage(const char *path, SDL_Renderer *sdl_renderer);
bool graphics_init();
bool graphics_data_init();
void gen_text_dual_color(TTF_Font *font, const char *text, SDL_Texture **uppertextbuf, SDL_Surface **uppertextsurf, SDL_Texture **lowertextbuf, SDL_Surface **lowertextsurf, SDL_Color uppercolor, SDL_Color lowercolor, SDL_Renderer *r);
void gen_text_dual_color_wrapped(TTF_Font *font, const char *text, SDL_Texture **uppertextbuf, SDL_Surface **uppertextsurf, SDL_Texture **lowertextbuf, SDL_Surface **lowertextsurf, SDL_Color uppercolor, SDL_Color lowercolor, SDL_Renderer *r, int windowx);
void gen_text_shadowed(TTF_Font *font, const char *text, SDL_Texture **text_texture, SDL_Surface **text_surf, SDL_Rect &text_rect, SDL_Texture **shadow_texture, SDL_Surface **shadow_surf, SDL_Rect &shadow_rect, SDL_Color color, SDL_Renderer *r);
#ifdef ANIMATIONS
void render_fade_in_all_special(SDL_Texture *cross_texture, SDL_Texture *circle_texture, SDL_Texture *background, SDL_Texture *message_texture, SDL_Texture *shadow_texture, SDL_Renderer *r, SDL_Rect message_rect, SDL_Rect shadow_rect, SDL_Rect cross_rect, SDL_Rect circle_rect);
void render_fade_out_all_special(SDL_Texture *selected_texture, SDL_Texture *remaining_texture, SDL_Texture *background, SDL_Texture *message_texture, SDL_Texture *shadow_texture, SDL_Renderer *r, SDL_Rect message_rect, SDL_Rect shadow_rect, SDL_Rect selected_rect, SDL_Rect remaining_rect);
#endif
void save_score(const char *player_name, double player_time, int player_turns);
std::string choose_sign_menu(SDL_Texture *bg, SDL_Texture *cross, SDL_Texture *circle, SDL_Rect cross_rect, SDL_Rect circle_rect);
std::string choose_sign_multi_menu();

