#include "main.h"
#include <stdbool.h>
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
  memset (&app, 0, sizeof (App));
  graphique_setup ();

  init (&ctx);

  // Emulation loop
  for (;;)
    {
      prepareScene ();
      cycle_emulator (&ctx);

      doInput ();
      render_graphic (&ctx);

      // presentScene ();
      printf ("pc = %d\n", ctx.pc);
      SDL_Delay (100);
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
  for (int i = 0; i < (64 * 32); i++)
    ctx->gfx[i] = 0;
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
          i++;
        }
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
  printf ("opcode %04x\n", opcode);
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
      ctx->pc = (opcode & 0x0FFF);
      break;
      // Call addr call subrutine
      // The interpreter increments the stack pointer, then puts the current PC
      // on the top of the stack. The PC is then set to nnn.
    case 0x2000:
      ctx->stack[ctx->sp++] = ctx->pc;
      ctx->pc = (opcode & 0x0FFF);
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
          ctx->V[0xF] = !(ctx->V[(opcode & 0x0F00) >> 8]
                          > ctx->V[(opcode & 0x00F0) >> 4]);
          ctx->V[(opcode & 0x0F00) >> 8] -= ctx->V[(opcode & 0x00F0) >> 4];
          break;
          // vx >>= 1
        case 0x0006:
          ctx->V[0xF] >>= ctx->V[(opcode & 0x0F00) >> 8];
          ctx->V[(opcode & 0x0F00) >> 8] >>= ctx->V[(opcode & 0x00F0) >> 4];
          break;
        // vx = Vy - Vx
        case 0x0007:
          ctx->V[0xF] = !(ctx->V[(opcode & 0x00F0) >> 4]
                          < ctx->V[(opcode & 0x0F00) >> 8]);
          ctx->V[(opcode & 0x0F00) >> 8] = ctx->V[(opcode & 0x00F0) >> 4]
                                           - ctx->V[(opcode & 0x0F00) >> 8];
          break;
          // vx <<= Vy
        case 0x000E:
          ctx->V[0xF] <<= ctx->V[(opcode & 0x0F00) >> 8];
          ctx->V[(opcode & 0x0F00) >> 8] <<= ctx->V[(opcode & 0x00F0) >> 4];
          break;
        }
      break;
      // Cond if (Vx != Vy)
    case 0x9000:
      if (ctx->V[(opcode & 0x0F00) >> 8] != ctx->V[(opcode & 0x00F0) >> 4])
        ctx->pc += 2;
      break;
      // Mem
    case 0xA000:
      ctx->I = (opcode & 0x0FFF);
      break;
      // Folow
    case 0xB000:
      // wtf??
      ctx->pc = ctx->V[0] + (opcode & 0x0FFF);
      break;
      // Rand
    case 0xC000:
      srand (2);
      ctx->V[(opcode & 0x0F00) >> 8] = rand () & (opcode & 0x00FF);
      break;
      // display
    case 0xD000:
      {
        unsigned short x_coord = ctx->V[(opcode & 0x0F00) >> 8] % 64;
        unsigned short y_coord = ctx->V[(opcode & 0x00F0) >> 4] % 32;
        unsigned short height = opcode & 0x000F;
        unsigned short x_orig = x_coord;
        unsigned short sprite;
        ctx->V[0xF] = 0;

        for (uint8_t i = 0; i < height; i++)
          {
            sprite = ctx->memory[ctx->I + i];
            x_coord = x_orig;
            for (int8_t j = 7; j >= 0; j--)
              {
                bool *pixel = &ctx->gfx[(y_coord * 64) + x_coord];
                bool sprite_bit = (sprite & (1 << j));
                if (sprite_bit && *pixel)
                  {
                    ctx->V[0xF] = 1;
                  }

                *pixel ^= sprite_bit;

                if (++x_coord >= 64)
                  break;
              }
            if (++y_coord >= 32)
              break;
          }
        ctx->drawFlag = true;
        break;
      }
      // keyboard input
      // KeyOp
    case 0xE000:

      break;
      //
    case 0xF000:
      switch (opcode & 0x00FF)
        {
          // get_delaytimer()
        case 0x0007:
          ctx->V[(opcode & 0x0F00) >> 8] = ctx->delay_timer;
          break;

          // keyOp
        case 0x000A:
          break;

        case 0x0015:
          ctx->delay_timer = ctx->V[(opcode & 0x0F00) >> 8];
          break;
        case 0x0018:
          ctx->sound_timer = ctx->V[(opcode & 0x0F00) >> 8];
          break;
          // MEM
        case 0x001E:
          ctx->I += ctx->V[(opcode & 0x0F00) >> 8];
          break;
          // MEM sprite_addr
        case 0x0029:
          ctx->I += chip8_fontset[ctx->V[(opcode & 0x0F00) >> 8]];
          break;
        case 0x0033:
          ctx->memory[ctx->I] = ctx->V[(opcode & 0x0F00) >> 8] / 100;
          ctx->memory[ctx->I + 1] = (ctx->V[(opcode & 0x0F00) >> 8] / 10) % 10;
          ctx->memory[ctx->I + 2]
              = (ctx->V[(opcode & 0x0F00) >> 8] % 100) % 10;
          break;

          // MEM
        case 0x0055:
          printf ("55\n");
          break;
          // MEM
        case 0x0065:
          printf ("65\n");
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

void
graphique_setup (void)
{
  int rendererFlags, windowFlags;

  rendererFlags = SDL_RENDERER_ACCELERATED;

  windowFlags = 0;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }

  app.window = SDL_CreateWindow ("Shooter 01", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                 SCREEN_HEIGHT, windowFlags);

  if (!app.window)
    {
      printf ("Failed to open %d x %d window: %s\n", SCREEN_WIDTH,
              SCREEN_HEIGHT, SDL_GetError ());
      exit (1);
    }

  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  app.renderer = SDL_CreateRenderer (app.window, -1, rendererFlags);

  if (!app.renderer)
    {
      printf ("Failed to create renderer: %s\n", SDL_GetError ());
      exit (1);
    }
}

void
doInput (void)
{
  SDL_Event event;

  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_QUIT:
          exit (0);
          break;

        default:
          break;
        }
    }
}

void
prepareScene (void)
{
  SDL_SetRenderDrawColor (app.renderer, 0, 0, 0, 255);
  SDL_RenderClear (app.renderer);
}

void
presentScene (void)
{
  SDL_RenderPresent (app.renderer);
}

void
render_graphic (Context *ctx)
{
  SDL_Rect rect;
  rect.w = 10;
  rect.h = 10;
  int x = 0;
  int y = 0;

  for (int i = 0; i < (64 * 32); i++)
    {

      if ((i != 0) && ((i % 64) == 0))
        {
          y++;
          x = 0;
        }

      rect.x = x * 10;
      rect.y = y * 10;

      if (ctx->gfx[i])
        {

          // SDL_SetRenderDrawColor (app.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
          // SDL_RenderClear (app.renderer);
          SDL_SetRenderDrawColor (app.renderer, 255, 255, 255,
                                  SDL_ALPHA_OPAQUE);
          SDL_RenderFillRect (app.renderer, &rect);
        }
      else
        {
          // SDL_SetRenderDrawColor (app.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
          // SDL_RenderFillRect (app.renderer, &rect);
        }
      x++;
    }
  SDL_RenderPresent (app.renderer);
}
