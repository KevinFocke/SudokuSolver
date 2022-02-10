
// Super Sudoku Solver

// Header

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <regex.h>
#include <stdlib.h>

// Supported sudoku is 9 x 9
const int MAXDIMENSION = 9; // Max dimension of sudoku
const int MAXARRAY = MAXDIMENSION * MAXDIMENSION; // MAXARRAY is the square of maxdimension.

#define MAX(A,B) ((A)>(B))?(A):(B);
// Sudoku is represented as a matrix
struct sudoku{
    int size;
    int rowlength;
    int collength;
    int *matrix2D[MAXARRAY];
    };

int readFile(char filename[], int *numbercount, int sudoku_array[])
{
    // TODO: Regex to set custom filename
    // readGit source control manager in the file
    FILE *fp;
    printf("%s", filename);
    fp = fopen(filename, "r");



    *numbercount = 0;
    printf("Initializing Max Sudoku array.");
    printf("scanning %s\n", filename);
    // TODO : Detect non-numeric characters
    int buffer = 0;
    while (fscanf(fp, "%d", &buffer) != EOF){
    if (buffer < 0){
            printf("\n Detected negative number. Exiting program.");
            return 1;
            }
        sudoku_array[*numbercount] = buffer; // put current int in array
        *numbercount += 1;
    }

    printf("Scan results: \n");
    for (int i= 0; i < *numbercount; i++)
    {
    printf("%d", sudoku_array[i]);    
    }
    printf("\nCounted %d numbers \n", *numbercount);

    return 0;
}

// TODO: Create a seperate function for reading in file
int main(void){
    //TODO: Enabling queuing sudokus
    // Init vars
    char filename[] = "sudoku_input.txt";
    int numbercount = 0;
    struct sudoku sud;
    // Initialize the maximum possible sudoku array
    int sudoku_array[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null.
    for (int i = 0; i < MAXARRAY; i++)
    {
        sudoku_array[i] = -1; 
    }
    //Function calls
    if (readFile(filename, &numbercount, sudoku_array))
    {
        return 1;
    }
    // Set sudoku
    sud.size = numbercount;
    sud.collength = sud.rowlength = MAXDIMENSION; //square dimensions



    // Argument: struct sudoku *sud
    // Count the size of the input. Does it match known dimensions?

    int dataMatrixDimension = 0; // Figure out the square dimension of the data
    for (int i = 1; i <= MAXDIMENSION; i++)
    {
        if (numbercount == i * i)
        {
            dataMatrixDimension = i;
            printf("The data dimension is %d x %d \n", dataMatrixDimension, dataMatrixDimension);
            break;
        }
    }

    if (dataMatrixDimension == 0)
    {
        printf("Invalid numbercount; does not match square dimensions. Numbers received: %d.", numbercount);
        return 1;
    }
    // TODO: Free Malloc


    // implement vertical line checker
    // implement horizontal line checker
    // implement box line checker


    /* TODO: Refactor into subroutines
    readData();
    makeSudoku(DataMatrixDimension, );
    solveSudoku();
    */


    return 0; // main finished succesfully 
}