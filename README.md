# SudokuSolver

Work-In-Progress, personal experiment to train on pointers, structs, recursion, backtracking & more. 

Feedback welcome!

Program structure:
1. Read in .txt file
2. Initialize sudoku structure
3. Solve sudoku using a chosen algorithm
4. Output the sudoku to terminal.

Design specifications:
- Solves all sudokus up to 36 x 36.
- Simple to use. Easy to extend.
- Can recognize a sudoku in a picture.


Implemented algorithms:
- simpleAlgo: For each field, check the row, check the col, check the box. Only 1 possibility in the current field? Fill in.
- backtrackAlgo: Find numbers using simpleAlgo. If no further numbers are found, find the most constrained box with >= 1 available fields. Within this field, try each possibility by recursively calling backtrackAlgo until a solution is found. If the possibilities are exhausted & the sudoku remains unsolved, backtrack to an earlier version & try other possibilities.