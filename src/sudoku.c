#include <stdint.h>
#include "sudoku.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Create new empty sudoku
Board *board_create(uint8_t n)
{
    Board *board = malloc(sizeof *board);
    board->size = n;
    board->box  = (n==4)?2 : (n==9)?3 : 4; //Box size based on board size
    board->g    = malloc(n * sizeof(uint8_t*));
    for (uint8_t r=0; r<n; ++r)
        board->g[r] = calloc(n, sizeof(uint8_t)); //All cells to 0
    return board;
}

void board_free(Board *board)
{
    for (uint8_t r=0; r<board->size; ++r)
        free(board->g[r]);
    free(board->g);
    free(board);
}
Board *board_clone(const Board *src)
{
    Board *dst = board_create(src->size);
    for (uint8_t r=0; r<src->size; ++r)
        memcpy(dst->g[r], src->g[r], src->size * sizeof(uint8_t));
    return dst;
}
#define MAX_N 16
static uint32_t rowMask[MAX_N], colMask[MAX_N], boxMask[MAX_N];

static inline uint8_t box_id(const Board *board,uint8_t r,uint8_t c) {
    return (r / board->box) * board->box + c / board->box;
}

static void init_masks(const Board *board)
{
    memset(rowMask,0,sizeof rowMask);
    memset(colMask,0,sizeof colMask);
    memset(boxMask,0,sizeof boxMask);
    for(uint8_t r=0; r<board->size; ++r)
        for(uint8_t c=0; c<board->size; ++c)
            if(board->g[r][c]){
                uint32_t bit = 1u << board->g[r][c];
                rowMask[r] |= bit;
                colMask[c] |= bit;
                boxMask[box_id(board,r,c)] |= bit;
            }
}

// Returns true if x isnt in the row i
static int row_free(int i, const Board *board, int x)
{
    for (int c = 0; c < board->size; ++c)
        if (board->g[i][c] == x) return 0;
    return 1;
}
// Returns true if x isnt in the col j
static int col_free(int j, const Board *board, int x)
{
    for (int r = 0; r < board->size; ++r)
        if (board->g[r][j] == x) return 0;
    return 1;
}
//Check element
static int box_free(int i, int j, const Board *board, int x)
{
    int r0 = (i / board->box) * board->box;
    int c0 = (j / board->box) * board->box;
    for (int r = r0; r < r0 + board->box; ++r)
        for (int c = c0; c < c0 + board->box; ++c)
            if (board->g[r][c] == x) return 0;
    return 1;
}

// Backtracking
static int find_empty(const Board *board, int *ri, int *ci)
{
    for (int r = 0; r < board->size; ++r)
        for (int c = 0; c < board->size; ++c)
            if (board->g[r][c] == 0) { *ri = r; *ci = c; return 1; }
    return 0;
}

int is_safe(const Board *board,int i,int j,int x)
{
    return row_free(i,board,x) &&
           col_free(j,board,x) &&
           box_free(i,j,board,x);
}

// Solver
int solve_board(Board *board)
{
    int row, col;
    if (!find_empty(board, &row, &col)) return 1;

    for (int val = 1; val <= board->size; ++val)
    {
        if (is_safe(board,row,col,val))
        {
            uint32_t bit = 1u << val;
            board->g[row][col] = val;
            rowMask[row] |= bit;
            colMask[col] |= bit;
            boxMask[box_id(board,row,col)] |= bit;

            if (solve_board(board)) return 1;

            board->g[row][col] = 0;
            rowMask[row] &= ~bit;
            colMask[col] &= ~bit;
            boxMask[box_id(board,row,col)] &= ~bit;
        }
    }
    return 0;
}

// Generator
static void shuffle(uint8_t a[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
}
static void full_board(Board *board)
{
    for (int r = 0; r < board->size; ++r)
        memset(board->g[r], 0, board->size * sizeof(uint8_t));
    uint8_t perm[MAX_N];
    for (int i = 0; i < board->size; ++i) perm[i] = i + 1;
    shuffle(perm, board->size);
    for (int c = 0; c < board->size; ++c) board->g[0][c] = perm[c];
    init_masks(board);
    solve_board(board);
}

void generate_board(Board *board, uint16_t clues)
{
    if (clues < board->size) clues = board->size;
    if (clues > board->size * board->size) clues = board->size * board->size;

    srand((unsigned)time(NULL));
    full_board(board);

    int to_remove = board->size * board->size - clues;
    while (to_remove)
    {
        int r = rand() % board ->size, c = rand() % board->size;
        if (board->g[r][c] == 0)
            continue;
        board->g[r][c] = 0;
        --to_remove;
    }
}
