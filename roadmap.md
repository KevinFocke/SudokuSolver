Ver 1 - The basics: 

x Create a program that solves simple sudokus inefficiently. No premature optimization!

x Can read in space-delimited text files.

x Sudoku struct includes 2D Matrix representation. 

Ver 2 - Solid foundations & backtracking: 

x Make extendible with different algorithms.

x Refactor functions to make struct sudoku the primary data representation.

x Implement backtracking algorithm

- Refactor initSudoku (size & dataDimension).

- Refactor struct sudoku (totalUnsolved is redundant because of numbersFoundTotal)

- Refactor initBoxMatrix (current implementation assumes a square box, should allow rectangles in preparation of snake sudoku) 

- Refactor initBoxList (current implementation assumes a square Box)

- Refactor dataDimension; encapsulate within the sudoku.

- Refactor boxHorizontalBound & boxVerticalBound (encapsulate within box)

- Allow queuing sudokus (uses Record-Jar Format, "%% \n" for every new sudoku in the .txt file). Principle: "Be generous in what you accept, rigorous in what you emit."

Under the seperator, one mandatory keyword called *sudoku* is expected. It represents the sudoku per row with each number space-delimited. Each row is on a new line. Use 0 for empty field, otherwise fill in the value. To improve readability, the first row starts on a new line after the *sudoku* keyword.

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

\# can be used for comments. Everything after the # will not be interpreted on the line.

- TODO: Copy this to README.md ^ (formatting under program structure, input formatting)

- Support Sudoku's up to 36 x 36.


Ver 3 - Testing + Documentation:
- Start developing on a linux environment (New WSL window)
- Track issues on Github (possible from terminal?)
- feat - constrainedAlgo, looks at most constrained first.
- Add command line recognition of flags, filename=mytext.txt (<argp.h> library, compiled using gcc, part of glibc library)
- Add automated test cases via CI (Jenkins, Travis CI, Buddy)
- Check for memory leaks (valgrind)
- Add automated security testing (WhiteSource Bolt, Snyk)



Ver 4:

- feat keywords also get copied into the output file + records get added "stats" & "solvedHash".
stats:
// time taken + iteration cycles + backtracking cycles
solvedHash: 
// takes a hash from the solved matrix and (if defined) the boxmatrix
- readFile, prevent octal interpretation of code. (013 being interpreted as 1 * 8 + 3 * 1 = 11)



Ver 5:
- Extending Python with C (https://docs.python.org/2/extending/extending.html). CAPI ? ctypes & CFFI interface?
https://cffi.readthedocs.io/en/latest/overview.html

- Make picture of sudoku, translates into matrix

- feat add optional input keywords; add array of optional keywords encapsulated in struct sudoku:

name: 
Newspaper_Sudoku_02_03_2022 //the name of the sudoku.

boxmatrix: 
// Used in snake matrix, for every number include which box it belongs to. To improve the readability of the input file, put the boxmatrix directly under the matrix

source: 
// where was the sudoku found?

method: 
// how was the sudoku fetched?

screenshot: 
// link to a screenshot of the sudoku 


 
- Make it possible to automatically fetch sudokus from a website.


Ver 6:

- Create GUI for sudoku solver (QT?)
- Support Snake Matrix (using boxmatrix record)
- Add hint system
- Emscripten, using clang LLVM compiler https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm ?