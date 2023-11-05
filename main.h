#ifndef MAIN_H
#define MAIN_H

#include "graphic.h"
#include </opt/homebrew/include/SDL2/SDL.h>

// current opcode
unsigned short opcode;

typedef struct
{
  // 4k memory
  unsigned char memory[4096];
  // CPU registers
  unsigned char V[16];

  // index register I
  // program counter pc
  // 0x000 to 0xFFF
  unsigned short I;
  unsigned short pc;

  // Memory addresses
  // 0x000 - 0x1FF - interpreter (contains font set in emu)
  // 0x050 - 0x0A0 - for the built in 4x5 pixel font set (0-F)
  // 0x200 - 0xFFF - program ROM and work RAM

  // 2048 pixels black 1 and white 0
  unsigned char gfx[64 * 32];

  // interupts and hardware registers
  unsigned char delay_timer;
  unsigned char sound_timer;

  unsigned short stack[16];
  // stack pointer
  unsigned short sp;
  unsigned char key[16];

} Context;


typedef struct
{
  SDL_Renderer *renderer;
  SDL_Window *window;
} App;

App app;

void init (Context *ctx);
void read_rom (Context *ctx);
void cycle_emulator (Context *ctx);
// graphique
void graphique_setup (void);
void graphique_break (void);

FILE *fptr;

#endif /* MAIN_H */
