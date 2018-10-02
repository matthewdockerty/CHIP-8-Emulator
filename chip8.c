#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _BSD_SOURCE

static void op_unimplemented(struct chip8 *c)
{
	fprintf(stderr, "unimplemented opcode: %x\n", c->opcode);	
	exit(EXIT_FAILURE);
}

static void op_0(struct chip8 *c)
{
	/* CLS : Clear the display */
	if (c->opcode == 0x00E0)
	{
		memset(c->screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(c->screen[0]));
	}
	/* RET : Return from subroutine */
	else if (c->opcode == 0x00EE)
	{
		c->pc = c->stack[--(c->sp)];
	}
	/* SYS addr : jump to a machine code routine */
	else
	{
		op_unimplemented(c);
	}
}

static void op_1(struct chip8 *c)
{
	/* 1nnn - JP addr : Jump to location nnn */
	uint16_t addr = c->opcode & 0xFFF;
	c->pc = addr;
}

static void op_2(struct chip8 *c)
{
	/* 2nnn - CALL addr : Call subroutine at nnn */
	uint16_t addr = c->opcode & 0xFFF;
	c->stack[c->sp++] = c->pc;
	c->pc = addr;
}

static void op_3(struct chip8 *c)
{
	/* 3xkk - SE Vx, byte : Skip next instruction if Vx = kk */
	int reg = (c->opcode >> 8) & 0xF;
	uint8_t value = c->opcode & 0xFF;

	if (c->v[reg] == value)
	{
		c->pc += 2;
	}
}

static void op_4(struct chip8 *c)
{
	/* 4xkk - SNE Vx, byte : Skip next instruction if Vx != kk */
	int reg = (c->opcode >> 8) & 0xF;
	uint8_t value = c->opcode &0xFF;

	if (c->v[reg] != value)
	{
		c->pc += 2;
	}	
}

static void op_6(struct chip8 *c)
{
	/* LD Vx, byte */

	int reg = (c->opcode >> 8) & 0xF;
	uint8_t value = c->opcode & 0xFF;

	c->v[reg] = value;
}

static void op_7(struct chip8 *c)
{
	/* 7xkk - ADD Vx, byte : Set Vx = Vx + kk */
	int reg = (c->opcode >> 8) & 0xF;
	uint8_t value = c->opcode & 0xFF;
	c->v[reg] += value;
}

static void op_8(struct chip8 *c)
{
	int x = (c->opcode >> 8) & 0xF;
	int y = (c->opcode >> 4) & 0xF;

	/* 8xy2 - AND Vx, Vy : Set Vx = Vx AND Vy */
	if ((c->opcode & 0xF) == 0x2)
	{	
		c->v[x] &= c->v[y];
	}
	/* 8xy4 - ADD Vx, Vy : Set Vx = Vx + Vy, set VF = carry */
	else if ((c->opcode & 0xF) == 0x4)
	{
		c->v[0xF] = c->v[y] > c->v[x] ? 1 : 0;
		c->v[x] += c->v[y];
	}
	/* 8xy0 - LD Vx, Vy : Set Vx = Vy */
	else if ((c->opcode & 0xF) == 0x0)
	{
		c->v[x] = c->v[y];
	}
	/* 8xy6 - SHR Vx {, Vy} : Set Vx = Vx SHR 1 */
	else if ((c->opcode & 0xF) == 0x6)
	{
		c->v[0xF] = c->v[x] & 1;
		c->v[x] = c->v[x] >> 1;
	}
	/* 8xy5 - SUB Vx, Vy : Set Vx = Vx - Vy, set VF = NOT borrow */
	else if ((c->opcode &0xF) == 0x5)
	{
		c->v[0xF] = c->v[x] > c->v[y] ? 1 : 0;
		c->v[x] -= c->v[y];
	}
	/* 8xy3 - XOR Vx, Vy : Set Vx = Vx XOR Vy */
	else if ((c->opcode &0xF) == 0x3)
	{
		c->v[x] ^= c->v[y];
	}
	/* 8xyE - SHL Vx {, Vy} : Set Vx = Vx SHL 1 */
	else if ((c->opcode & 0xF) == 0xE)
	{
		c->v[0xF] = c->v[x] >> 7;
		c->v[x] <<= 1;
	}
	else
	{
		op_unimplemented(c);
	}
}

static void op_a(struct chip8 *c)
{
	/* Annn - LD I, addr : Set I = nnn */
	uint16_t value = c->opcode & 0xFFF;
	c->I = value;
}

static bool set_pixel(int x, int y, uint32_t *screen)
{
	int index;
	uint32_t pixel;

	x %= SCREEN_WIDTH;
	y %= SCREEN_HEIGHT;

	index = (SCREEN_WIDTH * y) + x;
	pixel = screen[index];

	screen[index] = pixel ^ 0xFFFFFF;

	return (pixel == 0xFFFFFF && screen[index] == 0x000000) ? true : false;
}	

static void op_c(struct chip8 *c)
{
	/* Cxkk - RND Vx, byte : Set Vx = random byte AND kk */
	int reg = (c->opcode >> 8) & 0xF;
	uint8_t value = c->opcode & 0xFF;

	c->v[reg] = value & (rand() % 255);
}

