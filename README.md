# SudokuSolver

Work-In-Progress, personal experiment to train on pointers, structs, recursion, backtracking & more. 

Feedback welcome!

Program structure:
1. Read in .txt file
2. Initialize sudoku structure
3. Solve sudoku using a chosen algorithm
4. Output the sudoku to terminal.

Design specifications:
- Solves all square sudokus up to 36 x 36.
- Fast and simple to use.
- Can recognize a sudoku in a picture.


Implemented algorithms:
- solidAlgo: For each field, sequentially check the row, check the col, check the box. Only a single possibility in the current field? Fill in.

Called solidAlgo because:
1. It has been tested the most.
2. It does not have excessive bells & whistles.

- solidBacktrackAlgo: Find numbers using solidAlgo. If no further numbers are found, find the most constrained box with >= 1 available fields. Within this field, try each possibility by recursively calling solidBacktrackAlgo until a solution is found. If the possibilities are exhausted & the sudoku remains unsolved, backtrack to an earlier version & try other possibilities.