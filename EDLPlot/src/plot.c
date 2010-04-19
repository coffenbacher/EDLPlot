#include <stdlib.h>
#include "SDL/SDL.h"

#include "SDL_draw.h"
#include "edlPlot.h"


Uint32 FastestFlags(Uint32 flags, unsigned int width, unsigned int height,
 unsigned int bpp)
{
  const SDL_VideoInfo *info;

  flags |= SDL_FULLSCREEN;

  info = SDL_GetVideoInfo();
  if ( info->blit_hw_CC && info->blit_fill ) {
    flags |= SDL_HWSURFACE;
  }

  if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
    if ( info->video_mem*1024 > (height*width*bpp/8) ) {
      flags |= SDL_DOUBLEBUF;
    } else {
      flags &= ~SDL_HWSURFACE;
    }
  }

  return flags;
}

/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  SDL_Surface *screen;
  int width, height;
  Uint8  video_bpp;
  Uint32 videoflags;
  int done;
  SDL_Event event;
  Uint32 then, now, frames;

  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "SDL_Init problem: %s", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  videoflags = SDL_SWSURFACE | SDL_ANYFORMAT;
  width = 640;
  height = 480;
  video_bpp = 0;

  while ( argc > 1 ) {
      --argc;
           if ( strcmp(argv[argc-1], "-width") == 0 ) {
      width = atoi(argv[argc]);
      --argc;
    } else if ( strcmp(argv[argc-1], "-height") == 0 ) {
      height = atoi(argv[argc]);
      --argc;
    } else if ( strcmp(argv[argc-1], "-bpp") == 0 ) {
      video_bpp = atoi(argv[argc]);
      videoflags &= ~SDL_ANYFORMAT;
      --argc;
    } else if ( strcmp(argv[argc], "-fast") == 0 ) {
      videoflags = FastestFlags(videoflags, width, height, video_bpp);
    } else if ( strcmp(argv[argc], "-hw") == 0 ) {
      videoflags ^= SDL_HWSURFACE;
    } else if ( strcmp(argv[argc], "-flip") == 0 ) {
      videoflags ^= SDL_DOUBLEBUF;
    } else if ( strcmp(argv[argc], "-fullscreen") == 0 ) {
      videoflags ^= SDL_FULLSCREEN;
    } else {
      fprintf(stderr, "Use: %s [-bpp N] [-hw] [-flip] [-fast] [-fullscreen]\n",
              argv[0]);
      exit(1);
    }
  }/*while*/

  /*Video mode activation*/
  screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);
  if (!screen) {
    fprintf(stderr, "I can not activate video mode: %dx%d: %s\n",
            width, height, SDL_GetError());
    exit(2);
  }

{

  //Define Colors

  //1: White
  //2: Gray
  //3: Dark Gray
  //4: Red
  //5: Green
  //6: Blue
  Uint32 c_colors[] = {SDL_MapRGB(screen->format, 255,255,255), SDL_MapRGB(screen->format, 200,200,200),
		  SDL_MapRGB(screen->format, 64,64,64),  SDL_MapRGB(screen->format, 255,0,0),
		  SDL_MapRGB(screen->format, 0,255,0), SDL_MapRGB(screen->format, 0,0,255)};

  plotAxes(screen, c_colors, width, height, -20, 30, -20, 20);
  plotLine(screen, c_colors);

  SDL_UpdateRect(screen, 0, 0, 0, 0);

  frames = 0;
  then = SDL_GetTicks();
  done = 0;
  while( !done ) {
    ++frames;
    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_KEYDOWN:
        /*break;*/
        case SDL_QUIT:
          done = 1;
        break;
        default:
        break;
      }
    }/*while*/
  }/*while(!done)*/

}/*END*/

  now = SDL_GetTicks();
  if ( now > then ) {
    printf("%2.2f frames per second\n",
          ((double)frames*1000)/(now-then));
  }

  fprintf(stderr, "[END]\n");
  return 0;

}/*main*/
/*----------------------------------------------------------------------*/



