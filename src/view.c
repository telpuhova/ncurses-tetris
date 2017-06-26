#include "model.h"
#include "view.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <math.h>

#define KY 1
#define KX 2

void game_start(){
    initscr();
    curs_set(0);
    if (has_colors()){
        start_color();
    }
    mvprintw(8, 5, "PRESS ANY KEY");
    refresh();
}

void game_over(){
    mvprintw(8, 8, "GAME OVER");
    mvprintw(10, 5, "new game? (y/n)");
    refresh();
}

void game_end(){
    endwin();
}

void draw(){
    clear();
    mvprintw(5, 26, "lines = %i", lines);
    mvprintw(7, 26, "level = %i", level);

    get_data();
    for (int i = 0; i < 21; i++){
        for(int j = 0; j < 21; j++){
            if (data[i][j] != 0){
                if (has_colors()){
                    init_pair(data[i][j], data[i][j], 0);
                    attron(COLOR_PAIR(data[i][j]));
                }
                for(int kx = 0; kx < KX; kx++){
                    for(int ky = 0; ky < KY; ky++){
                        mvaddch(i * KY + ky, j * KX + kx, ACS_CKBOARD);
                    }
                }
                if (has_colors()){
                    attroff(COLOR_PAIR(data[i][j]));
                }
            }
        }
    }
    refresh();
}

