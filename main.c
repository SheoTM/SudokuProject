#include <stdio.h>
#include "sudoku.h"
#include "ui.h"          /* <── dodaj to */

int main(void)
{
    int board[N][N];
    generate_board(board, 30);
    ui_print(board);
    return 0;
}
