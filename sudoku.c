
#include <stdint.h>
#include "sudoku.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Board allocator
Board *board_create(uint8_t n)
{
    Board *b = malloc(sizeof *b);
    b->size  = n;
    b->box   = (n==4)?2 : (n==9)?3 : 4;
    b->g     = malloc(n * sizeof(uint8_t*));
    for(uint8_t r=0; r<n; ++r)
        b->g[r] = calloc(n, sizeof(uint8_t));
    return b;
}
void board_free(Board *b)
{
    for(uint8_t r=0;r<b->size;++r) {
        free(b->g[r]);
    }
    free(b->g);
    free(b);
}

#define MAX_N 16
static uint32_t rowMask[MAX_N], colMask[MAX_N], boxMask[MAX_N];

static inline uint8_t box_id(const Board *b,uint8_t r,uint8_t c) {
    return (r / b->box) * b->box + c / b->box;
}

static void init_masks(const Board *b)
{
    memset(rowMask,0,sizeof rowMask);
    memset(colMask,0,sizeof colMask);
    memset(boxMask,0,sizeof boxMask);
    for(uint8_t r=0; r<b->size; ++r)
        for(uint8_t c=0; c<b->size; ++c)
            if(b->g[r][c]){
                uint32_t bit = 1u << b->g[r][c];
                rowMask[r] |= bit;
                colMask[c] |= bit;
                boxMask[box_id(b,r,c)] |= bit;
            }
}

// Returns true if x isnt in the row i
static int row_free(int i, const Board *b, int x)
{
    for (int c = 0; c < b->size; ++c)
        if (b->g[i][c] == x) return 0;
    return 1;
}
// Returns true if x isnt in the col j
static int col_free(int j, const Board *b, int x)
{
    for (int r = 0; r < b->size; ++r)
        if (b->g[r][j] == x) return 0;
    return 1;
}
//Check element
static int box_free(int i, int j, const Board *b, int x)
{
    int r0 = (i / b->box) * b->box;
    int c0 = (j / b->box) * b->box;
    for (int r = r0; r < r0 + b->box; ++r)
        for (int c = c0; c < c0 + b->box; ++c)
            if (b->g[r][c] == x) return 0;
    return 1;
}

// Backtracking - looking for an empty cell
static int find_empty(const Board *b, int *ri, int *ci)
{
    for (int r = 0; r < b->size; ++r)
        for (int c = 0; c < b->size; ++c)
            if (b->g[r][c] == 0) { *ri = r; *ci = c; return 1; }
    return 0;
}

int is_safe(const Board *b,int i,int j,int x)
{
    return row_free(i,b,x) &&
           col_free(j,b,x) &&
           box_free(i,j,b,x);
}

// Solver
int solve_board(Board *b)
{
    int row, col;
    if (!find_empty(b, &row, &col)) return 1;

    for (int val = 1; val <= b->size; ++val)
    {
        if (is_safe(b,row,col,val))
        {
            uint32_t bit = 1u << val;
            b->g[row][col] = val;
            rowMask[row] |= bit; colMask[col] |= bit; boxMask[box_id(b,row,col)] |= bit;

            if (solve_board(b)) return 1;

            b->g[row][col] = 0;
            rowMask[row] &= ~bit; colMask[col] &= ~bit; boxMask[box_id(b,row,col)] &= ~bit;
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
static void full_board(Board *b)
{
    memset(b->g[0], 0, b->size*b->size);
    uint8_t perm[MAX_N];
    for (int i = 0; i < b->size; ++i) perm[i] = i + 1;
    shuffle(perm, b->size);
    for (int c = 0; c < b->size; ++c) b->g[0][c] = perm[c];
    init_masks(b);
    solve_board(b);
}

void generate_board(Board *b, uint16_t clues)
{
    if (clues < b->size) clues = b->size;
    if (clues > b->size * b->size) clues = b->size * b->size;

    srand((unsigned)time(NULL));
    full_board(b);

    int to_remove = b->size * b->size - clues;
    while (to_remove)
    {
        int r = rand() % b ->size, c = rand() % b->size;
        if (b->g[r][c] == 0)
            continue;
        b->g[r][c] = 0;
        --to_remove;
    }
}
