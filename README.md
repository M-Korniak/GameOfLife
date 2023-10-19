# GameOfLife
Program that simulates Game of Life

# How does it work?

The program simulates a world of cells following the rules of "Game of Life."

The simulation starts with a given initial generation. In the next generation, a cell will be alive if and only if:

In the current generation, it is alive and has exactly two or three live neighbors.
In the current generation, it is dead and has exactly three live neighbors.
The program reads the description of the initial generation. In a loop, it displays a portion of the board and then reads and executes user commands. These commands control the calculation of subsequent generations and define a window on the board.

The program recognizes the following commands:

- '.' - End the program.
- N (a positive integer) - Calculate the N-th generation.
- An empty line - Calculate the next generation.
- '0' - Generate a description of the current generation.
- 'w' 'k' - Change the coordinates of the upper-left corner of the window to row w and column 
