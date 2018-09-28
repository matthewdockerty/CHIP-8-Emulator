#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char **argv)
{
	struct chip8 c;
	SDL_Event event;
	bool quit;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <rom file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	chip8_init(&c, argv[1]);
	display_init();

	while (!quit)
	{
		while(SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					c.wait_key = false;
					switch (event.key.keysym.sym)
					{
						case SDLK_1:
							c.keyboard[0] = true;
							break;
						case SDLK_2:
							c.keyboard[1] = true;
							break;
						case SDLK_3:
							c.keyboard[2] = true;
							break;
						case SDLK_4:
							c.keyboard[3] = true;
							break;
						case SDLK_q:
							c.keyboard[4] = true;
							break;
						case SDLK_w:
							c.keyboard[5] = true;
							break;
						case SDLK_e:
							c.keyboard[6] = true;
							break;
						case SDLK_r:
							c.keyboard[7] = true;
							break;
						case SDLK_a:
							c.keyboard[8] = true;
							break;
						case SDLK_s:
							c.keyboard[9] = true;
							break;
						case SDLK_d:
							c.keyboard[10] = true;
							break;
						case SDLK_f:
							c.keyboard[11] = true;
							break;
						case SDLK_z:
							c.keyboard[12] = true;
							break;
						case SDLK_x:
							c.keyboard[13] = true;
							break;
						case SDLK_c:
							c.keyboard[14] = true;
							break;
						case SDLK_v:
							c.keyboard[15] = true;
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

		if (!c.wait_key)
			chip8_cycle(&c);

		display_update(c.screen);
	}

	display_close();

	return EXIT_SUCCESS;	
}

