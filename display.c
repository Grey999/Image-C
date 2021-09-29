# include <stdio.h>
# include <stdlib.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <stdio.h>
# include <assert.h>
# include <err.h>

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

int main()
{
  init_sdl();

  char str[50];
  
  SDL_Surface* image_surface;
  SDL_Surface* screen_surface;

  printf("Specify the path of the image: ");
  if(scanf("%s\n", str) == "thanatos.jpg")
    {
      printf("Find image.");
      printf("Please wait...");
      image_surface = load_image(str);
      screen_surface = display_image(image_surface);
      wait_for_keypressed();
      SDL_FreeSurface(image_surface);
      SDL_FreeSurface(screen_surface);
    }
  else printf("Wrong path");

  sdl_quitting();
}
