#include "tetris_model.h"
#include "tetris_view.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <math.h>
//#include <conio.h>

int main(){

    char key_pressed;
    struct timespec start_time, stop_time;
    int64_t t1, t2, delta_t = 1, summ_delta_t = 0, sk;
    time_t rand_num_generator;
    int speed_koef;

    srand((unsigned) time(&rand_num_generator));

    while (true){
        game_start();
        noecho();
        timeout(-1);
        getch();

        set_game_field();

        while (game_on()) {
            generate_figure();
            int rotation_flag = 0;
            //speed_koef = power(2, level);

            while (able_to_fall()) {

                clock_gettime(CLOCK_REALTIME, &start_time);
                t1 = start_time.tv_nsec;
                if (((summ_delta_t*(2*level+1))/(delta_t*2000000)) > 0){
                    move_down();
                    draw();
                    summ_delta_t = 0;
                } 
                else {
                    timeout(0);
                    key_pressed = getch();
                    if ((key_pressed == 'l') && (able_to_move_right())) { 
                        move_right();
                        draw();
                    }
                    if ((key_pressed == 'j') && (able_to_move_left())) { 
                        move_left();
                        draw();
                    }
                    if (key_pressed == 'i') { 
                        rotate();
                        draw();
                    }
                    if ((key_pressed == 'k') && (able_to_fall())) { 
                        move_down();
                        draw();
                    }
                    if (key_pressed == 'p') { 
                        // pause
                        game_start();
                        noecho();
                        timeout(-1);
                        getch();
                    }
                }

                clock_gettime(CLOCK_REALTIME, &stop_time);
                t2 = stop_time.tv_nsec;

                delta_t = abs(t2-t1);
                summ_delta_t += delta_t;
            } 
            figure_to_game_field();
            erase_filled_lines();
        }

        game_over();
        timeout(-1);

        while(true) {
            key_pressed = getch();
            if (key_pressed == 'y'){
                clear();
                break;
            }
            else if (key_pressed == 'n'){
                game_end();
                return 0;
            }
        }
    }
};

