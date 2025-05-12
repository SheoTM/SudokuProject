#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "anneal.h"

//Count how many errors are in
int conflicts(const Board *board)
{
    int N = board->size, err = 0;

    for (int i = 0; i < N; ++i) {
        unsigned rowMask = 0, colMask = 0;
        for (int j = 0; j < N; ++j) {
            uint8_t val_row = board->g[i][j]; //val in row
            uint8_t val_col = board->g[j][i]; //val in col
            if (val_row) {
                unsigned bit = 1u << val_row;
                err += (rowMask & bit) != 0; //conflict in row
                rowMask |= bit;
            }
            if (val_col) {
                unsigned bit = 1u << val_col;
                err += (colMask & bit) != 0;  //confinct in col
                colMask |= bit;
            }
        }
    }
    //Check each box
    for (int br = 0; br < N; br += board->box)
        for (int bc = 0; bc < N; bc += board->box) {
            unsigned mask = 0;
            for (int r = br; r < br + board->box; ++r)
                for (int c = bc; c < bc + board->box; ++c) {
                    uint8_t value = board->g[r][c];
                    if (value) {
                        unsigned bit = 1u << value;
                        err += (mask & bit) != 0; //conflict in box
                        mask |= bit;
                    }
                }
        }
    return err;
}

static void shuffle(uint8_t a[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        uint8_t temp = a[i]; a[i] = a[j]; a[j] = temp;
    }
}

void random_fill(Board *board, const Board *initial)
{
    int N = board->size;
    for (int r = 0; r < N; ++r) {
        int used[17] = {0};
        for (int c = 0; c < N; ++c)
            if (initial->g[r][c]) used[ initial->g[r][c] ] = 1;

        uint8_t available[16]; int k = 0;
        for (int value = 1; value <= N; ++value)
            if (!used[value]) available[k++] = value;
        shuffle(available, k);

        int index = 0;
        for (int c = 0; c < N; ++c)
            if (!initial->g[r][c]) board->g[r][c] = available[index++];
    }
}

void swap_in_row(Board *board, const Board *initial)
{
    int N = board->size, r = rand() % N;
    int freeIndex[16], k = 0;
    for (int c = 0; c < N; ++c)
        if (!initial->g[r][c]) freeIndex[k++] = c;
    if (k < 2) return;

    int i = rand() % k, j;
    do { j = rand() % k; } while (j == i);

    uint8_t t = board->g[r][freeIndex[i]];
    board->g[r][freeIndex[i]] = board->g[r][freeIndex[j]];
    board->g[r][freeIndex[j]] = t;
}

static unsigned now_msec(void)
{ return (unsigned)(clock() * 1000 / CLOCKS_PER_SEC); }

int sudoku_sa(Board *board, const Board *initial, unsigned msec_limit)
{
    const double  T0 = 2.0, Tend = 1e-4, alpha = 0.998;
    const unsigned PRINT_EVERY = 1000;

    srand((unsigned)time(NULL)); //For random results
    // srand(0); //For the same results

    Board *cur   = board_clone(initial);
    random_fill(cur, initial);
    int    Ecur  = conflicts(cur);
    int    Ebest = Ecur;

    unsigned long long iter_global = 0;
    double   T      = T0;
    unsigned tstart = now_msec();
    // printf("time limit = %u ms\n", msec_limit);
    //Loop
    while (Ecur && T > Tend && now_msec() - tstart < msec_limit) {
        int iterMax = 50 * board->size * board->size;
        for (int it = 0; it < iterMax; ++it) {

            Board *cand = board_clone(cur);
            swap_in_row(cand, initial);
            int Ecand = conflicts(cand);
            int dE = Ecand - Ecur;

            if (dE <= 0 || exp(-dE / T) > (double)rand() / RAND_MAX) {
                board_free(cur);
                cur = cand;
                Ecur = Ecand;
                if (!Ecur) break;
            } else {
                board_free(cand);
            }
            ++iter_global;
        }

        //Cooling&log every rep
        T *= alpha;
        if (Ecur < Ebest) Ebest = Ecur;
        if (iter_global % PRINT_EVERY == 0)
            printf("Iterations: %-8llu  T=%7.4f  Ecur=%d  Ebest=%d\n",
                   iter_global, T, Ecur, Ebest);
    }

    // copy result to board
    for (int r = 0; r < board->size; ++r)
        memcpy(board->g[r], cur->g[r], board->size);

    int solved = (Ecur == 0);
    board_free(cur);
    // printf("Time elapsed: %u ms\n", now_msec() - tstart);
    return solved;
}
