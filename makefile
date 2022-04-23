Compiler := gcc
soName := SudokuSolverLib
soOutput := SudokuSolverLib.so
cLibrary := SudokuSolverLib.c

compileFunction:
	${Compiler} -shared -Wl,-soname,${soName} -o ${soOutput} -fPIC ${cLibrary} 
