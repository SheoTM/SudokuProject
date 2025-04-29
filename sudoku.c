#include "sudoku.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int row_free(int i, int board[][N], int x)
{
    for (int c = 0; c < N; ++c)
        if (board[i][c] == x) return 0;
    return 1;
}

static int col_free(int j, int board[][N], int x)
{
    for (int r = 0; r < N; ++r)
        if (board[r][j] == x) return 0;
    return 1;
}

static int box_free(int i, int j, int board[][N], int x)
{
    int r0 = (i / 3) * 3, c0 = (j / 3) * 3;
    for (int r = r0; r < r0 + 3; ++r)
        for (int c = c0; c < c0 + 3; ++c)
            if (board[r][c] == x) return 0;
    return 1;
}

static int is_safe(int i, int j, int board[][N], int x)
{
    return row_free(i, board, x) &&
           col_free(j, board, x) &&
           box_free(i, j, board, x);
}

//Backtracking
static int find_empty(int board[][N], int *ri, int *ci)
{
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (board[r][c] == 0) { *ri = r; *ci = c; return 1; }
    return 0;
}

int solve_board(int board[N][N])
{
    int row, col;
    if (!find_empty(board, &row, &col)) return 1;

    for (int val = 1; val <= 9; ++val)
    {
        if (is_safe(row, col, board, val))
        {
            board[row][col] = val;
            if (solve_board(board)) return 1;
            board[row][col] = 0;
        }
    }
    return 0;
}

//Generator
static void shuffle(int a[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
}

static void full_board(int board[N][N])
{
    memset(board, 0, sizeof(int)*N*N);
    int perm[N]; for (int i = 0; i < N; ++i) perm[i] = i + 1;
    shuffle(perm, N);
    for (int c = 0; c < N; ++c) board[0][c] = perm[c];
    solve_board(board);
}

void generate_board(int board[N][N], int clues)
{
    if (clues < 17) clues = 17;
    if (clues > 81) clues = 81;

    srand((unsigned)time(NULL));
    full_board(board);

    int to_remove = N*N - clues;
    while (to_remove)
    {
        int r = rand() % N, c = rand() % N;
        if (board[r][c] == 0) continue;
        board[r][c] = 0;
        --to_remove;
    }
}
