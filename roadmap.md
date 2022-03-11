Ver 1 - The basics: 

x Create a program that solves simple sudokus inefficiently. No premature optimization!

x Can read in space-delimited text files.

x Sudoku struct includes 2D Matrix representation. 

Ver 2 - Solid foundations & backtracking: 

x Make extendible with different algorithms.

x Refactor functions to make struct sudoku the primary data representation.

x Implement backtracking algorithm

Ver 3 - Testing + Documentation:

- Add command line recognition of flags, filename=mytext.txt (<argp.h> library, compiled using gcc, part of glibc library)
- Automate test cases via CI (Jenkins, Travis CI, Buddy)
- Automate security testing (WhiteSource Bolt, Snyk)
- Check for memory leaks (valgrind)



Personal goals:
- Develop from linux environment (remote window to WSL2)
- Start tracking issues on Github (possible from terminal?)


Ver 4:
- Add photo mode; based on a screenshot of a sudoku, automatically interpret the sudoku & solve it.

Personal goals:

- Extend Python with C (https://docs.python.org/2/extending/extending.html). CAPI ? ctypes & CFFI interface? 
https://cffi.readthedocs.io/en/latest/overview.html

(How to pass Python OCR reads to standard input?)



Opportunities for improvement:
- Replace scanf in the readFile function with a more robust implementation. eg. Preventing accidental octal interpretation of the code. (013 being interpreted as 1 * 8 + 3 * 1 = 11).
- Increase testing input cases.
- Support more sudoku types eg. snake sudoku, samurai sudoku, …