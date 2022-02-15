
// Sudoku Solver
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h> 

// Max supported sudoku is 9 x 9
const int MAXDIMENSION = 9; // Max dimension of sudoku
const int MAXITERATIONS = 81;
const int MAXARRAY = MAXDIMENSION * MAXDIMENSION; // MAXARRAY is the square of maxdimension.

// Sudoku is represented as a 2D matrix
struct sudoku
{
    int size;
    int rowLength;
    int colLength;
    int boxWidth; // Not compatible with snake matrix
    int totalUnsolved;
    int initialUnsolved;
    int **matrix; // 2D matrix, list of pointers to arrays containing digits
    struct box **boxList; // list of pointers to Boxes, filling from top-left to right;
};

//BUG: Potential bug in **boxlist pointer logic


struct box
{
    int unsolvedCount; // How many elements are currently unsolved?
    int ***pointerMatrix; // (list of pointers to arrays containing pointers)
};

int printMatrix(int **matrix, int rowLength, int colLength, int highlightRow, int highlightCol)
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

int initBoxMatrix (struct box *box, struct sudoku *sud, int boxPosVertical, int boxPosHorizontal, int boxWidth)
{
    /* Terminology: boxPos refer to the position of the box within the matrix.
    Needed for initializing pointers to elements.
    
    a 9 x 9 Matrix will have 9 boxes:
    B B B
    B B B   (the first B in this row has boxPosVertical 0, boxPosHorizontal 1)
    B B B

    boxRow & boxCol refer to the cols and row INSIDE a box.

    Delving into box:
    sud->boxList[1][0]->boxMatrix:
    P P P
    P P P
    P P P
    */

   // Memory allocation
   int ***tempBoxMatrix = (int ***)saferCalloc(boxWidth, sizeof(int**));
   for (int i = 0; i < (boxWidth); i++)
   {
       tempBoxMatrix[i] = (int**)saferCalloc(boxWidth, sizeof(int*));
   }

    // Assign pointers + calculate unsolvedcount
    int tempUnsolved = sud->colLength;

   for (int row = 0; row < boxWidth; row++)
   {
       for (int col = 0; col < boxWidth; col++)
       {
           tempBoxMatrix[row][col] = &(sud->matrix[row+(boxPosVertical*3)][col+(boxPosHorizontal*3)]);
            int curval = *(tempBoxMatrix[row][col]);
            if (curval != 0)
            {
                tempUnsolved -= 1;
            }

       }
   }
    box->pointerMatrix = tempBoxMatrix;
    box->unsolvedCount = tempUnsolved;

    return 0;
}

