import ctypes
import math

inputFilePath = ""
SudokuSolverLib = ctypes.CDLL('./SudokuSolverLib.so') 

test_path = "Input_Cases/Individual/sudoku_input_difficult.txt"


def readSudInput(filepath):
    sud_sizes =  [el ** 2 for el in range(1, 17)] # square dimensions for sudokus
    text = ""
    sud_array = []
    with open(filepath, "r") as file:
        text = file.read()
    for el in text:
        if el.isdigit():
            sud_array.append(el)
    if len(sud_array) not in sud_sizes:
        print(f"unexpected amount of sudoku entries, got {len(sud_array)}, expected a value in: \n {sud_sizes}") 
        quit("quitting program")

    # calc dimensions
    sud_dimension = round(math.sqrt(len(sud_array)))

    # check if each number falls within this dimension, 0 means no value
    pos = 0
    for field in sud_array:
        if not (0 <= int(field) <= int(sud_dimension)):
            print(f"Unexpected value: {field} at pos {pos} \n")
            quit(f"Expected a number between 0 and {sud_dimension} based on calculated dimensions. Quitting.")
        pos += 1
    
    return sud_array

print(readSudInput(test_path))
    

#cAlgoChoiceValue = ctypes.c_int32(1) #Defaults to option 1 (robustBacktrackAlgo), see algo overview in readSudInputme

# None, integers, bytes objects and (unicode) strings can be passed directly as parameters
# However, the types are passed explicitly to avoid potential bugs.

# The arguments should be provided positionally as in C!

#SudokuSolverLib.startSudoku(cAlgoChoiceValue)

# Figure out how to pass array


#inputFilename = "Input_Cases/Individual/sudoku_input_difficult.txt"
#algoChoice = 1 ## The default algorithm is backtracking
## size = 0 ## total amount of numbers
##dataDimension = 0 ## Length of one side of a sudoku
##sudokuArray ## unsolved sudokus are zero. Unfilled sudoku elements are null. Bug value is -1.

