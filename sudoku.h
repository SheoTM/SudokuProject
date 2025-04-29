#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdint.h>

#define MAX_N 16

typedef struct {
    uint8_t  size;
    uint8_t  box;
    uint8_t **g;
} Board;

Board *board_create(uint8_t size);
void   board_free(Board *b);

void generate_board(Board *b, uint16_t clues);
int  solve_board   (Board *b);
int  is_safe       (const Board *b, int row, int col, int val);

#endif
