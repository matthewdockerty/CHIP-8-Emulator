OBJS = chip8.c display.c

OBJ_NAME = chip8

all : $(OBJS)
	gcc $(OBJS) -Wall -ansi -pedantic -lSDL2 -o $(OBJ_NAME)
