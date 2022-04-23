import ctypes

SudokuSolverLib = ctypes.CDLL('./SudokuSolverLib.so') 

cAlgoChoiceValue = ctypes.c_int32(1) #Defaults to option 1, see algo overview in Readme

# None, integers, bytes objects and (unicode) strings can be passed directly as parameters
# However, the types are passed explicitly to avoid potential bugs.


# Careful! The arguments should be provided positionally as in C!

SudokuSolverLib.startSudoku(cAlgoChoiceValue)

# Figure out how to pass array


#inputFilename = "Input_Cases/Individual/sudoku_input_difficult.txt"
#algoChoice = 1 ## The default algorithm is backtracking
## size = 0 ## total amount of numbers
##dataDimension = 0 ## Length of one side of a sudoku
##sudokuArray ## unsolved sudokus are zero. Unfilled sudoku elements are null. Bug value is -1.

