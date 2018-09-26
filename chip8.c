#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"

static const uint8_t sprites[] = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
	0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
	0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
	0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
	0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
	0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
	0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
	0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
	0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
	0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
	0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
	0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
	0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
	0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
	0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
	0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

void chip8_init(struct chip8 *c, char *romfile)
{
	FILE *f;
	int n;
	
	/* Load ROM into memory. */
	fprintf(stderr, "loading ROM file: %s...\n", romfile);
	
	f = fopen(romfile, "r");
	if (!f)
	{
		fprintf(stderr, "unable to open ROM file\n");
		exit(EXIT_FAILURE);
	}
	
	n = fread(&c->memory[ROM_START], sizeof(uint8_t), MAX_ROM_SIZE, f);

	fprintf(stderr, "read %d bytes into memory\n", n);

	/* Set PC and SP initial values. */
	c->sp = 0;
	c->pc = ROM_START;

	/* Reset timers. */
	c->t_d = 0;
	c->t_s = 0;

	/* Init key values. */
	memset(c->keyboard, 0, 0xF);

	/* Clear screen. */
	memset(c->screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

	/* Copy sprite data into memory. */
	printf("%d\n", sprites[0]);
}

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

	return EXIT_SUCCESS;
}