/*!
  \file sdldrawtest.c
  \author Mario Palomo <mpalomo@ihman.com>
  \date 05-2002

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include "SDL/SDL.h"

#include "SDL_draw.h"

/*Hardware surfaces*/
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
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int iCmdShow)
{
  return main(__argc, __argv);
}
#endif //WIN32
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

{/*BEGIN*/
  Uint32 c_white = SDL_MapRGB(screen->format, 255,255,255);
  Uint32 c_gray = SDL_MapRGB(screen->format, 200,200,200);
  Uint32 c_dgray= SDL_MapRGB(screen->format, 64,64,64);
  Uint32 c_cyan = SDL_MapRGB(screen->format, 32,255,255);

  //SDL_Rect r = {100,300,50,50};
  //SDL_SetClipRect(screen, &r);  //Test of clipping code


  Draw_Line(screen, 100,100, 30,0, c_white);
  Draw_Line(screen, 30,0, 100,100, c_white);


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
