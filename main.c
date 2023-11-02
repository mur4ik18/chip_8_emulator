
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      printf ("pc = %d\n", pc);
      // SDL_Delay (16);
    }

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
  if (fptr != NULL)
    {
      int i = 0;
      char ch;
      printf ("start loading your rom \n");

      while (!feof (fptr))
        {

          ch = fgetc (fptr);
          memory[i + 512] = ch;
          printf ("%04x\n", memory[i + 512]);
          i++;
        }
      printf ("a delicacy/n");
    }
  fclose (fptr);
}
