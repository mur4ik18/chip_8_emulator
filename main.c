#include <stdio.h>

 // current opcode
unsigned short opcode;
  // 4k memory
unsigned char memory[4096];
  // CPU registers
unsigned char V[16];

  // index register I
  // program counter pc
  // 0x000 to 0xFFF
unsigned short I;
unsigned short pc = 0;

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

void cycle_emulator();

int main(void)
{
  memory[4] = 0xAC;
  memory[5] = 0xF0;

  // Emulation loop
  for (;;)
  {
    cycle_emulator();
    if (pc >= 20) break;
  }
  printf("pc = %d\n", pc);
  
  
  return 0;
}

void cycle_emulator()
{
  // fetch => decode => execute opcode
  // fetch opcode
  //opcode = 0x255;
  //printf("%d %d\n", memory[pc], memory[pc+1]);
  opcode =  memory[pc] <<8 | memory[pc+1];
  //printf("opcode = %04X \n", memory[pc] <<8 | memory[pc+1]);
  
  
  switch (opcode & 0xF000) {
    // SYS addr 
  case 0x0000:
    break;
    // JP addr
  case 0x1000:
    pc = opcode & 0x0FFF;
    break;
    // Call addr
  case 0x2000:
    break;
  case 0x3000:
    break;
  case 0x4000:
    break;
  case 0x5000:
    break;
  case 0x6000:
    break;
  case 0x7000:
    break;
  case 0x8000:
    break;
  case 0x9000:
    break;
  case 0xA000:
    I = opcode & 0x0FFF;
    printf("I = %04X\n", I);
    break;
  case 0xB000:
    break;
  case 0xC000:
    break;
  case 0xD000:
    break;
  case 0xE000:
    break;
  case 0xF000:
    break;
    
  default:
    printf("Uknown opcode %04X \n", opcode);
  }
  pc += 2;
  // Update timers
}
