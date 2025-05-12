#include "save.h"
#include <stdio.h>

int save_game(const char *p, const Board *board)
{
    FILE *f = fopen(p,"w");
    if(!f)
      return 0;
    fprintf(f,"%d\n", board->size);
    for(int r=0; r<board->size; ++r){
        for(int c=0; c<board->size; ++c)
            fprintf(f,"%d", board->g[r][c]);
        fputc('\n',f);
    }
    fclose(f);
    return 1;
}

int load_game(const char *p, Board *board)
{
    FILE *f = fopen(p,"r");
    if(!f)
      return 0;

    int fileSize;
    if(fscanf(f,"%d",&fileSize ) !=1 || fileSize != board->size){
        fclose(f);
        return 0;
    }
    for (int r = 0; r < board->size; ++r)
        for (int c = 0; c < board->size; ++c) {
            int tmp;
            if (fscanf(f, "%1d", &tmp) != 1) {
                fclose(f); return 0;
            }
            board->g[r][c] = (uint8_t)tmp;
        }
    fclose(f);
    return 1;
}
