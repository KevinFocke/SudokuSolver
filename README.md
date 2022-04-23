# SudokuSolver

## General information
This was a project-based learning experience. Skills I practiced:
- Recursion (Backtracking)
- Integrating Python & C using cTypes
- Calculating Big O complexity
- Pointers
- Structs

## Usage Information

Usage steps:
1. Prepare a .txt file based on the input format
2. Add the path to your .txt file in SudokuSolver.py
3. Run SudokuSolver.py using a Python 3 interpreter.

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

## Technical information

The program uses a combination of Python & C. Python for text interpretation, C for solving with speed. cTypes are used for the glue layer between the languages.

Program structure:
1. Read in .txt file
2. Initialize sudoku structure
3. Solve sudoku using a chosen algorithm
4. Output the sudoku to terminal.

Design specifications:
x Solves 9 x 9 sudokus
x Efficient algorithm and simple to use.

Implemented algorithms:
1. robustAlgo: For each field, sequentially check the row, check the col, check the box. Only a single possibility in the current field? Fill in.

Pros:
- It has been tested the most
- No excessive bells & whistles.
Cons:
- Cannot find all solutions because it does not backtrack.

Algorithmic complexity analysis:
There are n rows & columns. In theory, the worst case scenario is that you need to check the row, col & box for every field in the sudoku while only finding 1 number per pass. If a solution is found a search is conducted on the possArray. Thus the theoretical algorithmic complexity is: n passes * n rows * n cols * n checkRow * n checkCol * n checkBox * n possSearch. Thus the theoretical big O is n<sup>7</sup>. Yikes. 

However, in practice the algorithm works significantly faster for several reasons:
- A sudoku does not start off blank. A 9 x 9 sudoku needs at minimum 17 fields filled in to have a unique solution.
(Source: https://www.technologyreview.com/2012/01/06/188520/mathematicians-solve-minimum-sudoku-problem/)
- The algorithm is optimized to avoid redundant steps. eg If a value is already present in a field it will skip that field.

This algorithm could be further improved by persisting the possibilities per field and avoiding linear search on the possArray. However, this would require more space and add unneeded complexity into the program.

2. robustBacktrackAlgo: Find numbers using robustAlgo. If no further numbers are found, find the most constrained box with >= 1 available fields. Within this field, try each possibility by recursively calling robustBacktrackAlgo until a solution is found. If the possibilities are exhausted & the sudoku remains unsolved, backtrack to an earlier version & try other possibilities.

Pros: 
- Can find all solutions because it exhaustively explores the search space
- Fast for 9 x 9 sudokus
Cons:
- Slow for anything above 9 x 9 sudokus because of combinatorial explosion.

Algorithmic complexity analysis:
robustBacktrackAlgo uses the robustAlgo under the hood. It exhaustively searches the possibilities.
Theoretically, the algorithmic complexity is: O( (row * col)! * n<sup>7</sup>) = O( (n * n)! * n)
In practice, the algorithmic complexity is lower because of the reasons explained in the analysis of robustAlgo. Additionally, the algorithm is optimized by first looking at the most constrained box.