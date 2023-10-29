#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
} App;

void graphique_setup (void);
void graphique_break (void);

#endif /* GRAPHIC_H */
