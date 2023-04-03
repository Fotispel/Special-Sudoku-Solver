main.o: main.c grid.h sudoku.h
	gcc -ansi -pedantic -Wall -c main.c

grid.o: grid.c grid.h
	gcc -ansi -pedantic -Wall -c grid.c

sudoku.o: sudoku.c sudoku.h grid.h
	gcc -ansi -pedantic -Wall -c sudoku.c

all: main.o grid.o sudoku.o
	gcc -ansi -pedantic -Wall -o exec main.o grid.o sudoku.o

clean:
	rm main.o grid.o sudoku.o exec