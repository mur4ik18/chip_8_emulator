#include "graphic.h"

void
graphique_setup (void)
{
  int rendererFlags, windowFlags;

  rendererFlags = SDL_RENDERER_ACCELERATED;

  windowFlags = 0;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }

  app.window = SDL_CreateWindow ("Shooter 01", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                 SCREEN_HEIGHT, windowFlags);

  if (!app.window)
    {
      printf ("Failed to open %d x %d window: %s\n", SCREEN_WIDTH,
              SCREEN_HEIGHT, SDL_GetError ());
      exit (1);
    }

  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  app.renderer = SDL_CreateRenderer (app.window, -1, rendererFlags);

  if (!app.renderer)
    {
      printf ("Failed to create renderer: %s\n", SDL_GetError ());
      exit (1);
    }
}

void
doInput (void)
{
  SDL_Event event;

  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_QUIT:
          exit (0);
          break;

        default:
          break;
        }
    }
}

void
prepareScene (void)
{
  SDL_SetRenderDrawColor (app.renderer, 0, 0, 0, 255);
  SDL_RenderClear (app.renderer);
}

void
presentScene (void)
{
  SDL_RenderPresent (app.renderer);
}

void
render_graphic (void)
{
  SDL_Rect rect;
  rect.x = 250;
  rect.y = 150;
  rect.w = 10;
  rect.h = 10;
  SDL_SetRenderDrawColor (app.renderer, 255, 255, 255, 255);
  SDL_RenderFillRect (app.renderer, &rect);
  SDL_RenderPresent (app.renderer);
}
