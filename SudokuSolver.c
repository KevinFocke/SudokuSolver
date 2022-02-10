
// Super Sudoku Solver

// Header

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <regex.h>
#include <stdlib.h>
// Supported sudoku is 9 x 9
int MAXWIDTH = 9; // Max width of sudoku;
int MAXLENGTH = 9; // Max length of sudoku
#define MAX(A,B) ((A)>(B))?(A):(B);

struct sudoku{
    int* dataMatrix;
     // a 9 x 9 matrix contains 3 boxes on top, 3 boxes in middle, 3 in bottom
    int dimension;
    int numberCount;
    };

int checkDimensions(int *dimension1, int *dimension2)
{
    // Are the dimensions square?
    if (*dimension1 == *dimension2)
    {
        return 0;
    }
    else
    {
        printf("Dimension weirdness. Sudoku's should have square dimensions \n Dim 1: %d\n Dim 2: %d \n", *dimension1, *dimension2);
        return 1;
        //     char filename[] = "sudoku_input.txt";return 1;
    }
}

int readFile(char filename[], int *maxdimension, int *numbercount, struct sudoku *sud)
{
    // TODO: Regex to set custom filename
    // readGit source control manager in the file
    FILE *fp;
    printf("%s", filename);
    fp = fopen(filename, "r");

    // Calc max possible array
    int MAXARRAY = MAXWIDTH * MAXLENGTH;
    *maxdimension = 0;
    if (MAXWIDTH == MAXLENGTH){
        *maxdimension = MAXWIDTH;
    }

    *numbercount = 0;
    printf("Initializing Max Sudoku array.");
    int sudoku_array[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null.
    for (int i = 0; i < MAXARRAY; i++)
    {
        sudoku_array[i] = -1; 
    }
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
    // Check globals
    if (checkDimensions(&MAXLENGTH,&MAXWIDTH))
    {
        return 1;
    };

    // Init vars
    char filename[] = "sudoku_input.txt";
    int maxdimension, numbercount = {0};
    //TODO: Enabling queuing sudokus
    struct sudoku sud; 
    // *psud = &sud; // create a sud, and a pointer to it
    //Function calls
    if (readFile(filename, &maxdimension, &numbercount, &sud))
    {
        return 1;
    }
    // Count the size of the input. Does it match known dimensions?

    int DataMatrixDimension = 0; // Figure out the square dimension of the data
    for (int i = 1; i <= maxdimension; i++)
    {
        if (numbercount == i * i)
        {
            DataMatrixDimension = i;
            printf("The data dimension is %d x %d \n", DataMatrixDimension, DataMatrixDimension);
            break;
        }
    }

    if (DataMatrixDimension == 0)
    {
        printf("Invalid numbercount. Numbers received: %d.", numbercount);
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