#ifndef TEST_CDL_HEADER_H_
#define TEST_CDL_HEADER_H_


SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
void init_sdl();
void sdl_quitting();

#endif
