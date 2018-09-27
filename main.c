#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char **argv)
{
	struct chip8 c;


	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <rom file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	chip8_init(&c, argv[1]);

	display_init();

	display_update(c.screen);

	return EXIT_SUCCESS;	
}

