# SudokuSolver

Work-In-Progress, personal experiment to train on pointers, structs, recursion, backtracking & more. 

Feedback welcome!

Program structure:
1. Read in .txt file
2. Initialize sudoku structure
3. Solve sudoku using a chosen algorithm
4. Output the sudoku to terminal

Implemented algorithms:
- simpleAlgo: For each row & col, check the row, check the col, check the box.
- backtrackAlgo: Find numbers using simpleAlgo. If no further numbers are found, find the most constrained box with >= 1 available fields. Within this field, try each possibility by recursively calling backtrackAlgo until a solution is found. If the possibilities are exhausted & the sudoku remains unsolved, backtrack to an earlier version & try other possibilities.



Lessons learned:

- Be more consistent with casing. CAPITALS for globals. camelCase for local vars.
- Be more granular with commits. One thing at a time. Learn branching + merging for this purpose.

There has to be an easier way to delete THIS:
(int arg1, int arg2, int THIS);

Copy contents in Vim:
int function(VARS TO COPY)
NORMAL pi( // paste inner bracket
NORMAL vibp // replace inside brackets, goes to visual mode, selects inner brackets, pastes.
NORMAL '' // go back to previous place.

Pointers are fun and efficient, but don't overdo. If a pointer is accidentally changed (its value or reference) it can affect the WHOLE downstream.

Problematic: Making a deep copy of a struct with pointers. Can be solved by making a subroutine to copy every element of a struct, and run another subroutine to reconstruct the pointers within it.

Reflect: 
In checkRow & checkCol I wanted to use DRY programming, but found it difficult because you need to access the right element in the matrix. How could I have made it DRY??
