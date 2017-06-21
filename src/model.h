#ifndef TETRIS_MODEL_H
#define TETRIS_MODEL_H

#include <ncurses.h>

int data[21][21];
int lines;
int level;

void get_data(void);

void set_game_field(void);

bool game_on(void);

void generate_figure(void);

bool able_to_fall(void);

bool able_to_move_left(void);

bool able_to_move_right(void);

void move_down(void);

void move_right(void);

void move_left(void);

void rotate(void);

void figure_to_game_field(void);

void erase_filled_lines(void);

double power(double, double);

#endif
