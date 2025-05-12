#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "src/ui.h"
#include "src/save.h"
#include "src/anneal.h"
#include "src/sudoku.h"

//If board is full
static int board_full(const Board *b)
{
    for (int r=0;r<b->size;++r)
        for (int c=0;c<b->size;++c)
            if (b->g[r][c]==0) return 0; //empty cell found
    return 1; //all cells filled
}


static void play_game(Board *board)
{
    Board *initial = board_clone(board);
    char line[32];

    while (1) {
        ui_print(board);
        if (board_full(board)) {
            puts("Congratulations!");
            break;
        }

        printf("Move: "); if (!fgets(line,sizeof line,stdin))
            break;
        if (!strncmp(line,"quit",1))
            break;

        if (!strncmp(line,"save",4)){
            puts(save_game("save.txt",board)? "Saved." : "Save error!");
            continue;
        }

        if (!strncmp(line,"load",4)){
            if (load_game("save.txt",board)){
                for(int r=0; r<board->size; ++r)
                    memcpy(initial->g[r],board->g[r],board->size);
                puts("Loaded"); } else puts("No save");
            continue;
        }

        char crd[4]; int val;
        if (sscanf(line,"%3s %d",crd,&val)!=2){
            puts("Format: B3 7");
            continue;
        }

        int r,c;
        if (!ui_parse(board,crd,&r,&c) || val<1 || val>board->size){
            puts("Bad data");
            continue;
        }
        if (initial->g[r][c]){
            puts("Locked");
            continue;
        }
        if (!is_safe(board,r,c,val)){
            puts("Conflict");
            continue;
        }
        board->g[r][c]=(uint8_t)val;
    }
    board_free(initial);
}

//Menu
int main(void)
{
    while (1) {
        puts("\n           MAIN MENU           ");
        puts("1. New game");
        puts("2. Load save");
        puts("3. Instruction");
        puts("4. Solve (Simulated Annealing)");
        puts("0. Quit");
        printf("Choose: ");

        char sel[8]; if (!fgets(sel,sizeof sel,stdin))
            break;

        //Quit
        if (sel[0]=='0')
            break;

        //New game
        else if (sel[0]=='1') {
            int size;
            printf("Board size (4 / 9 / 16): ");
            if (scanf("%d",&size)!=1 || (size!=4&&size!=9&&size!=16)){ puts("Invalid."); while(getchar()!='\n'); continue; }

            int clues; printf("How many clues? (%d-%d): ",size,size*size);
            if (scanf("%d",&clues)!=1){ while(getchar()!='\n'); continue; }
            while (getchar()!='\n');

            Board *board = board_create(size);
            generate_board(board,clues);
            play_game(board); board_free(board);
        }

        //Load game
        else if (sel[0]=='2'){
            FILE *t=fopen("save.txt","r");
            if(!t){ puts("save.txt not found");
                continue;
            }
            int s; if(fscanf(t,"%d",&s)!=1||(s!=4&&s!=9&&s!=16)){ fclose(t); puts("Save corrupted");
                continue;
            }
            fclose(t);

            Board *board=board_create(s);
            if (load_game("save.txt",board)){ puts("Save loaded");
                play_game(board);
            }
            else puts("Load failed");
            board_free(board);
        }

        //Instruction
        else if (sel[0]=='3'){
            puts("\nType moves like  B3 7");
            puts("Columns A-P, rows 1-16");
            puts("Commands in game: save / load / quit");
        }

        else if (sel[0] == '4') {
            //Check if file exists
            FILE *f = fopen("save.txt", "r");
            if (!f) { puts("save.txt not found");
                continue;
            }

            int FileSize;
            if (fscanf(f, "%d", &FileSize) != 1 ||
                (FileSize != 4 && FileSize != 9 && FileSize != 16)) {
                fclose(f); puts("Save corrupted");
                continue;
                }
            fclose(f);

            //Load board
            Board *board = board_create((uint8_t) FileSize);
            if (!load_game("save.txt", board)) {
                puts("Load failed"); board_free(board);
                continue;
            }

            Board *initial = board_clone(board);
            int ok = sudoku_sa(board, initial, 10000);

            ui_print(board);

            if (ok)
                puts("Solved");

            board_free(initial);
            board_free(board);
        }

        else puts("Unknown option");
    }
    puts("Carbonaura!");
    return 0;
}
