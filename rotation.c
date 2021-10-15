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
  
  unsigned int r = 0;
  unsigned int s = 0;
  
  unsigned int height = img -> h;
  double ymid = height/2;
  
  unsigned int width  = img -> w;
  double xmid = width/2;
  
  Uint32 pixel = 0;
  SDL_Surface* rotate = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
  
  double nangle = ((double)angle/360.0)*2.0*pi;
  for(unsigned int x = 0; x < width; x++)
    {
      for(unsigned int y = 0; y < height; y++)
	{
	  pixel = 0;
	  r = (unsigned int)(((double)x-xmid)*cos(nangle) - ((double)y-ymid)*sin(nangle) + xmid);
	  s = (unsigned int)(((double)x-xmid)*sin(nangle) + ((double)y-ymid)*cos(nangle) + ymid);
	  if(r > 0 && r < width && s > 0 && s < height)
	    {
	      pixel = get_pixel(img,x,y);
	      put_pixel(rotate,r,s,pixel);
	      
	    }
	 
	}
    }
  return rotate;
}

int main()
{
  init_sdl();
  SDL_Surface* img_surface;
  SDL_Surface* rotate;

  img_surface = load_image("thanatos");
  rotate = rotation(img_surface, 18);
  SDL_SaveBMP(rotate, "rotation.bmp");
  SDL_FreeSurface(img_surface);
  SDL_FreeSurface(rotate);
  sdl_quitting();
}
