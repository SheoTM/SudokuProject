#include <stdio.h>
#include <ctype.h>
#include "ui.h"

//Printing
void ui_print(int b[N][N])
{
    for (int r = 0; r < N; ++r) {
        if (r % 3 == 0) puts("+-------+-------+-------+");
        for (int c = 0; c < N; ++c) {
            if (c % 3 == 0) printf("| ");
            printf("%c ", b[r][c] ? '0' + b[r][c] : '.');
        }
        puts("|");
    }
    puts("+-------+-------+-------+");
}

int ui_parse(const char *txt,int *row,int *col)
{
    if (!isalpha((unsigned char)txt[0]) || !isdigit((unsigned char)txt[1]))
        return 0;
    *col = toupper((unsigned char)txt[0]) - 'A';
    *row = txt[1] - '1';
    return *row>=0 && *row<9 && *col>=0 && *col<9;
}
