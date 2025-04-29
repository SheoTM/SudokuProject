#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sudoku.h"
#include "ui.h"
#include "save.h"

//Check if there are no empty fields
static int board_full(const Board *b)
{
    for (int r = 0; r < b->size; ++r)
        for (int c = 0; c < b->size; ++c)
            if (b->g[r][c] == 0) return 0;
    return 1;
}

static void play_game(Board *b)
{
    //Remember clues
    Board *initial = board_create(b->size);
    for (int r = 0; r < b->size; ++r)
        memcpy(initial->g[r], b->g[r], b->size);

    char line[32];

    while (1) {
        ui_print(b);

        if (board_full(b)) {
            puts("Congratulations! You solved it!");
            break;
        }

        printf("Move: ");
        if (!fgets(line, sizeof line, stdin)) break;

        if (!strncmp(line, "q", 1)) break;

        if (!strncmp(line, "save", 4)) {
            if (save_game("save.txt", b))
                puts("Saved to save.txt");
            else
                puts("Save error!");
            continue;
        }
        if (!strncmp(line, "load", 4)) {
            if (load_game("save.txt", b)) {
                for (int r = 0; r < b->size; ++r)
                    memcpy(initial->g[r], b->g[r], b->size);
                puts("Loaded");
            } else
                puts("No save file");
            continue;
        }

        //Player move
        char crd[4];
        int  val;
        if (sscanf(line, "%3s %d", crd, &val) != 2) {
            puts("Incorrect format. Try: B3 7");
            continue;
        }

        int r, c;
        if (!ui_parse(b, crd, &r, &c) || val < 1 || val > b->size) {
            puts("Incorrect data");
            continue;
        }
        if (initial->g[r][c]) {
            puts("This field is locked");
            continue;
        }
        if (!is_safe(b, r, c, val)) {
            puts("Incorrect number");
            continue;
        }

        b->g[r][c] = (uint8_t)val;
    }

    board_free(initial);
}

// Menu
int main(void)
{
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

            int size;
            printf("Board size (4 / 9 / 16): ");
            if (scanf("%d", &size) != 1 || (size!=4 && size!=9 && size!=16)) {
                puts("Invalid size.");
                while (getchar()!='\n');
                continue;
            }

            int clues_min = size;
            int clues_max = size*size;
            int clues;
            printf("How many clues? (%d-%d): ", clues_min, clues_max);
            if (scanf("%d", &clues) != 1) {
                while (getchar()!='\n');
                continue;
            }
            while (getchar()!='\n');

            Board *b = board_create((uint8_t)size);
            generate_board(b, (uint16_t)clues);
            play_game(b);
            board_free(b);
        }

        //Loading save
        else if (sel[0] == '2') {
            FILE *test = fopen("save.txt","r");
            if(!test) {
                puts("save.txt not found"); continue;
            }

            int temp_size;
            if(fscanf(test,"%d",&temp_size)!=1 ||
               (temp_size!=4 && temp_size!=9 && temp_size!=16)) {
                fclose(test); puts("Save corrupted"); continue;
            }
            fclose(test);

            Board *b = board_create((uint8_t)temp_size);
            if(load_game("save.txt", b)){
                puts("Save loaded");
                play_game(b);
            } else {
                puts("Load failed");
            }
            board_free(b);
        }

        //Instruction
        else if (sel[0] == '3') {
            puts("\nEnter moves for any size as e.g.  B3 7");
            puts("Columns are letters A-P, rows are 1-16");
            puts("Commands inside game:  save   load   q");
        }

        else {
            puts("Unknown option – choose 0, 1, 2 or 3");
        }
    }
    puts("Carbonaura");
    return 0;
}
