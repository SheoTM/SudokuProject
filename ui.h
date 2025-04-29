#ifndef UI_H
#define UI_H
#include "sudoku.h"

void ui_print(const Board *b);
int  ui_parse(const Board *b, const char *txt, int *row, int *col);

#endif
