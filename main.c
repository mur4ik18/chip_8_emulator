#include <stdio.h>
#include <stdlib.h>

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

void cycle_emulator ();

int
main (void)
{
  // memoir writing
  memory[0] = 0x61;
  memory[1] = 0x05;

  memory[2] = 0x62;
  memory[3] = 0x02;

  memory[4] = 0x81;
  memory[5] = 0x24;

  // Emulation loop
  for (;;)
    {
      cycle_emulator ();
      if (pc >= 20)
        break;
    }
  printf ("pc = %d\n", pc);

  return 0;
}

void
cycle_emulator ()
{
  // fetch => decode => execute opcode
  // fetch opcode
  // opcode = 0x255;
  // printf("%d %d\n", memory[pc], memory[pc+1]);
  opcode = memory[pc] << 8 | memory[pc + 1];
  // printf("opcode = %04X \n", memory[pc] <<8 | memory[pc+1]);

  switch (opcode & 0xF000)
    {
      // SYS addr
    case 0x0000:
      switch (opcode & 0x00FF)
        {
          // RET
        case 0x00EE:
          pc = stack[sp--];
          printf ("return from subrutine\n");
          break;
          // clear display
        case 0x00E0:
          printf ("clear_display()\n");
          system ("clear");
          break;
        }
      break;
      // JP addr
    case 0x1000:
      pc = opcode & 0x0FFF;
      break;
      // Call addr call subrutine
      // The interpreter increments the stack pointer, then puts the current PC
      // on the top of the stack. The PC is then set to nnn.
    case 0x2000:
      stack[sp] = pc;
      sp++;
      pc = opcode & 0x0FFF;
      break;
      // SE Vx byte
      // 3Xkk
    case 0x3000:
      if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        pc += 2;
      break;
      // SNE
    case 0x4000:
      if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        pc += 2;
      break;
      // SE Vx Vy
    case 0x5000:
      if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00FF) >> 4])
        pc += 2;
      break;
      // SET NN in Vx
      // 0x6XNN
    case 0x6000:
      V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      printf ("%04X <- %d\n", (opcode & 0x0F00) >> 8,
              V[(opcode & 0x0F00) >> 8]);
      break;
      // ADD Vx += KK
      // 7XKK
    case 0x7000:
      V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
      break;
      //
    case 0x8000:
      switch (opcode & 0x000F)
        {
          // vx = vy
        case 0x0000:
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] break;
          // vx |= vy
        case 0x0001:
          V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4] break;
          // vx &= vy
        case 0x0002:
          V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4] break;
          // vx ^= Vy
        case 0x0003:
          V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4] break;
          // ADD VY to VX
          // 0x8xy4
        case 0x0004:
          if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
            V[0xF] = 1;
          else
            v[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
          break;
          // vx -= Vy
        case 0x0005:
          if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
            V[0xF] = 0;
          else
            v[0xF] = 1;
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
          break;
          // vx >>= 1
        case 0x0006:
          V[(opcode & 0x0F00) >> 8] >>= V[(opcode & 0x00F0) >> 4] break;
        // vx = Vy - Vx
        case 0x0007:
          if (V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8])
            V[0xF] = 0;
          else
            V[0xF] = 1;
          V[(opcode & 0x0F00) >> 8]
              = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8] break;
          // vx <<= Vy
        case 0x000E:
          V[(opcode & 0x0F00) >> 8] <<= V[(opcode & 0x00F0) >> 4] break;
        }
      break;
      // Cond if (Vx != Vy)
    case 0x9000:
      break;
      // Mem
    case 0xA000:
      I = opcode & 0x0FFF;
      break;
      // Folow
    case 0xB000:
      break;
      // Rand
    case 0xC000:
      break;
      // display
    case 0xD000:
      break;
      // KeyOp
    case 0xE000:
      break;
      //
    case 0xF000:
      switch (opcode & 0x00FF)
        {
        case 0x0007:
          break;
        case 0x000A:
          break;
        case 0x0015:
          break;
        case 0x0018:
          break;
        case 0x001E:
          break;
        case 0x0029:
          break;
        case 0x0033:
          break;
        case 0x0055:
          break;
        case 0x0065:
          break;
        }

    default:
      printf ("Uknown opcode %04X \n", opcode);
    }
  pc += 2;
  // Update timers
}
