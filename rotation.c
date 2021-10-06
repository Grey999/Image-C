# include <stdio.h>
# include <stdlib.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdio.h>
# include <assert.h>
# include <err.h>
# include <math.h>
# include "pixel_operations.h"

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

SDL_Surface* rotation(SDL_Surface *img, int  angle)
{
  double pi = 3.1415;
  int c = cos(angle/180*pi);
  int s = sin(angle/180*pi);
  int height = img -> h;
  int width  = img -> w;
  Uint32 pixel = 0;
  SDL_Surface* rotate = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);

  for(int x = 0; x < height; x++)
    {
      for(int y = 0; y < width; y++)
	{
	  pixel = get_pixel(img,x,y);
	  put_pixel(rotate,x+c,y+s,pixel);
	}
    }
  return rotate;
}

int main()
{
  init_sdl();
  SDL_Surface* img_surface;
  SDL_Surface* screen_surface;

  img_surface = load_image("thanatos.jpg");

  img_surface = rotation(img_surface, 90);
  screen_surface = display_image(img_surface);
  wait_for_keypressed();
  SDL_FreeSurface(img_surface);
  SDL_FreeSurface(screen_surface);

  sdl_quitting();
}
