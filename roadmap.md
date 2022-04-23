Ver 1 - The basics: 

x Create a program that solves simple sudokus inefficiently. No premature optimization!

x Can read in space-delimited text files.

x Sudoku struct includes 2D Matrix representation. 

Ver 2 - Solid foundations & backtracking: 

x Make extendible with different algorithms.

x Refactor functions to make struct sudoku the primary data representation.

x Implement backtracking algorithm

Ver 3 - Python Integration

x Program becomes a python program with C integrated for speed.

x Rewrite readFile in Python

x Create seperate folder for C library

x Dynamically recompile using custom makefile

Ver 4 - CLI + Testing + Documentation:

- Implement command-line flags: https://typer.tiangolo.com/ (same developer as FastAPI)

Alternatively:
https://github.com/google/python-fire 

- Automate test cases via CI (Jenkins, Travis CI, Buddy)

- Automate security testing (WhiteSource Bolt, Snyk)

- Check for memory leaks (valgrind)


Opportunities for improvement:
- Support more sudoku types eg. snake sudoku, samurai sudoku, …
- Add photo mode; based on a screenshot of a sudoku, automatically interpret the sudoku & solve it.