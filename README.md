# SudokuSolver

Work-In-Progress, personal experiment to train on pointers, structs, recursion & backtracking. Feedback welcome!

Ver 1: 

x Create a program that solves simple sudokus inefficiently.

x Refactor code into multiple subroutines

x Can read in space-delimited text files.

x Sudoku struct including 2D Matrix representation. 

x Exit in case of error instead of return 1.

x Initialize a box structure (eg 3 x 3 box in a 9 x 9 matrix)

x Refactor readMatrix into own function

x Double check for pointer bug in box structure.

x Can solve simple sudoku

x BUG: Does not find 9's

Ver 2: 

x Make extendible with different algorithms.

- Solve cases with 2 or more possible fields 

e.g.

9 5 0 0 0 0 0 0 0

7 0 0 4 0 0 0 0 0

0 1 4 0 2 7 0 0 0

0 4 0 0 0 1 6 8 0

5 0 0 0 0 0 0 0 2

0 7 6 8 0 0 0 4 0

0 0 X 3 5 0 2 6 0       solution: X is 7

0 0 0 0 0 0 0 1 9

- Implement backtracking algorithm
BUG: The backtracking algo only looks at the first box. If it can't find by quessing the first box, it won't find anything more!
BUG: Solve prints twice (because backtrack calls it) & 

x BUG: The sudTemp does not replace the sud downstream. // Solved by making a deepCopy

- Add command line recognizition of flags, filename=mytext.txt 

- Refactor input error detection; using fgets and regex
https://www.quora.com/What-are-some-better-alternatives-to-scanf-in-C-and-what-do-they-do-exactly

- Allow queuing sudokus
- Check for memory leaks (valgrind)

Ver 3:
- Add test cases
- Increase the portability of the code
- Refactor code to make more extendible.

Ver 4:
- Host online version of the sudoku maker and solver.
- Add cloud computing support

Ver 5:
- Add photo mode, make picture of sudoku, translates into matrix
- Make it possible to automatically fetch sudokus from a website.
- Convert ints to a shorter type depending on needed number range?

*/

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

Pointers are fun and efficient, but don't overdo. If a pointer is accidentally changed (its value or reference) it can effect the WHOLE downstream.

Problematic: Making a deep copy of a struct with pointers. Can be solved by making a subroutine to copy every element of a struct, and run another subroutine to reconstruct the pointers within it.

Reflect: 
In checkRow & checkCol I wanted to use DRY programming, but found it difficult because you need to access the right element in the matrix. How could I have made it DRY??
