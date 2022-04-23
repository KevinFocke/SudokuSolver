import ctypes
import math

inputFilePath = "Input_Cases/Individual/sudoku_input_difficult.txt"
algoChoice = 1 # defaults to backtrack algo

SudokuSolverLib = ctypes.CDLL('./SudokuSolverLib.so') 


def readSudInput(filepath):
    # helper function ,reads in .txt

    sud_sizes =  [el ** 2 for el in range(1, 17)] # square dimensions for sudokus
    text = ""
    sud_array = []
    with open(filepath, "r") as file:
        text = file.read()
    for el in text:
        if el.isdigit():
            sud_array.append(int(el))
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
    
    return [sud_array, len(sud_array), sud_dimension]

def convertToCArray(array, elementCount):

    # explicitly converts array to 32 bit ints

    cArray= (ctypes.c_int32 * elementCount)(*array)
    # * array accesses the contents of the list, space-seperated
    
    return cArray


def convertToCParameters(filepath, algoChoice):

    # create cArray
    array, arr_len, sud_dimension = readSudInput(filepath)
    cArray = convertToCArray(array, arr_len)

    # create 32 bit c ints
    def createCInts(*args):
        converted_ints = []
        for arg in args:
            converted_ints.append(ctypes.c_int32(arg))
        return converted_ints

    size, dataDimension = [arr_len,sud_dimension]

    cAlgoChoice, cSize, cDataDimension = createCInts(algoChoice, size, dataDimension)

    return [cAlgoChoice, cSize, cDataDimension, cArray]
    
        
args = convertToCParameters(filepath=inputFilePath,algoChoice = algoChoice)

print(args)
# int startSudoku(int algoChoice, int size, int dataDimension, int *sudokuArray){
    
# None, integers, bytes objects and (unicode) strings can be passed directly as parameters
# However, the types are passed explicitly to avoid potential bugs.

# The arguments should be provided positionally as in C!

SudokuSolverLib.startSudoku(*args) 

# Figure out how to pass array


#inputFilename = "Input_Cases/Individual/sudoku_input_difficult.txt"
#algoChoice = 1 ## The default algorithm is backtracking
## size = 0 ## total amount of numbers
##dataDimension = 0 ## Length of one side of a sudoku
##sudokuArray ## unsolved sudokus are zero. Unfilled sudoku elements are null. Bug value is -1.

