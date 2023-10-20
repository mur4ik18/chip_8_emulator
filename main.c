#include <stdio.h>
#include<stdlib.h>

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
  // memoir writing
  memory[0] = 0x61;
  memory[1] = 0x05;

  memory[2] = 0x62;
  memory[3] = 0x02;
  
  memory[4] = 0x81;
  memory[5] = 0x24;
  
  memory[6] = 0x00;
  memory[7] = 0xE0;

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
    switch (opcode & 0x00FF) {
      // RET 
    case 0x00EE:
      pc = stack[sp--];
      printf("return from subrutine\n");
      break;
      // clear display
    case 0x00E0:
      printf("clear_display()\n");
      system("clear");
      break;
    }
    break;
    // JP addr
  case 0x1000:
    pc = opcode & 0x0FFF;
    break;
    // Call addr call subrutine
    // The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
  case 0x2000:
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
    break;
    // SE Vx byte
  case 0x3000:
    break;
    // SNE
  case 0x4000:
    break;
    // SE Vx Vy
  case 0x5000:
    break;
    // SET NN in Vx
    // 0x6XNN
  case 0x6000:
    V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    printf("%04X <- %d\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8]);
    break;
    //
  case 0x7000:
    break;
    // 
  case 0x8000:
    switch (opcode & 0x000F) {
      // ADD VY to VX
      // 0x8xy4
    case 0x0004: 
      printf("Add VY to VX\n");
      printf("Add %d to %d\n", V[(opcode & 0x00F0) >> 4],  V[(opcode & 0x0F00) >> 8]);
      V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
      printf("%d\n",V[(opcode & 0x0F00) >> 8]);
      break;
    }
    break;
    //
  case 0x9000:
    break;
    //
  case 0xA000:
    I = opcode & 0x0FFF;
    printf("I = %04X\n", I);
    break;
    //
  case 0xB000:
    break;
    //
  case 0xC000:
    break;
    //
  case 0xD000:
    break;
    //
  case 0xE000:
    break;
    //
  case 0xF000:
    break;
    
  default:
    printf("Uknown opcode %04X \n", opcode);
  }
  pc += 2;
  // Update timers
}
