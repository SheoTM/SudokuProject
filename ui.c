#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "ui.h"

static char cell(uint8_t v)
{
    if (v == 0)  return '.';
    if (v <= 9)  return '0' + v;          //1-9
    return 'A' + (v - 10);                //10-16 - A-G
}

static void print_col_header(uint8_t n, uint8_t box)
{
    printf("     ");
    for (uint8_t c = 0; c < n; ++c) {
        if (c % box == 0 && c != 0) printf("  ");
        printf("%c ", 'A' + c);
    }
    putchar('\n');
}

static void print_hline(uint8_t box)
{
    printf("    ");
    for(uint8_t b = 0; b < box; ++b){
        printf("-");
        for(uint8_t j = 0; j < box*2+1; ++j) printf("-");
    }
    puts("");
}

void ui_print(const Board *b)
{
    uint8_t n   = b->size;
    uint8_t box = b->box;

    print_col_header(n, box);

    for (uint8_t r = 0; r < n; ++r) {
        if (r % box == 0) print_hline(box);

        printf("%2d  ", r + 1);

        for (uint8_t c = 0; c < n; ++c) {
            if (c % box == 0) printf("| ");
            printf("%c ", cell(b->g[r][c]));
        }
        puts("|");
    }
    print_hline(box);
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
