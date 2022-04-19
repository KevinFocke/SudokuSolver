%module SudokuSolverLib

%{
#include "SudokuSolverLib.c"
%}

// Now list ISO C/C++ declarations
int startSudoku(int algoChoice);
