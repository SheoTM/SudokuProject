#include <stdio.h>
#include <string.h>
#include "sudoku.h"
#include "ui.h"
#include "save.h"

// Check if there are no empty fields
static int board_full(int b[N][N])
{
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (b[r][c] == 0) return 0;
    return 1;
}

static void play_game(int board[N][N])
{
    int initial[N][N];
    memcpy(initial, board, sizeof board);   //Remember the clues

    char line[32];

    while (1) {
        ui_print(board);

        if (board_full(board)) {
            puts("Congratulations! You solved it!");
            break;
        }

        printf("Move: ");
        if (!fgets(line, sizeof line, stdin)) break;

        // Commands
        if (!strncmp(line, "q", 1)) break;

        if (!strncmp(line, "save", 4)) {
            if (save_game("save.txt", board))
                puts("Saved to save.txt");
            else
                puts("Save error!");
            continue;
        }
        if (!strncmp(line, "load", 4)) {
            if (load_game("save.txt", board)) {
                memcpy(initial, board, sizeof board);  //Update the clues
                puts("Loaded");
            } else
                puts("No save file");
            continue;
        }

        // Player move
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
            puts("Incorrect number");
            continue;
        }

        board[r][c] = val;
    }
}

// Menu
int main(void)
{
    int board[N][N];
//TODO: Add instruction and 4x4, 9x9, 16x16 mode
    while (1) {
        puts("\n           MAIN MENU           ");
        puts("1. New game");
        puts("2. Load save");
        puts("3. Instruction");
        puts("0. Quit");
        printf("Choose: ");

        char sel[8];
        if (!fgets(sel, sizeof sel, stdin)) break;

        if (sel[0] == '0')
            break;

        else if (sel[0] == '1') {
            int clues;
            printf("How many clues? (17-81): ");
            if (scanf("%d", &clues) != 1) {
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            generate_board(board, clues);
            play_game(board);
        }

        else if (sel[0] == '2') {
            if (load_game("save.txt", board)) {
                puts("Save loaded");
                play_game(board);
            } else {
                puts("save.txt not found");
            }
        }

        else {
            puts("Unknown option – choose 0, 1 or 2");
        }
    }
    puts("Carbonaura");
    return 0;
}