static void op_d(struct chip8 *c)
{
	/* Dxyn - DRW Vx, Vy, nibble : Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. */
	int x = c->v[(c->opcode >> 8) & 0xF];
	int y = c->v[(c->opcode >> 4) & 0xF];
	int n = c->opcode & 0xF;

	int i;
	for (i = 0; i < n; i++)
	{
		uint8_t row = c->memory[c->I + i];
		int j;

		for (j = 0; j < 8; j++)
		{
			if (((row >> j) & 0x1) == 1)
			{
				if (set_pixel(x + (7 - j), y + i, c->screen) == true)
				{
					c->v[0xF] = 1;
				}
			}
		}
	}
}

static void op_e(struct chip8 *c)
{
	int x = c->opcode >> 8 & 0xF;

	/* Ex9E - SKP Vx : Skip next instruction if key with the value of Vx is pressed */
	if ((c->opcode & 0xFF) == 0x9E)
	{
		if (c->keyboard[c->v[x]] == true)
			c->pc += 2;
	}
	/* ExA1 - SKNP Vx : Skip next instruction if key with the value of Vx is not pressed */
	else if ((c->opcode & 0xFF) == 0xA1)
	{
		if (c->keyboard[c->v[x]] == false)
			c->pc += 2;
	}
}

static void op_f(struct chip8 *c)
{
	int x = c->opcode >> 8 & 0xF;

	/* Fx33 - LD B, Vx : Store BCD representation of Vx in memory locations I, I+1, and I+2 */
	if ((c->opcode & 0xFF) == 0x33)
	{
		c->memory[c->I] = c->v[x] / 100;
		c->memory[c->I + 1] = (c->v[x] / 10) % 10;
		c->memory[c->I + 2] = (c->v[x] % 100) % 10;
	}
	/* Fx65 - LD Vx, [I] : Read registers V0 through Vx from memory starting at location I */
	else if ((c->opcode & 0xFF) == 0x65)
	{
		int reg;
		for (reg = 0; reg <= x; reg++)
		{
			c->v[reg] = c->memory[c->I + reg];	
		}
	}
	/* Fx29 - LD F, Vx : Set I = location of sprite for digit Vx */
	else if ((c->opcode & 0xFF) == 0x29)
	{
		c->I = 5 * c->v[x]; 
	}
	/* Fx15 - LD DT, Vx : Set delay timer = Vx */ 
	else if ((c->opcode & 0xFF) == 0x15)
	{
		c->t_d = c->v[x];
	}
	/* Fx07 - LD Vx, DT : Set Vx = delay timer value */
	else if ((c->opcode & 0xFF) == 0x07)
	{
		c->v[x] = c->t_d;
	}
	/* Fx1E - Add I, Vx : Set I = I + Vx */
	else if ((c->opcode & 0xFF) == 0x1E)
	{
		c->v[0xF] = c->I + c->v[x] > 0xFFF ? 1 : 0;
		c->I += c->v[x];
	}
	/* Fx18 - LD ST, Vx : Set sound timer = Vx */
	else if ((c->opcode & 0xFF) == 0x18)
	{
		c->t_s = c->v[x];
	}
	/* Fx0A - LD Vx, K : Wait for a key press, store the value of the key in Vx. */
	else if ((c->opcode & 0xFF) == 0x0A)
	{
		/* Is stored in Vx when key is pressed. */
		c->wait_reg = x;
		c->wait_key = true;
	}
	/* Fx55 - LD [I], Fx : Store registers V0 through Vx in memory starting at location I */
	else if ((c->opcode & 0xFF) == 0x55)
	{
		int i;
		for (i = 0; i <= x; i++)
		{
			c->memory[(int)(i + c->I)] = c->v[i];
		}
	}
	else
	{
		op_unimplemented(c);
	}
}

static void (*ops[0x10])(struct chip8 *) = 
{
	op_0,
	op_1,
	op_2,
	op_3,
	op_4,
	op_unimplemented,
	op_6,
	op_7,
	op_8,
	op_unimplemented,
	op_a,
	op_unimplemented,
	op_c,
	op_d,
	op_e,
	op_f
};

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

	f = fopen(romfile, "rb");
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

	/* Clear stack */
	memset(c->stack, 0, 16 * sizeof(c->stack[0]));

	/* Clear registers */
	memset(c->v, 0, 16 * sizeof(c->v[0]));


	/* Reset timers. */
	c->t_d = 0;
	c->t_s = 0;

	/* Init key values. */
	memset(c->keyboard, false, 0xF);

	/* Clear screen. */
	memset(c->screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(c->screen[0]));

	/* Copy sprite data into memory. */
	memcpy(c->memory, sprites, 0xF * 5);

	c->wait_key = false;
}

#include <Windows.h>

void chip8_cycle(struct chip8 *c)
{
	c->opcode = ((uint16_t)(c->memory[c->pc] << 8)) | c->memory[c->pc + 1];
	c->pc+=2;

	/*printf("Opcode: %x\n", c->opcode);*/
	ops[(c->opcode & 0xF000) >> 12](c);

	if (c->t_d > 0)
	{
		c->t_d -= 1;
	}
	if (c->t_s > 0)
	{
		c->t_s -= 1;

		printf("SOUND\n");
	}
}
