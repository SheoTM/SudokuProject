#include <stdio.h>
#include "sudoku.h"

static void print_board(int b[N][N])
{
    for (int r = 0; r < N; ++r) {
        if (r % 3 == 0)  puts("+-------+-------+-------+");
        for (int c = 0; c < N; ++c) {
            if (c % 3 == 0) printf("| ");
            printf("%c ", b[r][c] ? '0' + b[r][c] : '.');
        }
        puts("|");
    }
    puts("+-------+-------+-------+");
}

int main(void)
{
    int board[N][N];
    generate_board(board, 30);   /* 30 wypełnionych pól = poziom średni */
    print_board(board);
    return 0;
}
