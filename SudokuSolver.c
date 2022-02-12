
// Sudoku Solver

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
    int rowLength;
    int colLength;
    int **dataMatrix;
    };


int readMatrix(int **matrix, int rowLength, int colLength)
{
    for (int row = 0; row < (rowLength); row++)
    {
        printf("|");
       for (int col = 0; col < (colLength); col++)
        {
            printf("%d|", matrix[row][col]);
        }
        printf("\n"); 
    }

    return 0;
}


void *saferCalloc(int numOfElements, int sizeOf)
{
    void *memoryAddress = calloc(numOfElements,sizeOf);
    if (memoryAddress == NULL) // Check pointer
    {
        printf("Calloc failed. Terminating program.");
        exit(1);
    }
    return memoryAddress;
}
int convertArrayDimension(int *onedimensional,  int **matrix, int dataDimension, int datacount){
    int row, col;
    row = col = 0;
    for (int i = 0; i < datacount; i++)
    {
        if (i % (dataDimension) == 0 && i != 0) // problem: i = 0 also counts as 0.
        {
            row += 1;
            col = 0;
        }
        matrix[row][col] = onedimensional[i];
        col += 1;
        // printf("\n i = %d, modulo = %d, datadimension = %d; row = %d, col = %d, onedimensional = %d", i, i % (dataDimension), dataDimension, row, col, onedimensional[i]);
    }
    return 0;
}
int initSudoku(int *size, int *dataDimension, int *sudokuArray,  struct sudoku *sud)
{
    // assign one-dimensional attributes
    sud->size = *size;
    sud->rowLength = sud->colLength = *dataDimension; // square hence same length and width
    // initialize Matrix via array of pointers to arrays
    int rowcount = *dataDimension;
    int colcount = *dataDimension;
    int **matrix = saferCalloc(rowcount, sizeof(int*)); // Dynamically allocate pointers to an array.
    for (int i = 0; i < rowcount; i++) {
        matrix[i] = saferCalloc(colcount, sizeof(int)); // We now have a matrix[row][col] initialized to all zeros.
    }
    convertArrayDimension(sudokuArray, matrix, *dataDimension, *size); // convert 1D to 2D
    printf("Sudoku initialized.\nSize: %d, Length of rows: %d, Length of cols: %d \n", sud->size, sud->colLength, sud->rowLength);
    printf("Sudoku Matrix: \n");
    sud->dataMatrix = matrix;
    readMatrix(sud->dataMatrix, sud->rowLength, sud->colLength);
    return 0;
}

int readFile(char filename[], int *dataCount, int *sudokuArray, int *dataMatrixDimension)
{
    // TODO: Regex to set custom filename
    // readGit source control manager in the file
    FILE *fp;
    fp = fopen(filename, "r");


    *dataCount = 0;
    printf("Scanning %s\n", filename);
    // TODO : Detect non-numeric characters
    int buffer = 0;
    while (fscanf(fp, "%d", &buffer) != EOF){
    if (buffer < 0){
            printf("\n Detected negative number.");
            exit(1);
            }
        sudokuArray[*dataCount] = buffer; // put current int in array
        *dataCount += 1;
    }

    for (int i = 1; i <= MAXDIMENSION; i++)
    {
        if (*dataCount == i * i)
        {
            *dataMatrixDimension = i;
            break;
        }
    }

    if (*dataMatrixDimension == 0)
    {
        printf("\n Invalid dataCount. Numbers received: %d. \n Expected a square of a number between 0 - %d. \n",*dataCount,MAXDIMENSION);
        exit(1);
    }

    return 0;
}

// TODO: Create a seperate function for reading in file
int main(void){
    //TODO: Enable queuing sudokus
    // Init vars
    char filename[] = "sudoku_input.txt";
    int dataCount;
    int dataMatrixDimension;
    // Initialize the maximum possible sudoku array; one-dimensional
    int sudokuArray[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null. Bug is -1.
    for (int i = 0; i < MAXARRAY; i++)
    {
        sudokuArray[i] = -1; 
    }

    //Function calls
    if (readFile(filename, &dataCount, sudokuArray, &dataMatrixDimension))
    {
        printf("Failed to read file.");
        exit(1);
    }
    
    // Convert one-dimensional temporary array to 2D matrix in sudoku struct
    struct sudoku sud;
    initSudoku(&dataCount,&dataMatrixDimension, sudokuArray, &sud) ;

    // Free temporary variables; converted into struct sudoku
    free(sudokuArray);
    free(dataCount);
    free(dataMatrixDimension);


    // TODO: Free Malloc


    // implement vertical line checker
    // implement horizontal line checker
    // implement box line checker


    return 0; // main finished succesfully 
}