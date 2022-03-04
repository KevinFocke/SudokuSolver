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

x Implement backtracking algorithm

x BUG: The backtracking algo only looks at the first box. 

x BUG: Solve prints multiple times (because backtrack recursively calls it)

x BUG: The sudTemp does not replace the sud downstream. // Solved by making a deepCopy

x Cleanup checkBox, unused vars matrixRow + matrixCol

x Rename dataCount var to size

x Rename filename to inputFilename

- Allow queuing sudokus (uses Record-Jar Format, "%% \n" for every new sudoku in the .txt file).

Under the seperator, one mandatory keyword called *sudoku* is expected. It represents the sudoku per row with each number space-delimited. Each row is on a new line. Use 0 for empty field, otherwise fill in the value. To improve readability, it's recommended to start the first row on a new line after the *sudoku* keyword.

eg.

sudoku:

0 5 0 7 8 0 0 0 0

9 0 8 2 3 0 7 5 6

2 7 4 6 1 0 0 3 9

0 4 0 9 0 0 0 0 0

0 0 0 5 0 2 0 9 8

0 0 2 0 0 3 1 0 7

0 0 0 0 0 7 0 1 0

4 3 0 0 0 0 9 0 5

1 0 9 3 0 0 0 0 0

If a number is bigger than 10 you can either use the numeric value or English alphabet letter (A is 10, B is 11, F is 15…)

The interpreter keeps reading input until either a new keyword is detected, "%% \n", or the end of file is reached.


Ver 3:
- Start developing on a linux environment
- Add automated test cases via CI
- Add automated security testing 
- Support Sudoku's up to 36 x 36.
- Track bugs outside of the code (github? elsewhere?)
- refactor dataDimension; encapsulate within the sudoku.
- refactor boxHorizontalBound & boxVerticalBound (encapsulate within box)
- refactor outputSudoku to make a .txt with the final sudoku
- feat compare initial sudoku vs end sudoku, highlight changed values in red.

Ver 4:


- feat add optional input keywords; add array of optional keywords encapsulated in struct sudoku:

name: Newspaper_02_03_2022 //the name of the sudoku.

dimensions: 9 x 9 // Format is rowDimension x colDimension. Used as a double check. 

boxmatrix: // Used in snake matrix, for every number include which box it belongs to. To improve the readability of the input file, put the boxmatrix directly under the matrix

source: // where was the sudoku found?

method: // how was the sudoku fetched?

screenshot: // link to a screenshot of the sudoku 

hash: // takes a hash from the matrix and (if defined) the boxmatrix

comment: // optional comment about the sudoku

\# can be used for comments. Everything after the # will not be interpreted on the line. 

- feat keywords also get copied into the output file + records get added "stats" & "solved".

- Create automated benchmarking of algos.
- Add optimized Algo (checks the ROW + COL of diagonal instead of every field)
- Refactor input error detection; using fgets and regex
https://www.quora.com/What-are-some-better-alternatives-to-scanf-in-C-and-what-do-they-do-exactly

- Create fixed-width ints to improve  portability(int32_t instead of int)
- Check for memory leaks (valgrind)

BUG: Fix overcounting of MAXITERATIONS (can go above limit);

Ver 5:
- Add photo mode in Python, make picture of sudoku, translates into matrix
- Make it possible to automatically fetch sudokus from a website.
- Support Samurai Sudoku
- Support Snake Matrix (using boxmatrix record)

Ver 6:
- Add command line recognition of flags, filename=mytext.txt (<argph.h> library)
- Create GUI for sudoku solver
- Precompute sudoku's and their solutions
- Add hint system
- Host online version of the sudoku maker and solver.
- Emscripten, using clang LLVM compiler https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm

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
