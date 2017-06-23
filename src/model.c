#include "model.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <math.h>

#define KY 1
#define KX 2
#define MK 7

int game_field[21][12]; 
int current_figure[4][2];
int fig_num[2], rotation_flag;


const uint8_t FIGURES[7][4][2] = {
//    // line
    {
        {0, 0},{1, 0},{2, 0},{3, 0}
    },
//    // cube
    {
        {0, 0},{1, 0},{1, 1},{0, 1}
    },
//    // turn to the right ---|
    {
        {0, 0},{1, 0},{2, 0},{2, 1}
    },
//    // turn to the left |--- 
    {
        {0, 1},{1, 1},{2, 1},{2, 0}
    },
//    // cube shifted to the right __--
    {
        {0, 0},{0, 1},{1, 1},{1, 2}
    },
//    // cube shifted to the left --__
    {
        {0, 0},{1, 0},{1, 1},{2, 1}
    },
//    // three to one
    {
        {0, 0},{1, 0},{2, 0},{1, 1}
    }
};


void get_data(){

    //int data[21][12];
    for (int i=0; i<21; i++){
        for (int j=0; j<12; j++){
            data[i][j] = game_field[i][j];
        }
    }

    for (int i=0; i<4; i++){
        data[current_figure[i][1]][current_figure[i][0]] = fig_num[1] + 1;
    }

    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            data[9 + i][14 + j] = 0;
        }
    }

    for (int i=0; i<4; i++){
        data[ FIGURES[fig_num[0]][i][1] + 9][ FIGURES[fig_num[0]][i][0] + 14] = fig_num[0] + 1;
    }

    //return data;
}

void set_game_field(){

    //filling the gamefield with barriers
    for (int i=0; i<20; i++){
        for (int j=1; j<11; j++){
            game_field[i][j] = 0;
        }
    }
    for (int j=1; j<11; j++){ game_field[20][j] = 7; }
    for (int i=0; i<21; i++){ game_field[i][0] = 7; game_field[i][11] = 7; }

    // clearing the score
    lines = 0;
    level = 0;
    fig_num[0] = rand()%7;
}

bool game_on(){
    for (int j=1; j<11; j++){
        if (game_field[0][j] != 0) { return false; }
    }
    return true;
}

void generate_figure(){
    //generate figure
    fig_num[1] = fig_num[0]; 
    fig_num[0] = rand()%7; //rand number for a figure
    for (int i=0; i<4; i++){
        for (int j =0; j<2; j++){
            current_figure[i][j] = FIGURES[fig_num[1]][i][j];
        }
        current_figure[i][0] += 5;
        current_figure[i][1] -= 1;
    }
}

//collisions

bool collision_y(){
    for (int i = 0; i < 4; i++){
        if (game_field[current_figure[i][1] + 1][current_figure[i][0]] != 0){ return true; }
    }
    return false;
}

bool collision_x_right(){
    for (int i = 0; i < 4; i++){
        if (game_field[current_figure[i][1]][current_figure[i][0] + 1] != 0){ return true; }
    }
    return false;
}

bool collision_x_left(){
    for (int i = 0; i < 4; i++){
        if (game_field[current_figure[i][1]][current_figure[i][0] - 1] != 0){ return true; }
    }
    return false;
}

//

bool able_to_fall(){
    return !collision_y();
}

bool able_to_move_left(){
    return !collision_x_left();
}

bool able_to_move_right(){
    return !collision_x_right();
}

void move_down(){
    for (int k=0; k<4; k++){
        current_figure[k][1]++; // MOVE DOWN
    }
}

void move_right(){
    for (int k=0; k<4; k++){
        current_figure[k][0]++;
    }
}

void move_left(){
    for (int k=0; k<4; k++){
        current_figure[k][0]--;
    }
}

// min&max

int maxy(){
    int j = 1;
    int result = current_figure[0][j];
    for (int i=0; i<4; i++){
        if (result < current_figure[i][j]){ result = current_figure[i][j]; }
    }
    return result;
}

int maxx(){
    int j = 0;
    int result = current_figure[0][j];
    for (int i=0; i<4; i++){
        if (result < current_figure[i][j]){ result = current_figure[i][j]; }
    }
    return result;
}

int miny(){
    int j = 1;
    int result = current_figure[0][j];
    for (int i=0; i<4; i++){
        if (result > current_figure[i][j]){ result = current_figure[i][j]; }
    }
    return result;
}

int minx(){
    int j = 0;
    int result = current_figure[0][j];
    for (int i=0; i<4; i++){
        if (result > current_figure[i][j]){ result = current_figure[i][j]; }
    }
    return result;
}

//

