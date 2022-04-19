%module SudokuSolverLib
%{
#include "SudokuSolverLib.c"
%}
// Now list ISO C/C++ declarations
int globalint;
int main(void);
int setAndPrint(int myint);