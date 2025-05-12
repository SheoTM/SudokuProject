#ifndef ANNEAL_H
#define ANNEAL_H
#include "sudoku.h"

int conflicts(const Board *board);
void random_fill(Board *board, const Board *initial);
void swap_in_row(Board *board, const Board *initial);
int sudoku_sa(Board *board, const Board *initial, unsigned msec_limit);

#endif
