#ifndef UI_H
#define UI_H
#include "sudoku.h"

void ui_print(int board[N][N]);
int  ui_parse(const char *txt,int *row,int *col);

#endif
