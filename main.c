
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
  if (argc == 2)
    {
      fptr = fopen (argv[1], "rb");
      read_rom ();
    }
  // graphique setup
  // memset(&app, 0, sizeof(App));
  // graphique_setup ();

  init ();

  // Emulation loop
  for (;;)
    {
      cycle_emulator ();
      // prepareScene ();

      // doInput ();
      // render_graphic();
      // presentScene ();

      SDL_Delay (16);
    }
  printf ("pc = %d\n", pc);
  return 0;
}

void
init (void)
{
  for (int i = 0; i < 80; ++i)
    memory[i] = chip8_fontset[i];
}

void
read_rom (void)
{
  printf("start loading your rom \n");
  char *buffer = (char*) malloc(200);
  while(fgets(buffer, 100, fptr)) {
    printf("%s ", buffer);
  }
  for(int i = 0; i < 20; ++i)
    memory[i + 512] = buffer[i];
  
  printf("a delicacy/n");
}
