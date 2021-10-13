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
  double pi = M_PI;
  int r = 0;
  int s = 0;
  int height = img -> h;
  int ymid = height/2;
  int width  = img -> w;
  int xmid = width/2;
  Uint32 pixel = 0;
  SDL_Surface* rotate = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);
  double nangle = (angle*180)/pi;
  if(nangle < 0)
    {
      nangle = -nangle;
    }
  for(int x = 0; x < width; x++)
    {
      for(int y = 0; y < height; y++)
	{
	  pixel = 0;
	  r = (x-xmid)*cos(-nangle) - (y-ymid)*sin(-nangle) + xmid;
	  s = (x-xmid)*sin(-nangle) + (y-ymid)*cos(-nangle) + ymid;
	  if(r > 0 && r < xmid && s > 0 && s < ymid)
	    {
	       pixel = get_pixel(img,x,y);
	    }
	  put_pixel(rotate,r,s,pixel);
	 
	}
    }
  return rotate;
}

int main()
{
  init_sdl();
  SDL_Surface* img_surface;
  SDL_Surface* rotate;
  SDL_Surface* screen_surface;

  img_surface = load_image("thanatos");
  rotate = rotation(img_surface, 90);
  int h = rotate -> h;
  int w = rotate -> w;
  screen_surface = SDL_CreateRGBSurface(0,h,w,32,0,0,0,0);

  update_surface(screen_surface, rotate);
  screen_surface = display_image(rotate);
  wait_for_keypressed();
  SDL_FreeSurface(img_surface);
  SDL_FreeSurface(screen_surface);
  SDL_FreeSurface(rotate);

  sdl_quitting();
}
