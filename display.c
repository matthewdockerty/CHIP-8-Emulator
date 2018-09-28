#include "display.h"

#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

void display_init(void)
{
	/* Initialize SDL. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "unable to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Create the window. */
	window = SDL_CreateWindow(DISPLAY_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * DISPLAY_SCALE, SCREEN_HEIGHT * DISPLAY_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		fprintf(stderr, "unable to create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Create the renderer. */
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		fprintf(stderr, "unable to create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == NULL)
	{
		fprintf(stderr, "unable to create texture: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void display_update(uint32_t *screen)
{
	SDL_UpdateTexture(texture, NULL, screen, SCREEN_WIDTH * sizeof(uint32_t));

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void display_close(void)
{
	SDL_DestroyTexture(texture);
	texture = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}
