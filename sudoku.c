#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "sudoku.h"

Grid_T sudoku_generate(int nelts, int unique)
{
    Grid_T g;
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            g.cell[i][j].choices[0] = rand() % 9 + 1; /* 1 - 9 */
        }
    }
    return g;
}

Grid_T sudoku_read(void)
{
    char *filename = "s.3";
    FILE *fp;
    int i, j, cell[9][9];
    Grid_T g;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    }

    i = 0;
    j = 0;
    while (fscanf(fp, "%d", &cell[i][j]) != EOF)
    {
        /*printf("%d ", cell[i][j]);*/
        j++;

        if (j == 9)
        {
            /*printf("\n");*/
            i++;
            j = 0;
        }
    }
    fclose(fp);
    g = grid_init(g, cell);
    return g;
}

Grid_T sudoku_solve(Grid_T g, Choice_T c)
{
    return g;
}

int sudoku_is_correct(Grid_T g)
{
    int i, j, k;

    /* check if the array cell is complete */
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (g.cell[i][j].choices[0] == 0)
                return 0;
        }
    }

    /* check if the array cell has any duplicates in rows */
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            for (k = 0; k < 9; k++)
            {
                if (g.cell[i][j].choices[0] == g.cell[i][k].choices[0] && j != k)
                    return 0;
            }
        }
    }

    /* check if the array cell has any duplicates in columns */
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            for (k = 0; k < 9; k++)
            {
                if (g.cell[j][i].choices[0] == g.cell[k][i].choices[0] && j != k)
                    return 0;
            }
        }
    }

    /* check if the array cell has any duplicates in 3x3 blocks */
    for (i = 0; i < 9; i = i + 3)
    {
        for (j = 0; j < 9; j = j + 3)
        {
            for (k = 0; k < 9; k++)
            {
                if (g.cell[i + k / 3][j + k % 3].choices[0] == g.cell[i + (k + 1) / 3][j + (k + 1) % 3].choices[0])
                    return 0;
            }
        }
    }

    return 1;
}