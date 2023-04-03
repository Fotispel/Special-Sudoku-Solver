#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "grid.h"

int check_row(Grid_T g, int row, int value)
{
    int j;

    /* check if the row has already the value "value" */
    for (j = 0; j < 9; j++)
    {
        if (g.cell[row][j].choices[0] == value)
            return 0;
    }
    return 1;
}

int check_column(Grid_T g, int col, int value)
{
    int i;

    /* check if the column has already the value "value" */
    for (i = 0; i < 9; i++)
    {
        if (g.cell[i][col].choices[0] == value)
            return 0;
    }
    return 1;
}

int check_block(Grid_T g, int row, int col, int value)
{
    int i, j;

    /* check if the array cell has any duplicates in blocks */
    for (i = row; i < row + 3 - row % 3; i++)
    {
        for (j = col; j < col + 3 - col % 3; j++)
        {
            if (g.cell[i][j].choices[0] == value)
                return 0;
        }
    }
    return 1;
}

Grid_T grid_init(Grid_T g, int v[9][9])
{
    int i, j, value;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            g.cell[i][j].choices[0] = v[i][j];
            g.cell[i][j].count = 0;
        }
    }

    /* initialize the choices */
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (g.cell[i][j].choices[0] == 0)
            {
                for (value = 1; value < 10; value++)
                {
                    if (check_row(g, i, value) && check_column(g, j, value) && check_block(g, i - i % 3, j - j % 3, value))
                    {
                        g.cell[i][j].choices[value] = 1;
                        g.cell[i][j].count++;
                    }
                }
            }
            else
            {
                for (value = 1; value < 10; value++)
                    g.cell[i][j].choices[value] = 0;
            }
        }
    }

    g.unique = grid_unique(g);
    return g;
}

Grid_T grid_update(Grid_T g, Choice_T c)
{
    g.cell[c.i][c.j].choices[0] = c.n;
    g.cell[c.i][c.j].choices[c.n] = 0;

    if (g.cell[c.i][c.j].count > 0)
        g.cell[c.i][c.j].count--;

    return g;
}

Choice_T grid_iterate(Grid_T g, Choice_T t)
{
    int i, j, k;
    Choice_T choice = {0, 0, 0};
    for (i = t.i; i < 9; i++)
    {
        for (j = t.j; j < 9; j++)
        {
            for (k = t.n + 1; k <= 9; k++)
            {
                if (g.cell[i][j].choices[k] == 1 && g.cell[i][j].choices[0] == 0)
                {
                    choice.i = i;
                    choice.j = j;
                    choice.n = k;
                    return choice;
                }
            }
            t.n = 0;
        }
    }
    return choice;
}

int grid_unique(Grid_T g)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (g.cell[i][j].count > 1)
                return 0;
        }
    }
    return 1;
}

Choice_T grid_exist_unique(Grid_T g)
{
    int i, j;
    Choice_T c = {0, 0, 0};
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (g.cell[i][j].count == 0) /*if the cell has a unique choice in position 0*/
            {
                c.i = i;
                c.j = j;
                c.n = g.cell[i][j].choices[0];
                return c;
            }
        }
    }
    return c;
}

Grid_T grid_clear_unique(Grid_T g)
{
    g.unique = 2;
    return g;
}

Choice_T grid_read_value(Grid_T g, Choice_T c)
{
    int value;
    Choice_T choice;

    value = g.cell[c.i][c.j].choices[0];

    choice.i = c.i;
    choice.j = c.j;
    choice.n = value;

    return choice;
}

void gr_print(Grid_T g)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            printf("%d ", g.cell[i][j].choices[0]);
        }
        printf("\n");
    }
}