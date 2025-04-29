#include "save.h"
#include <stdio.h>

int save_game(const char *p, int b[N][N])
{
    FILE *f = fopen(p, "w");
    if (!f) return 0;

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c)
            fprintf(f, "%d", b[r][c]);
        fputc('\n', f);
    }
    fclose(f);
    return 1;
}

int load_game(const char *p, int b[N][N])
{
    FILE *f = fopen(p, "r");
    if (!f) return 0;

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (fscanf(f, "%1d", &b[r][c]) != 1) {
              fclose(f); return 0;
            }

    fclose(f);
    return 1;
}