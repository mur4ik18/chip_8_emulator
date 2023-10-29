
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int
main (void)
{
  // graphique setup
  graphique_setup ();

  init ();

  // Emulation loop
  for (;;)
    {
      cycle_emulator ();
      if (pc >= 20)
        break;
    }
  printf ("pc = %d\n", pc);

  graphique_break ();
  return 0;
}
void
init (void)
{
  // memoir writing
  memory[0] = 0x61;
  memory[1] = 0x05;

  memory[2] = 0x62;
  memory[3] = 0x02;

  memory[4] = 0x81;
  memory[5] = 0x24;
}
