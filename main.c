#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char **argv)
{
	struct chip8 c;
	SDL_Event event;
	bool quit = false;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <rom file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	chip8_init(&c, argv[1]);
	display_init();

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				c.wait_key = false;
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					c.keyboard[0x1] = true;
					c.v[c.wait_reg] = 0x1;
					break;
				case SDLK_2:
					c.keyboard[0x2] = true;
					c.v[c.wait_reg] = 0x2;
					break;
				case SDLK_3:
					c.keyboard[0x3] = true;
					c.v[c.wait_reg] = 0x3;
					break;
				case SDLK_4:
					c.keyboard[0xC] = true;
					c.v[c.wait_reg] = 0xC;
					break;
				case SDLK_q:
					c.keyboard[0x4] = true;
					c.v[c.wait_reg] = 0x4;
					break;
				case SDLK_w:
					c.keyboard[0x5] = true;
					c.v[c.wait_reg] = 0x5;
					break;
				case SDLK_e:
					c.keyboard[0x6] = true;
					c.v[c.wait_reg] = 0x6;
					break;
				case SDLK_r:
					c.keyboard[0xD] = true;
					c.v[c.wait_reg] = 0xD;
					break;
				case SDLK_a:
					c.keyboard[0x7] = true;
					c.v[c.wait_reg] = 0x7;
					break;
				case SDLK_s:
					c.keyboard[0x8] = true;
					c.v[c.wait_reg] = 0x8;
					break;
				case SDLK_d:
					c.keyboard[0x9] = true;
					c.v[c.wait_reg] = 0x9;
					break;
				case SDLK_f:
					c.keyboard[0xE] = true;
					c.v[c.wait_reg] = 0xE;
					break;
				case SDLK_z:
					c.keyboard[0xA] = true;
					c.v[c.wait_reg] = 0xA;
					break;
				case SDLK_x:
					c.keyboard[0x0] = true;
					c.v[c.wait_reg] = 0x0;
					break;
				case SDLK_c:
					c.keyboard[0xB] = true;
					c.v[c.wait_reg] = 0xB;
					break;
				case SDLK_v:
					c.keyboard[0xF] = true;
					c.v[c.wait_reg] = 0xF;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					c.keyboard[0x1] = false;
					break;
				case SDLK_2:
					c.keyboard[0x2] = false;
					break;
				case SDLK_3:
					c.keyboard[0x3] = false;
					break;
				case SDLK_4:
					c.keyboard[0xC] = false;
					break;
				case SDLK_q:
					c.keyboard[0x4] = false;
					break;
				case SDLK_w:
					c.keyboard[0x5] = false;
					break;
				case SDLK_e:
					c.keyboard[0x6] = false;
					break;
				case SDLK_r:
					c.keyboard[0xD] = false;
					break;
				case SDLK_a:
					c.keyboard[0x7] = false;
					break;
				case SDLK_s:
					c.keyboard[0x8] = false;
					break;
				case SDLK_d:
					c.keyboard[0x9] = false;
					break;
				case SDLK_f:
					c.keyboard[0xE] = false;
					break;
				case SDLK_z:
					c.keyboard[0xA] = false;
					break;
				case SDLK_x:
					c.keyboard[0x0] = false;
					break;
				case SDLK_c:
					c.keyboard[0xB] = false;
					break;
				case SDLK_v:
					c.keyboard[0xF] = false;
					break;
				}

				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	
		chip8_cycle(&c);
		display_update(c.screen);
	}

	display_close();

	return EXIT_SUCCESS;	
}

