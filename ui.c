#include <stdio.h>
#include <ctype.h>
#include "ui.h"
#include <stdlib.h>

static char cell(uint8_t v)
{
    if(v==0)        return '.';
    if(v<=9)        return '0'+v;
    return 'A'+(v-10);
}

void ui_print(const Board *b)
{
    uint8_t n=b->size, box=b->box;

    for(uint8_t r=0;r<n;++r){
        if(r%box==0){
            for(uint8_t i=0;i<box;++i){
                printf("+");
                for(uint8_t j=0;j<box*2+1;++j) printf("-");
            }
            puts("+");
        }
        for(uint8_t c=0;c<n;++c){
            if(c%box==0) printf("| ");
            printf("%c ", cell(b->g[r][c]));
        }
        puts("|");
    }
    for(uint8_t i=0;i<box;++i){
        printf("+");
        for(uint8_t j=0; j<box*2+1; ++j) printf("-");
    }
    puts("+");
}

int ui_parse(const Board *b,const char *txt,int *row,int *col)
{
    if(!isalpha((unsigned char)txt[0]) || !isdigit((unsigned char)txt[1]))
        return 0;
    int c=toupper((unsigned char)txt[0])-'A';
    int r=atoi(txt+1)-1;
    if(r<0||r>=b->size||c<0||c>=b->size) return 0;
    *row=r; *col=c; return 1;
}
