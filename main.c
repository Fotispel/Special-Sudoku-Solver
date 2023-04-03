#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int main()
{
    Grid_T g;
    int temp;

    g = sudoku_read();


    temp = sudoku_is_correct(g);
    if (temp == 1)
        printf("Correct\n");
    else
        printf("Incorrect\n");
    

    if (g.unique == 1)
        printf("Unique\n");
    else
        printf("Not unique\n");

    return 0;
}