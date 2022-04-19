# SudokuSolver

Work-In-Progress, personal experiment to train on pointers, structs, recursion, backtracking & more. 

Feedback welcome!

Program structure:
1. Read in .txt file
2. Initialize sudoku structure
3. Solve sudoku using a chosen algorithm
4. Output the sudoku to terminal.

Input format (base10, one-indexed, zero indicates an empty field):
```
0 5 0 7 8 0 0 0 0
9 0 8 2 3 0 7 5 6
2 7 4 6 1 0 0 3 9
0 4 0 9 0 0 0 0 0
0 0 0 5 0 2 0 9 8
0 0 2 0 0 3 1 0 7
0 0 0 0 0 7 0 1 0
4 3 0 0 0 0 9 0 5
1 0 9 3 0 0 0 0 0
```

Design specifications:
x Solves 9 x 9 sudokus
x Efficient algorithm and simple to use.
- Can recognize a sudoku in a picture.


Implemented algorithms:
- robustAlgo: For each field, sequentially check the row, check the col, check the box. Only a single possibility in the current field? Fill in.

Called robustAlgo because:
1. It has been tested the most.
2. It does not have excessive bells & whistles.

- robustBacktrackAlgo: Find numbers using robustAlgo. If no further numbers are found, find the most constrained box with >= 1 available fields. Within this field, try each possibility by recursively calling robustBacktrackAlgo until a solution is found. If the possibilities are exhausted & the sudoku remains unsolved, backtrack to an earlier version & try other possibilities.