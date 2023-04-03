#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "sudoku.h"

int check_row(Grid_T g, int row, int value);
int check_column(Grid_T g, int col, int value);
int check_block(Grid_T g, int row, int col, int value);
void print_grid(Grid_T g);

Grid_T sudoku_generate(int nelts, int unique)
{
    Grid_T g;
    int i, j, k;

    int count_nelts = 0;
    int place, place_row, place_col;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            g.cell[i][j].choices[0] = 0;
            g.cell[i][j].count = 0;
        }
    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (count_nelts < nelts)
            {
                place = rand() % nelts + 1; /* 1 - nelts */
                place_row = place / 9;
                place_col = place % 9;

                if (g.cell[place_row][place_col].choices[0] == 0)
                {
                    g.cell[place_row][place_col].choices[0] = rand() % 9 + 1; /* 1 - 9 */
                    count_nelts++;

                    for (k = 1; k < 10; k++)
                        g.cell[i][j].choices[k] = 0;
                }
            }
            else
                g.cell[i][j].choices[0] = 0;
        }
    }
    return g;
}

Grid_T sudoku_read(void)
{
    char *filename = "s.6";
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

void sudoku_print(FILE *fp, Grid_T g)
{
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fprintf(fp, "%d ", g.cell[i][j].choices[0]);
        }
        fprintf(fp, "\n");
    }
}

Grid_T sudoku_solve(Grid_T g, Choice_T c)
{
    static int count = 0;
    int i, j;

    /* Case: sudoku is unique */
    if (g.unique == 1)
    {
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                if (g.cell[i][j].choices[0] == 0 && g.cell[i][j].count == 0)
                {
                    printf("It has not any solution\n");
                    exit(1);
                }

                c = grid_iterate(g, c);

                if (g.cell[c.i][c.j].choices[0] == 0)
                {
                    g = grid_update(g, c);
                }
            }
        }
        return g;
    }

    /* Case: sudoku is not unique */
    if (c.j == 9)
    {
        c.i++;
        c.j = 0;
    }

    if (g.cell[c.i][c.j].choices[0] != 0)
    {
        c.j++;
        return sudoku_solve(g, c);
    }

    for (c.n = 1; c.n <= 9; c.n++)
    {
        if (check_row(g, c.i, c.n) == 1 && check_column(g, c.j, c.n) == 1 && check_block(g, c.i, c.j, c.n) == 1)
        {
            g.cell[c.i][c.j].choices[0] = c.n;
            c.j++;

            while (sudoku_is_correct(g) == 0)
            {
                count++;
                if (count == 1000)
                {
                    printf("It has not any solution\n");
                    exit(1);
                }
                g = sudoku_solve(g, c);
            }

            return g;
        }

        g.cell[c.i][c.j].choices[0] = 0;
    }
    return g;
}

int sudoku_is_complete(Grid_T g)
{
    int i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (g.cell[i][j].choices[0] == 0)
                return 0;
        }
    }
    return 1;
}

int sudoku_is_correct(Grid_T g)
{
    int i, j, k, l;

    /* check if the array cell is complete */
    if (sudoku_is_complete(g) == 0)
        return 0;

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
            for (k = i - i % 3; k < i - i % 3 + 3; k++)
            {
                for (l = j - j % 3; l < j - j % 3 + 3; l++)
                {
                    if (g.cell[i][j].choices[0] == g.cell[k][l].choices[0] && i != k && j != l)
                        return 0;
                }
            }
        }
    }

    return 1;
}