#ifndef SUDOKU_H
#define SUDOKU_H

#define N 9
void generate_board(int board[N][N], int clues);
int  solve_board(int board[N][N]);
int  is_safe(int row,int col,int board[N][N],int val);

#endif