int initBoxList(int dataDimension, struct sudoku *sud)
{  
    int boxWidth = floor(sqrt((double) dataDimension));
    sud->boxWidth = boxWidth;
    if ((boxWidth * boxWidth) != dataDimension)
    {
        printf("BoxWidth sqrt flooring failed to produce round number.");
        exit (1);
    }
    
    //printf("\n dataDimension is %d, Boxwidth is:%d",dataDimension, boxWidth);

    // Init boxList
    struct box**tempBoxList = (struct box**)saferCalloc(boxWidth, sizeof(struct box*)); 
    for (int i = 0; i < boxWidth; i++) // create boxList and assign pointer to a struct box
    {
        tempBoxList[i] = (struct box*)saferCalloc(boxWidth, sizeof(struct box));
    }
    for (int row = 0; row < boxWidth; row++)
    {
        for (int col = 0; col < boxWidth; col++)
        {
        struct box tempBox;
        tempBox.unsolvedCount = MAXDIMENSION+1; // TODO: Check for errors in unsolvedCount logic by searching MAXDIMENSION+1
        initBoxMatrix(&tempBox,sud, row,col, boxWidth);
        tempBoxList[row][col] = tempBox;
        }
    }
    sud->boxList = tempBoxList;
    // printf("\nUnsolved count: %i", sud->boxList[0][2].unsolvedCount);
    //printMatrix(*(sud->boxList[0][0].pointerMatrix), 3,3);

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
    for (int i = 0; i < rowcount; i++) {
        matrix[i] = (int *)saferCalloc(colcount, sizeof(int)); // We now have a matrix[row][col] initialized to all zeros.
    }
    convertArrayDimension(sudokuArray, matrix, *dataDimension, *size); // convert 1D to 2D
    printf("Sudoku initialized.\nSize: %d, Length of rows: %d, Length of cols: %d \n", sud->size, sud->colLength, sud->rowLength);
    printf("Sudoku Matrix: \n");
    sud->matrix = matrix;
    printMatrix(sud->matrix, sud->rowLength, sud->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    // Init boxStructure
    initBoxList(*dataDimension, sud);

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

int checkRow(struct sudoku *sud, int number, int matrixRow)
{
   /* If you check row, the row is fixed, col variablet & vice versa. */ 

    for (int varDimension = 0; varDimension < sud->colLength; varDimension++) // iterate over cols
    {
        if (sud->matrix[matrixRow][varDimension] == number)
        {
            return 1; // Found matching number
        }
    }

    return 0; // No result found
}

int checkCol(struct sudoku *sud, int number, int matrixCol)
{
   /* If you check row, the row is fixed, col variablet & vice versa. */ 

    for (int varDimension = 0; varDimension < sud->rowLength; varDimension++) // iterate over rows
    {
        if (sud->matrix[varDimension][matrixCol] == number)
        {
            return 1; // Found matching number
        }
    }

    return 0; // No result found
}

int checkBox(struct sudoku *sud, int number, int matrixRow, int matrixCol, int currentBoxHorizontal, int currentBoxVertical, int horizontalBound, int verticalBound)
{
    for (int row = 0; row < verticalBound; row ++)
    {
        for (int col = 0; col < horizontalBound; col++)
        {
            
            if (*(sud->boxList[currentBoxVertical][currentBoxHorizontal].pointerMatrix[row][col]) == number)
            {
                return 1; // found matching number
            }
        }
    }
    return 0; // found no match
}

int solveSudoku(struct sudoku *sud, int iterations)
{
    // Count unsolved entries
    int totalUnsolved = 0;
    for(int row = 0; row < sud->boxWidth; row++)
    {
        for (int col = 0; col < sud->boxWidth; col++)
        {
            totalUnsolved += sud->boxList[row][col].unsolvedCount;
        }
    
    }
    sud->totalUnsolved = totalUnsolved;
    sud->initialUnsolved = totalUnsolved;
    
    // Check each number
    // NOT extendible to bigger sudokus or different formats; what if rowLength is largest dimension?
    int boxHorizontalBound = floor(sqrt((double)sud->colLength));
    int boxVerticalBound = floor(sqrt((double)sud->rowLength));
    
    for (int i = 0; i < iterations; i++)
    {
        int amountFoundInIteration = 0; // How many numbers were found this iteration?
        printf("Current iteration: %i \n", i+1);
    
        int currentBoxHorizontal = 0;
        int currentBoxVertical= 0;
        for (int row = 0; row < sud->rowLength; row++)
        {
            if (row != 0 && ((row % boxVerticalBound) == 0)) // Find out when box changes vertically
            {
                currentBoxVertical += 1;
            }
            // Keep track of horizontal box position
            currentBoxHorizontal = 0; // reset current horizontal box at every new row


            for (int col = 0; col < sud->colLength; col++) // iterate over cols
            {
            if (col != 0 && ((col % boxHorizontalBound) == 0)) // Find out when box changes horizontally
            {
                currentBoxHorizontal += 1;
            }

            // Allocate memory for possibilities & initialize max possibilites.
            int* posArray = (int*) saferCalloc(sud->colLength + 1,sizeof(int));
            int posCounter = sud->colLength; // How many possibilities are there?
                for (int number = 1; number <= sud->rowLength; number++)
                {   // For every row & col, check whether number appears in other place than itself
                    if (sud->matrix[row][col] != 0)
                    {
                        break; // Already a value present
                    }
                    else if (checkRow(sud, number, row) == 1) // found matching number
                    {
                        posArray[number] = MAXDIMENSION+1; //posArray is one-indexed for consistency with number
                        posCounter -= 1;
                        continue;
                    }
                    else if (checkCol(sud, number, col) == 1) // found mathcing number   
                    {
                        posArray[number] = MAXDIMENSION+1;
                        posCounter -= 1;
                        continue;
                    }

                    else if (checkBox(sud, number, row, col, currentBoxHorizontal, currentBoxVertical, boxHorizontalBound, boxVerticalBound) == 1) // found matching number
                    {
                        posArray[number] = MAXDIMENSION+1;
                        posCounter -= 1;
                        continue;
                    } // remember: Potential bug in pointer arithmetic
                }

            // Check if there is a single solution possible: 
            if (posCounter == 1)
            {
                for (int number = 1; number < sud->rowLength; number++) //check for every number
                {
                    if (posArray[number] == 0)
                    {
                        printf("Found number: %i at row %i col %i | ", number, row+1, col+1);
                        sud->matrix[row][col] = number;
                        printf("\n");
                        printMatrix(sud->matrix,sud->rowLength, sud->colLength,row,col);
                        amountFoundInIteration += 1;
                        sud->totalUnsolved -= 1;
                        break; // break the current loop
                    }
                }
            }
            free(posArray);
            }
        }
    if (amountFoundInIteration == 0)
    {
        printf("Algorithm finds no more numbers. \n");
        break;
    }
    printf("Amount of numbers found this iteration: %i \n", amountFoundInIteration);
    }


    return 0;
}

int outputSudoku(struct sudoku *sud)
{
    if (sud->totalUnsolved == 0)
    {
    printf("\nSudoku Solved:\n");
    printMatrix(sud->matrix,sud->rowLength,sud->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    }
    else
    {
        printf("Solution not found. Solved %i out of %i\n current Matrix:\n", (sud->initialUnsolved) - (sud->totalUnsolved), sud->initialUnsolved);
        printMatrix(sud->matrix,sud->rowLength,sud->colLength, MAXDIMENSION+1,MAXDIMENSION+1); 
    }

    // save to disk


    return 0;}

int main(void){
    //TODO: Enable queuing sudokus
    // Init vars
    char filename[] = "sudoku_input_simple.txt";
    
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
    solveSudoku(sud, MAXITERATIONS);
    outputSudoku(sud);
    free(sud);

    return 0; // main finished succesfully 
}