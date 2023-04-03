#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int main()
{
    Grid_T g;
    Choice_T c = {0, 0, 0};
    int temp;

    g = sudoku_read();

    temp = sudoku_is_correct(g);
    if (temp == 1)
        printf("Correct\n");
    else
        printf("Incorrect (duplicate in row, column or block in the first sudoku)\n");

    if (g.unique == 1)
        printf("Unique\n");
    else
        printf("Not unique\n");

    sudoku_print(stdout, g);

    printf("\n");

    g = sudoku_solve(g, c);

    printf("Solution:\n");
    sudoku_print(stdout, g);
    return 0;
}