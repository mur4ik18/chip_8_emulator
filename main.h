#ifndef MAIN_H
#define MAIN_H

#include </opt/homebrew/include/SDL2/SDL.h>
#include <stdbool.h>

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
  bool gfx[64 * 32];

  // interupts and hardware registers
  unsigned char delay_timer;
  unsigned char sound_timer;

  unsigned short stack[16];
  // stack pointer
  unsigned short sp;
  unsigned char key[16];
  bool drawFlag;

} Context;

typedef struct
{
  SDL_Renderer *renderer;
  SDL_Window *window;
} App;

unsigned char chip8_fontset[] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

App app;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

void graphique_setup (void);
void doInput (void);
void prepareScene (void);
void presentScene (void);
void render_graphic (Context *ctx);
void init (Context *ctx);
void read_rom (Context *ctx);
void cycle_emulator (Context *ctx);
// graphique
void graphique_setup (void);
void graphique_break (void);

FILE *fptr;

#endif /* MAIN_H */
