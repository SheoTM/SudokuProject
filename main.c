#include <stdio.h>
#include "sudoku.h"
#include "ui.h"
#include "string.h"
//Check if there is no empty fields
static int board_full(int b[N][N]) {
    for (int r=0; r<N; ++r)
        for (int c=0; c<N; ++c)
            if (b[r][c] == 0) return 0;
    return 1;
}

int main(void)
{
    int board[N][N];
    generate_board(board, 30);
    int initial[N][N];
    memcpy(initial, board, sizeof board);
    char line[32];
    while (1) {
        ui_print(board);

        if (board_full(board)) {
            puts("Congratulations! U solved");
            break;
        }

        printf("Move (np. B3 7 | q = quit): ");

        if (!fgets(line, sizeof line, stdin)) break;
        if (line[0] == 'q') break;

        char crd[3];
        int  val;
        if (sscanf(line, "%2s %d", crd, &val) != 2) {
            puts("Incorrect format. Try: B3 7");
            continue;
        }

        int r, c;
        if (!ui_parse(crd, &r, &c) || val < 1 || val > 9) {
            puts("Incorrect data");
            continue;
        }
        if (initial[r][c]) {
            puts("This field is locked");
            continue;
        }
        if (!is_safe(r, c, board, val)) {
            puts("The move breaks the rules");
            continue;
        }

        board[r][c] = val;
    }
    return 0;
}
