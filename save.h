/* save.h */
#ifndef SAVE_H
#define SAVE_H
#include "sudoku.h"
int save_game(const char *path,int b[N][N]);
int load_game(const char *path,int b[N][N]);
#endif
