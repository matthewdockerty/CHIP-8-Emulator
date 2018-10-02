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
	unsigned int last_time = 0, current_time;

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
					c.keyboard[0] = true;
					c.v[c.wait_reg] = 0;
					break;
				case SDLK_2:
					c.keyboard[1] = true;
					c.v[c.wait_reg] = 1;
					break;
				case SDLK_3:
					c.keyboard[2] = true;
					c.v[c.wait_reg] = 2;
					break;
				case SDLK_4:
					c.keyboard[3] = true;
					c.v[c.wait_reg] = 3;
					break;
				case SDLK_q:
					c.keyboard[4] = true;
					c.v[c.wait_reg] = 4;
					break;
				case SDLK_w:
					c.keyboard[5] = true;
					c.v[c.wait_reg] = 5;
					break;
				case SDLK_e:
					c.keyboard[6] = true;
					c.v[c.wait_reg] = 6;
					break;
				case SDLK_r:
					c.keyboard[7] = true;
					c.v[c.wait_reg] = 7;
					break;
				case SDLK_a:
					c.keyboard[8] = true;
					c.v[c.wait_reg] = 8;
					break;
				case SDLK_s:
					c.keyboard[9] = true;
					c.v[c.wait_reg] = 9;
					break;
				case SDLK_d:
					c.keyboard[10] = true;
					c.v[c.wait_reg] = 10;
					break;
				case SDLK_f:
					c.keyboard[11] = true;
					c.v[c.wait_reg] = 11;
					break;
				case SDLK_z:
					c.keyboard[12] = true;
					c.v[c.wait_reg] = 12;
					break;
				case SDLK_x:
					c.keyboard[13] = true;
					c.v[c.wait_reg] = 13;
					break;
				case SDLK_c:
					c.keyboard[14] = true;
					c.v[c.wait_reg] = 14;
					break;
				case SDLK_v:
					c.keyboard[15] = true;
					c.v[c.wait_reg] = 15;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					c.keyboard[0] = false;
					break;
				case SDLK_2:
					c.keyboard[1] = false;
					break;
				case SDLK_3:
					c.keyboard[2] = false;
					break;
				case SDLK_4:
					c.keyboard[3] = false;
					break;
				case SDLK_q:
					c.keyboard[4] = false;
					break;
				case SDLK_w:
					c.keyboard[5] = false;
					break;
				case SDLK_e:
					c.keyboard[6] = false;
					break;
				case SDLK_r:
					c.keyboard[7] = false;
					break;
				case SDLK_a:
					c.keyboard[8] = false;
					break;
				case SDLK_s:
					c.keyboard[9] = false;
					break;
				case SDLK_d:
					c.keyboard[10] = false;
					break;
				case SDLK_f:
					c.keyboard[11] = false;
					break;
				case SDLK_z:
					c.keyboard[12] = false;
					break;
				case SDLK_x:
					c.keyboard[13] = false;
					break;
				case SDLK_c:
					c.keyboard[14] = false;
					break;
				case SDLK_v:
					c.keyboard[15] = false;
					break;
				}

				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	
		current_time = SDL_GetTicks();
		if (!c.wait_key && current_time > last_time + 1)
		{
			chip8_cycle(&c);
			last_time = current_time;
		}

		display_update(c.screen);
	}

	display_close();

	return EXIT_SUCCESS;	
}

