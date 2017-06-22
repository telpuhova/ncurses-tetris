CC=gcc
CFLAGS=-lncurses

tetris: ./src/controller.o ./src/model.o ./src/view.o
	$(CC) -o tetris ./src/controller.o ./src/model.o ./src/view.o -lncurses
