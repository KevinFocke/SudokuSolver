
// Sudoku Solver
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Max supported sudoku is 9 x 9
const int MAXDIMENSION = 9; // Max dimension of sudoku
const int MAXARRAY = MAXDIMENSION * MAXDIMENSION; // MAXARRAY is the square of maxdimension.

// Sudoku is represented as a 2D matrix
struct sudoku
{
    int size;
    int rowLength;
    int colLength;
    int **matrix; // 2D matrix (list of pointers to arrays containing digits)
    int **box; // list of pointers to Boxes
};


struct box
{
    int unsolvedCount; // How many elements are currently unsolved?
    int **boxMatrix; // (list of pointers to arrays containing pointers)
};

int printMatrix(int **matrix, int rowLength, int colLength)
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
    int **matrix = (int **)saferCalloc(rowcount, sizeof(int*)); // Dynamically allocate pointers to an array.
    for (int pointer = 0; pointer < rowcount; pointer++) {
        matrix[pointer] = (int *)saferCalloc(colcount, sizeof(int)); // We now have a matrix[row][col] initialized to all zeros.
    }
    convertArrayDimension(sudokuArray, matrix, *dataDimension, *size); // convert 1D to 2D
    printf("Sudoku initialized.\nSize: %d, Length of rows: %d, Length of cols: %d \n", sud->size, sud->colLength, sud->rowLength);
    printf("Sudoku Matrix: \n");
    sud->matrix = matrix;
    printMatrix(sud->matrix, sud->rowLength, sud->colLength);
    return 0;
}

int readFile(char *filename, int *dataCount, int *sudokuArray, int *matrixDimension)
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
            *matrixDimension = i;
            break;
        }
    }

    if (*matrixDimension == 0)
    {
        printf("\n Invalid dataCount. Numbers received: %d. \n Expected a square of a number between 0 - %d. \n",*dataCount,MAXDIMENSION);
        exit(1);
    }

    return 0;
}

int solveSudoku()
{
    // keep track of: steps taken, cycles taken

    return 0;}

int outputSudoku()
{
    // print

    // save to disk


    return 0;}

int main(void){
    //TODO: Enable queuing sudokus
    // Init vars
    char filename[] = "sudoku_input.txt";
    
    // Process per Sudoku

    int dataCount;
    int matrixDimension;
    // Initialize the maximum possible sudoku array; one-dimensional
    int sudokuArray[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null. Bug is -1.
    for (int i = 0; i < MAXARRAY; i++)
    {
        sudokuArray[i] = -1; 
    }

    //Function calls
    if (readFile(filename, &dataCount, sudokuArray, &matrixDimension))
    {
        printf("Failed to read file.");
        exit(1);
    }
    
    // Convert one-dimensional temporary array to 2D matrix in sudoku struct
    struct sudoku *sud = (struct sudoku *) saferCalloc(1, sizeof(struct sudoku)); // initialize sud pointer to struct sudoku
    initSudoku(&dataCount,&matrixDimension, sudokuArray, sud) ;

    // Free temporary variables; converted into struct sudoku

    // solveSudoku
    // printSudoku
    // TODO: Free Malloc
    solveSudoku();
    outputSudoku();
    free(sud);
    
    // implement vertical line checker
    // implement horizontal line checker
    // implement box line checker


    return 0; // main finished succesfully 
}