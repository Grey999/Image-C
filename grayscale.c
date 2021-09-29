# include <stdio.h>
# include <stdlib.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdio.h>
# include <assert.h>
# include <err.h>

# include "pixel_operation.h"

SDL_Surface* load_image(char *path)
{
  SDL_Surface *img;

  img = IMG_Load(path);

  if(!img){
    errx(3, "can't load %s: %s", path, IMG_GetError());
  }

  return img;
}

SDL_Surface* display_image(SDL_Surface *img){

  SDL_Surface *screen;

  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);

  if(screen == NULL)
    {
      errx(1,"Couldn't set %dx%d video mode: %s\n", img->w, img->h, SDL_GetError());
    }

  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());

  SDL_UpdateRect(screen, 0, 0, img->w, img->h);

  return screen;
}


void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void init_sdl()
{
  printf("Initializing SDL.\n");
  if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1)){
    printf("Coudl not initialize SDL: %s.\n", SDL_GetError());
    exit(-1);
  }

  printf("SDL initialized.\n");
}

void sdl_quitting(){
  printf("Quiting SDL.\n");

  SDL_Quit();

  printf("Quiting....\n");
}


int main(){
  init_sdl();

  char str[50];

  SDL_Surface* image_surface;
  SDL_Surface* screen_surface;

  printf("Specify the name of your image: ");
  if(scanf("%s\n", str) < 50)
    {
      printf("Find image.");
      printf("grayscale in progress...");
      image_surface = load_image(str);
      int h = image_surface -> h;
      int w = image_surface -> w;
      Uint32 p = 0;
      Uint32 average = 0;
      Uint8 r, g, b;
      for(int x = 0, x < h, x++)
	{
	for(int y = 0, y < w, y++)
	  {
	    pixel = get_pixel(image_surface, x, y);
	    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	    average = 0.3*r + 0.59*g + 0.11*b;
	    r = average;
	    g = average;
	    b = average;
	    pixel = SDL_MapRGB(image_surface->format, r, g, b);
	    put_pixel(image_surface, x, y, pixel);
	  }
	}
  image_surface =update_surface(screen_surface, image_surface) ;
  screen_surface = display_image(image_surface);
  wait_for_keypressed();
  SDL_FreeSurface(image_surface);
  SDL_FreeSurface(screen_surface);
    }
  else printf("Wrong path");

  sdl_quitting();
}
