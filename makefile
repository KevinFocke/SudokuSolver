Compiler := gcc
soName := SudokuSolverLib
soOutput := ./lib/SudokuSolverLib.so
cLibrary := ./lib/SudokuSolverLib.c

compileFunction:
	${Compiler} -shared -Wl,-soname,${soName} -o ${soOutput} -fPIC ${cLibrary} 
