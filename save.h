#ifndef SAVE_H
#define SAVE_H
#include "sudoku.h"

int save_game(const char *path, const Board *board);
int load_game(const char *path, Board *board);

#endif