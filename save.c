#include "save.h"
#include <stdio.h>

int save_game(const char *p, const Board *b)
{
    FILE *f = fopen(p,"w");
    if(!f)
      return 0;
    fprintf(f,"%d\n", b->size);
    for(int r=0;r<b->size;++r){
        for(int c=0;c<b->size;++c)
            fprintf(f,"%d", b->g[r][c]);
        fputc('\n',f);
    }
    fclose(f);
    return 1;
}

int load_game(const char *p, Board *b)
{
    FILE *f = fopen(p,"r");
    if(!f)
      return 0;

    int sz;
    if(fscanf(f,"%d",&sz)!=1 || sz!=b->size){
        fclose(f);
        return 0;
    }
    for (int r = 0; r < b->size; ++r)
        for (int c = 0; c < b->size; ++c) {
            int tmp;
            if (fscanf(f, "%1d", &tmp) != 1) { fclose(f); return 0; }
            b->g[r][c] = (uint8_t)tmp;
        }
    fclose(f);
    return 1;
}