void rotate(){
//void rotation(int * current_figure, int * game_field, int * rotation_flag, int fig_num){
    int collision_flag[4];
    for (int i=0; i<4; i++){
        collision_flag[i] = 0;
    }
    bool simmetry;
    if (fig_num[1] <= 1) { simmetry = true; }
    if (fig_num[1] > 1) { simmetry = false; }
    
    int start_j = minx(current_figure);
    int start_i = miny(current_figure);
    int i, j;

    if (rotation_flag == 4){ rotation_flag = 0; }
    if (simmetry == false){
        if (rotation_flag == 2){ start_j += 1; }
        if (rotation_flag == 3){ start_i += 1; }
    }
    if (simmetry == true){
        int l = ((maxy(current_figure)-miny(current_figure)) + (maxx(current_figure) - minx(current_figure)))%2;
        if (rotation_flag == 0){ start_i -= l; start_j += l; }
        if (rotation_flag == 1){ start_i += l; }
        if (rotation_flag == 2){ start_j += l; }
        if (rotation_flag == 3){ start_i += l; start_j -= l; }
    }

    for (int b = 0; b<4; b++){
        //i = *(current_figure + b*MJ + 1);
        i = current_figure[b][1];
        //j = *(current_figure + b*MJ + 0);
        j = current_figure[b][0];

        //small sqr
        if ((i == start_i) && (j == start_j)){
            //*(current_figure + b*MJ + 0) += 1;
            current_figure[b][0] += 1;
        }
        if ((i == start_i) && (j == start_j + 1)){
            //*(current_figure + b*MJ + 1) += 1;
            current_figure[b][1] += 1;
        }
        if ((i == start_i + 1) && (j == start_j + 1)){ 
            //*(current_figure + b*MJ + 0) -= 1;
            current_figure[b][0] -= 1;
        }
        if ((i == start_i + 1) && (j == start_j)){
            //*(current_figure + b*MJ + 1) -= 1;
            current_figure[b][1] -= 1;
        }

        //large sqr
        if (j == start_j+2){
            //*(current_figure + b*MJ + 0) = start_j + start_i - i + 1;
            current_figure[b][0] = start_j + start_i - i + 1;
            //*(current_figure + b*MJ + 1) = start_i + 2;
            current_figure[b][1] = start_i + 2;
            //if (*(game_field + *(current_figure + b*MJ + 1)*MJ_GF + *(current_figure + b*MJ + 0)) != 0){ collision_flag[0] += 1; }
            if (game_field[current_figure[b][1]][current_figure[b][0]] != 0){ collision_flag[0] += 1; }
        }
        if (i == start_i+2){
            current_figure[b][0] = start_j - 1;
            current_figure[b][1] = j - start_j + start_i;
            if (game_field[current_figure[b][1]][current_figure[b][0]] != 0){ collision_flag[1] += 1; }
        }
        if (j == start_j-1){
            current_figure[b][0] = start_j + start_i - i + 1;
            current_figure[b][1] = start_i - 1;
            if (game_field[current_figure[b][1]][current_figure[b][0]] != 0){ collision_flag[2] += 1; }
        }
        if (i == start_i-1){
            current_figure[b][0] = start_j + 2;
            current_figure[b][1] = j - start_j + start_i;
            if (game_field[current_figure[b][1]][current_figure[b][0]] != 0){ collision_flag[3] += 1; }
        }
    }
    rotation_flag += 1;
    for (int i=0; i<4; i++){
        if (collision_flag[0] != 0){
            current_figure[i][1] -= collision_flag[0];
        }
        if (collision_flag[1] != 0){
            current_figure[i][0] += collision_flag[1];
        }
        if (collision_flag[2] != 0){
            current_figure[i][1] += collision_flag[2];
        }
        if (collision_flag[3] != 0){
            current_figure[i][0] -= collision_flag[3];
        }
    }
}


void figure_to_game_field(){
    //figure to the gamefield
    for (int i=0; i<4; i++){
        game_field[current_figure[i][1]][current_figure[i][0]] = fig_num[1] + 1;
    }
}

void erase_filled_lines(){
    //erasing filled lines
    for (int line=0; line<20; line++){
        int f = 0;
        for (int row=1; row<11; row++){
            if (game_field[line][row] == 0) { f=1; break; }
        }
        if (f==1) { continue; }
        else{
            for (int l=line; l>1; l--){
                for (int j=1; j<11; j++){
                    game_field[l][j] = game_field[l-1][j];
                }
            }
            lines += 1;
            if (lines == 10){ level += 1; lines = 0; }
        }
    }
}

double power(double x, double y){
    double result = 1;
    for (int i=0; i<y; i++){
        result *= x;
    }
    return result;
}


