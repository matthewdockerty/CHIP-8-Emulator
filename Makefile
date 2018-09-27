OBJS = chip8.c display.c main.c

OBJ_NAME = chip8

all : $(OBJS)
	gcc $(OBJS) -Wall -ansi -pedantic -lSDL2 -o $(OBJ_NAME)
