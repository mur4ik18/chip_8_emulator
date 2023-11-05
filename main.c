#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  Context ctx;
  if (argc == 2)
    {
      fptr = fopen (argv[1], "rb");
      read_rom (&ctx);
    }
  // graphique setup
  // memset(&app, 0, sizeof(App));
  // graphique_setup ();

  init (&ctx);

  // Emulation loop
  for (;;)
    {
      cycle_emulator (&ctx);

      // prepareScene ();

      // doInput ();
      // render_graphic();
      // presentScene ();
      printf ("pc = %d\n", ctx.pc);
      // SDL_Delay (16);
    }

  return 0;
}

void
init (Context *ctx)
{
  ctx->pc = 0x200;
  opcode = 0;
  ctx->I = 0;
  ctx->sp = 0;
  for (int i = 0; i < 80; ++i)
    ctx->memory[i] = chip8_fontset[i];
}

void
read_rom (Context *ctx)
{
  if (fptr != NULL)
    {
      int i = 0;
      char ch;
      printf ("start loading your rom \n");

      while (!feof (fptr))
        {

          ch = fgetc (fptr);
          ctx->memory[i + 512] = ch;
          printf ("%04x\n", ctx->memory[i + 512]);
          i++;
        }
      printf ("a delicacy/n");
    }
  fclose (fptr);
}

void
cycle_emulator (Context *ctx)
{
  // fetch => decode => execute opcode
  // fetch opcode
  // opcode = 0x255;
  // printf("%d %d\n", memory[pc], memory[pc+1]);
  opcode = ctx->memory[ctx->pc] << 8 | ctx->memory[ctx->pc + 1];
  // printf("opcode = %04X \n", memory[pc] <<8 | memory[pc+1]);

  switch (opcode & 0xF000)
    {
      // SYS addr
    case 0x0000:
      switch (opcode & 0x00FF)
        {
          // RET
        case 0x00EE:
          ctx->pc = ctx->stack[ctx->sp--];
          printf ("return from subrutine\n");
          break;
          // clear display
        case 0x00E0:
          printf ("clear_display()\n");
          // system ("clear");
          break;
        }
      break;
      // JP addr
    case 0x1000:
      ctx->pc = opcode & 0x0FFF - 2;
      break;
      // Call addr call subrutine
      // The interpreter increments the stack pointer, then puts the current PC
      // on the top of the stack. The PC is then set to nnn.
    case 0x2000:
      ctx->stack[ctx->sp] = ctx->pc;
      ctx->sp++;
      ctx->pc = opcode & 0x0FFF - 2;
      break;
      // SE Vx byte
      // 3Xkk
    case 0x3000:
      if (ctx->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        ctx->pc += 2;
      break;
      // SNE
    case 0x4000:
      if (ctx->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        ctx->pc += 2;
      break;
      // SE Vx Vy
    case 0x5000:
      if (ctx->V[(opcode & 0x0F00) >> 8] == ctx->V[(opcode & 0x00FF) >> 4])
        ctx->pc += 2;
      break;
      // SET NN in Vx
      // 0x6XNN
    case 0x6000:
      ctx->V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      printf ("%04X <- %d\n", (opcode & 0x0F00) >> 8,
              ctx->V[(opcode & 0x0F00) >> 8]);
      break;
      // ADD Vx += KK
      // 7XKK
    case 0x7000:
      ctx->V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
      break;
      //
    case 0x8000:
      switch (opcode & 0x000F)
        {
          // vx = vy
        case 0x0000:
          ctx->V[(opcode & 0x0F00) >> 8] = ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx |= vy
        case 0x0001:
          ctx->V[(opcode & 0x0F00) >> 8] |= ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx &= vy
        case 0x0002:
          ctx->V[(opcode & 0x0F00) >> 8] &= ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx ^= Vy
        case 0x0003:
          ctx->V[(opcode & 0x0F00) >> 8] ^= ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // ADD VY to VX
          // 0x8xy4
        case 0x0004:
          ctx->V[0xF] = (ctx->V[(opcode & 0x00F0) >> 4]
                         > ctx->V[(opcode & 0x0F00) >> 8]);
          ctx->V[(opcode & 0x0F00) >> 8] += ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx -= Vy
        case 0x0005:
          if (ctx->V[(opcode & 0x0F00) >> 8] > ctx->V[(opcode & 0x00F0) >> 4])
            ctx->V[0xF] = 0;
          else
            ctx->V[0xF] = 1;
          ctx->V[(opcode & 0x0F00) >> 8] -= ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx >>= 1
        case 0x0006:
          ctx->V[(opcode & 0x0F00) >> 8] >>= ctx->V[(opcode & 0x00F0) >> 4];
          break;
        // vx = Vy - Vx
        case 0x0007:
          if (ctx->V[(opcode & 0x00F0) >> 4] < ctx->V[(opcode & 0x0F00) >> 8])
            ctx->V[0xF] = 0;
          else
            ctx->V[0xF] = 1;
          ctx->V[(opcode & 0x0F00) >> 8]
              = ctx->V[(opcode & 0x00F0) >> 4] - ctx->V[(opcode & 0x0F00) >> 8];
          break;
          // vx <<= Vy
        case 0x000E:
          ctx->V[(opcode & 0x0F00) >> 8] <<= ctx->V[(opcode & 0x00F0) >> 4];
          break;
        }
      break;
      // Cond if (Vx != Vy)
    case 0x9000:
      break;
      // Mem
    case 0xA000:
      ctx->I = opcode & 0x0FFF;
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
  ctx->pc += 2;
  // Update timers
  if (ctx->delay_timer > 0)
    --ctx->delay_timer;

  if (ctx->sound_timer > 0)
    {
      if (ctx->sound_timer == 1)
        printf ("BEEEEEEEEEEEEEP!\n");
      --ctx->sound_timer;
    }
}
