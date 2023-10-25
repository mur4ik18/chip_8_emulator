#include "main.h"
#include "stdio.h"

void
cycle_emulator (void)
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
          //system ("clear");
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
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
          break;
          // vx |= vy
        case 0x0001:
          V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
          break;
          // vx &= vy
        case 0x0002:
          V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
          break;
          // vx ^= Vy
        case 0x0003:
          V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
          break;
          // ADD VY to VX
          // 0x8xy4
        case 0x0004:
          if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
          break;
          // vx -= Vy
        case 0x0005:
          if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
            V[0xF] = 0;
          else
            V[0xF] = 1;
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
          break;
          // vx >>= 1
        case 0x0006:
          V[(opcode & 0x0F00) >> 8] >>= V[(opcode & 0x00F0) >> 4];
          break;
        // vx = Vy - Vx
        case 0x0007:
          if (V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8])
            V[0xF] = 0;
          else
            V[0xF] = 1;
          V[(opcode & 0x0F00) >> 8]
              = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
          break;
          // vx <<= Vy
        case 0x000E:
          V[(opcode & 0x0F00) >> 8] <<= V[(opcode & 0x00F0) >> 4];
          break;
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

  // Update timers
  if (delay_timer > 0)
    --delay_timer;

  if (sound_timer > 0)
    {
      if (sound_timer == 1)
        printf ("BEEEEEEEEEEEEEP!\n");
      --sound_timer;
    }
}
