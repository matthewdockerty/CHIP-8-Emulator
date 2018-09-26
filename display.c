#include "display.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

const int DISPLAY_WIDTH = DISPLAY_SCALE * SCREEN_WIDTH;
const int DISPLAY_HEIGHT = DISPLAY_SCALE * SCREEN_HEIGHT;

void display_init(void)
{
	/* Window and surface contained by the window. */
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;

	/* Initialize SDL. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "unable to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Create the window. */
	window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		fprintf(stderr, "unable to create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Get and setup the window surface */
	surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);

	SDL_Delay(2000);

	/* Destroy the window. */
	SDL_DestroyWindow(window);

	/* Quit. */
	SDL_Quit();
}
