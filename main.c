#include <stdio.h>

void cycle_emulator(unsigned short* opcode, unsigned short* pc);

int main(void)
{
  // current opcode
  unsigned short* opcode;
  // 4k memory
  unsigned char* memory[4096];
  // CPU registers
  unsigned char* V[16];

  // index register I
  // program counter pc
  // 0x000 to 0xFFF
  unsigned short* I;
  unsigned short pc = 0;

  // Memory addresses
  // 0x000 - 0x1FF - interpreter (contains font set in emu)
  // 0x050 - 0x0A0 - for the built in 4x5 pixel font set (0-F)
  // 0x200 - 0xFFF - program ROM and work RAM

  // 2048 pixels black 1 and white 0
  unsigned char* gfx[64 * 32];

  // interupts and hardware registers
  unsigned char* delay_timer;
  unsigned char* sound_timer;

  unsigned short* stack[16];
  // stack pointer
  unsigned short* sp;

  unsigned char* key[16];

  // Emulation loop
  for (;;)
  {
    cycle_emulator(opcode, &pc);
    break;
  }
  printf("pc = %d\n", pc);
  return 0;
}

void cycle_emulator(unsigned short* opcode, unsigned short* pc)
{
  // fetch => decode => execute opcode
  // fetch opcode
  //opcode = 0x255;
  *pc += 2;
  // Update timers
}
