#ifndef UI_H
#define UI_H
#include "sudoku.h"

void ui_print(const Board *board);
int  ui_parse(const Board *board, const char *txt, int *row, int *col);

#endif
