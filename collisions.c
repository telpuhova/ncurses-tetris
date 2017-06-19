

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

