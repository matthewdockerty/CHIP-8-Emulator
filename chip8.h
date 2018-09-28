#ifndef CHIP8_H_
#define CHIP8_H_

#include <stdint.h>
#include <stdbool.h>

#define ROM_START 0x200
#define MAX_ROM_SIZE 0xE00

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct chip8
{
	/* memory */
	uint8_t memory[4096];

	/* registers */
	uint8_t v[0xF];
	uint16_t I;
	uint8_t t_d, t_s; /* delay and sound timers */

	/* program counter and stack pointer */
	uint16_t pc;
	uint8_t sp;

	/* stack */
	uint16_t stack[16];
	
	/* 16 key hexadecimal keypad:
	 * 1 2 3 C
	 * 4 5 6 D
	 * 7 8 9 E
	 * A 0 B F
	 */
	bool keyboard[0xF];
	uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];

	uint16_t opcode;

	bool wait_key;
};

extern void chip8_init(struct chip8 *c, char *romfile);
extern void chip8_cycle(struct chip8 *c);

#endif
