// Sudoku Solver 

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

const int MAXDIMENSION = 16; // Max dimension of sudoku. >9 is experimental & known to be combinatorially explosive. In other words, it scales exponentially and slows down tremendously with scale.
const int MAXITERATIONS = 100000000;  // Max iterations per sudoku
const int MAXARRAY = MAXDIMENSION * MAXDIMENSION; // Used to allocate memory for matrix array.

struct sudoku
{
    int size; // total amount of numbers
    int dataDimension; /* square matrix assumed, dataDimension is length of one side of a sud
    Vars rowLength and colLength are redundant of dataDimension. 
    Purpose: Avoid data abstraction violation. */
    int rowLength; // length of row
    int colLength; // length of column
    int boxDimension; // boxDimension is the length of one side of a box
    int totalUnsolved;
    int initialUnsolved;
    int numbersFound; // state variable for numers found during algo
    int solveIterations; // How many iterations did solve run?
    int backtrackIterations; // How many times did the backtrack algo backtrack?
    int **matrix; // main representation; 2D matrix, list of pointers to arrays containing digits
    struct box **boxList; // list of pointers to Boxes, filling from top-left to right
};

struct box
{
    int unsolvedCount; // How many elements are currently unsolved?
    int ***pointerMatrix; // (list of pointers to arrays containing pointers)
    int boxHorizontalBound; // width of box
    int boxVerticalBound; // height of box
};

// Prototypes

int solveSudoku(struct sudoku *sud, int algoChoice);
int outputSudoku(struct sudoku *sud);
int robustBacktrackAlgo(struct sudoku *sud);
int robustAlgo(struct sudoku *sud);

// General functions

int printMatrix(int **matrix, int rowLength, int colLength, int highlightRow, int highlightCol)
{

    for (int row = 0; row < (rowLength); row++)
    {
        printf("|");
       for (int col = 0; col < (colLength); col++)
        {
            if (highlightRow == row && highlightCol == col)
            {
                printf(ANSI_COLOR_RED "%d" ANSI_COLOR_RESET "|", matrix[row][col] );
            }
            else
            {
            printf("%d|", matrix[row][col]);
            }
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
        printf("Calloc failed to allocate memory. Terminating program.");
        exit(1);
    }
    return memoryAddress;
}

int convertArrayDimension(int *onedimensional,  int **matrix, int dataDimension, int datacount){
    int row, col;
    row = col = 0;
    for (int i = 0; i < datacount; i++)
    {
        if (i % (dataDimension) == 0 && i != 0)
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

int countBoxUnsolved(struct box *box)
{
    // returns -1 if mistake, else returns boxcount

    int boxUnsolvedCount = box->boxHorizontalBound * box->boxVerticalBound; // TODO: Fix for snake matrix

    for (int boxRow = 0; boxRow < box->boxHorizontalBound; boxRow++)
    {
        for (int boxCol = 0; boxCol < box->boxVerticalBound; boxCol++)
        {
            int curval = *(box->pointerMatrix[boxRow][boxCol]);
            if (curval != 0)
            {
                boxUnsolvedCount -= 1;
            }

        }
    }
    box->unsolvedCount = boxUnsolvedCount; 


    return 0;


}

int countSudUnsolved(struct sudoku *sud)
{
    // Counts unsolved entries in Sudoku
    int totalUnsolved = 0;
    int rowlength = sud->rowLength;
    int colLength = sud->colLength;

    for (int row = 0; row < rowlength; row++)
    {
        for (int col = 0; col < colLength; col++)
        {
            if (sud->matrix[row][col] == 0)
            {
                totalUnsolved += 1;
            }
        }
    }

    sud->totalUnsolved = totalUnsolved;
    return 0;
}

int initBoxMatrix (struct box *box, struct sudoku *sud, int boxPosVertical, int boxPosHorizontal, int boxDimension)
{
    /* Terminology: 
    
    boxPos refer to the position of the box within the matrix.
    Needed for initializing pointers to elements.
    
    a 9 x 9 Matrix will have 9 boxes:
    B B B
    X B B   (Box X has boxPosVertical 1, boxPosHorizontal 0)
    B B B
    This functions creates a Box at pos boxPosVertical, boxPosHorizontal
    boxRow & boxCol refer to the cols and row INSIDE a box.
    Delving into box:
    sud->boxList[1][0]->boxMatrix:
    P P P
    P P P // These are pointers to a sud matrix
    P P P
    */

    box->boxHorizontalBound = floor(sqrt((double)sud->colLength));
    box->boxVerticalBound = floor(sqrt((double)sud->rowLength));

   // Memory allocation
   int ***tempBoxMatrix = (int ***)saferCalloc(boxDimension, sizeof(int**));
   for (int i = 0; i < (boxDimension); i++)
   {
       tempBoxMatrix[i] = (int**)saferCalloc(boxDimension, sizeof(int*));
   }

    // Assign pointers + calculate unsolvedcount

    for (int boxRow = 0; boxRow < boxDimension; boxRow++)
    {
        for (int boxCol = 0; boxCol < boxDimension; boxCol++)
        {
            tempBoxMatrix[boxRow][boxCol] = &(sud->matrix[boxRow+(boxPosVertical*(box->boxVerticalBound))][boxCol+(boxPosHorizontal*(box->boxHorizontalBound))]); // assign pointer to field
        }
    }

    box->pointerMatrix = tempBoxMatrix;
    box->unsolvedCount = countBoxUnsolved(box);

    return 0;
}

int initBoxList(struct sudoku *sud)
{  
    int boxDimension = floor(sqrt((double) sud->dataDimension));
    sud->boxDimension = boxDimension;
    if ((boxDimension * boxDimension) != sud->dataDimension)
    {
        printf("BoxWidth sqrt flooring failed to produce round number.");
        exit (1);
    }


    // Init boxList
    struct box**tempBoxList = (struct box**)saferCalloc(boxDimension, sizeof(struct box*)); 
    for (int i = 0; i < boxDimension; i++) // create boxList and assign pointer to a struct box
    {
        tempBoxList[i] = (struct box*)saferCalloc(boxDimension, sizeof(struct box));
    }
    for (int row = 0; row < boxDimension; row++)
    {
        for (int col = 0; col < boxDimension; col++)
        {
        struct box tempBox;
        tempBox.unsolvedCount = MAXDIMENSION+1;
        initBoxMatrix(&tempBox,sud, row,col, boxDimension);
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

    // sudokuArray is a one-dimensional representation
    // Assign one-dimensional attributes
    sud->size = *size;
    sud->rowLength = sud->colLength = sud->dataDimension = *dataDimension; // square sudoku assumed hence same length and width
    sud->backtrackIterations = 0;
    sud->solveIterations = 0;
    sud->numbersFound = 0;
    // Initialize Matrix via array of pointers to arrays
    int rowcount = *dataDimension;
    int colcount = *dataDimension;
    int **matrix = (int **)saferCalloc(rowcount, sizeof(int*)); // Dynamically allocate pointers to an array.
    for (int i = 0; i < rowcount; i++) {
        matrix[i] = (int *)saferCalloc(colcount, sizeof(int)); // We now have a matrix[row][col] initialized to all zeros.
    }

    convertArrayDimension(sudokuArray, matrix, *dataDimension, *size); // convert 1D to 2D
    sud->matrix = matrix;
    printf("Sudoku initialized.\nSize: %d, Length of rows: %d, Length of cols: %d \n", sud->size, sud->colLength, sud->rowLength);
    printf("Sudoku Matrix: \n");
    printMatrix(sud->matrix, sud->rowLength, sud->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    // Init boxStructure
    initBoxList(sud);
    countSudUnsolved(sud); // How many entries unsolved?
    sud->initialUnsolved = sud->totalUnsolved; // set starting point of totalUnsolved
    return 0;
}

int solveSudoku(struct sudoku *sud, int algoChoice)
{

    /* Multiple algorithms are available for solving sudokus. 
    
    The argument algoChoice refers to the chosen algorithm:
    0 = robustAlgo (for each field, check every row, col, box; if there is only one possibility within the field, fill it in.)
    1 = Backtracking using the robustAlgo.
    */

    // Main algo method
    int (*algoMethod[])(struct sudoku *sud) = {robustAlgo, robustBacktrackAlgo}; 
    /*array of functions
    
    The robustAlgo is algoMethod[0], backtrack is algoMethod[1] etc.
    Takes arguments:
    - Struct sudoku
    - State variable for how many numbers were found in iteration
    */ 

    for (int i = sud->solveIterations; i < MAXITERATIONS; i++)
    {
        sud->numbersFound = 0; // How many numbers were found this iteration?
        sud->solveIterations += 1;
        // printf("Current iteration: %i \n", sud->solveIterations);

        (*algoMethod[algoChoice])(sud); // Solve using the chosen algoMethod

    if (sud->numbersFound == 0)
    {

        if (sud->totalUnsolved == 0)
        {
            // printf("All numbers were found! \n"); // backtrack base cases
            return 0;
        }
        else
        {
            // printf("Algorithm did not find all numbers. \n");
            return 1;
        }
    }

    // printf("Amount of numbers found this iteration: %i \n", numbersFound);
    }


    return 0;
}

int deepCopySud(struct sudoku *sudToCopy, struct sudoku *sudTarget)
{
    // *sudToCopy will be the copied sud, saved in *sudTarget

    sudTarget->size = sudToCopy->size;
    sudTarget->dataDimension = sudToCopy->dataDimension;
    sudTarget->rowLength = sudToCopy->rowLength;
    sudTarget->colLength = sudToCopy->colLength;
    sudTarget->boxDimension = sudToCopy->boxDimension;
    sudTarget->totalUnsolved = sudToCopy ->totalUnsolved;
    sudTarget->initialUnsolved = sudToCopy->initialUnsolved;
    sudTarget->numbersFound = sudToCopy ->numbersFound;
    sudTarget->solveIterations = sudToCopy->solveIterations;
    sudTarget->backtrackIterations = sudToCopy ->backtrackIterations;

    // Deep copy matrix & remake boxList

    //Allocate space for matrix
    int **matrix = (int **)saferCalloc(sudToCopy->rowLength, sizeof(int*)); // Dynamically allocate pointers to an array.
    for (int i = 0; i < sudToCopy->rowLength; i++) {
        matrix[i] = (int *)saferCalloc(sudToCopy->colLength, sizeof(int)); // We now have a matrix[row][col] initialized to all zeros.
    }
    sudTarget->matrix = matrix;

    for (int row = 0; row < sudToCopy->rowLength; row++)
    {
        for (int col = 0; col < sudToCopy->colLength; col++)
        {
            sudTarget->matrix[row][col] = sudToCopy->matrix[row][col];
        }
    }

    initBoxList(sudTarget);

    /*printf("Original matrix: \n");
    printMatrix(sudToCopy->matrix, sudToCopy->rowLength, sudToCopy->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    printf("Copied matrix: \n");
    printMatrix(sudTarget->matrix, sudToCopy->rowLength, sudToCopy->colLength, MAXDIMENSION+1, MAXDIMENSION+1);*/

    return 0;
}

int outputSudoku(struct sudoku *sud)
{
    // print stats
    printf("\n\n\n");

    if (sud->backtrackIterations != 0)
    {
        printf("The code backtracked %i times \n", sud->backtrackIterations);
    }

    printf("Solving took %i iterations \n", sud->solveIterations);
    if (sud->solveIterations >= MAXITERATIONS)
    {
        printf("Stopped attempting to solve sudoku because maximum iterations reached.");
    }

    if (sud->totalUnsolved == 0)
    {
    // print result
    printf("\nSudoku Solved:\n");
    printMatrix(sud->matrix,sud->rowLength,sud->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    }
    else
    {
        printf("Solution not found. \n Sudoku:\n");
        printMatrix(sud->matrix,sud->rowLength,sud->colLength, MAXDIMENSION+1,MAXDIMENSION+1); 
    }

    // save to disk


    return 0;
    }

/* Algorithms
Each algorithm has two basic arguments:
- struct sudoku sud
Algorithms prepend their dependencies 
eg. robustBacktrack depends on robustAlgo.
robustAlgo depends on robustRow, robustCol, robustBox, robustPoss.
*/

// Algo - robust Algo

int robustCheckRow(struct sudoku *sud, int number, int matrixRow)
{
    for (int varDimension = 0; varDimension < sud->colLength; varDimension++) // iterate over cols
    {
        if (sud->matrix[matrixRow][varDimension] == number)
        {
            return 1; // Found matching number
        }
    }

    return 0; // No result found
}

int robustCheckCol(struct sudoku *sud, int number, int matrixCol)
{
    for (int varDimension = 0; varDimension < sud->rowLength; varDimension++) // iterate over rows
    {
        if (sud->matrix[varDimension][matrixCol] == number)
        {
            return 1; // Found matching number
        }
    }

    return 0; // No result found
}

int robustCheckBox(struct sudoku *sud, int number, int currentBoxHorizontal, int currentBoxVertical, int horizontalBound, int verticalBound)
{
    int curValue = 0;
    for (int row = 0; row < verticalBound; row ++)
    {
        for (int col = 0; col < horizontalBound; col++)
        {

            curValue = *(sud->boxList[currentBoxVertical][currentBoxHorizontal].pointerMatrix[row][col]);
            if (curValue == number)
            {
                return 1; // found matching number
            }
        }
    }
    return 0; // found no match
}

int robustPoss(struct sudoku *sud, int row, int col, int *posArray, int currentBoxHorizontal, int currentBoxVertical, int boxHorizontalBound, int boxVerticalBound)
{
    int posCounter = sud->colLength; // How many possibilities are there?

        for (int number = 1; number <= sud->rowLength; number++)
        {   // For every row & col, check whether number appears
            if (sud->matrix[row][col] != 0 && sud->matrix[row][col] != -1)
            {
                posCounter = 0;
                break; // Already a value present
            }

            // Check the row
            else if (robustCheckRow(sud, number, row) == 1) // found matching number
            {
                posArray[number] = MAXDIMENSION+1; //posArray is one-indexed for consistency with number
                posCounter -= 1;
                continue;
            }

            // Check the col
            else if (robustCheckCol(sud, number, col) == 1) // found mathcing number   
            {
                posArray[number] = MAXDIMENSION+1;
                posCounter -= 1;
                continue;
            }

            // Check the box
            else if (robustCheckBox(sud, number, currentBoxHorizontal, currentBoxVertical, boxHorizontalBound, boxVerticalBound) == 1) // found matching number
            {
                posArray[number] = MAXDIMENSION+1;
                posCounter -= 1;
                continue;
            }
        }

    return posCounter;
}

int robustAlgo(struct sudoku *sud)
{

    /*
    Optimizations: 
    - Stops searching when no more possibilities; does not redundantly call checkCol after checkRow if not needed.
    */
    int boxHorizontalBound = floor(sqrt((double)sud->colLength));
    int boxVerticalBound = floor(sqrt((double)sud->rowLength));
    int currentBoxHorizontal = 0;
    int currentBoxVertical= 0;


    for (int row = 0; row < sud->rowLength; row++)
    {
        // Track current box
        if (row != 0 && ((row % boxVerticalBound) == 0))
        {
            currentBoxVertical += 1;
        }
        currentBoxHorizontal = 0; // reset current horizontal box at every new row
        for (int col = 0; col < sud->colLength; col++)
        {
        if (col != 0 && ((col % boxHorizontalBound) == 0)) // Find out when box changes horizontally
        {
            currentBoxHorizontal += 1;
        }


        // Eliminate possibilities

        int* posArray = (int*) saferCalloc(sud->colLength + 1,sizeof(int)); 
        /* Potential optimization:
        Optimize length of posArray to match the remaining number of possibilities.  
        (no need to check every number if there are only 2 possibilities, 
        eg. 5 and 9 are possible; instead of [1,1,1,1,0,1,1,1,1,0] do [5,9])
        */

        int posCounter = robustPoss(sud, row, col, posArray, currentBoxHorizontal, currentBoxVertical, boxHorizontalBound,boxVerticalBound); // returns the amount of possibilities; -1 is an error.
        // Allocate memory for possibilities & initialize max possibilites.

        // Check if there is a single solution possible: 
        if (posCounter == 1)
        {
            //Find the only possible number
            for (int number = 1; number <= sud->rowLength; number++)
            {
                if (posArray[number] == 0) // If the number has not been used yet
                {
                    // printf("Found number: %i at row %i col %i | ", number, row+1, col+1); // one-indexed print
                    sud->matrix[row][col] = number;
                    // printf("\n");
                    // printMatrix(sud->matrix,sud->rowLength, sud->colLength,row,col);
                    sud->numbersFound += 1;
                    sud->totalUnsolved -= 1;
                }
            }
        }

        // Reset the possibility array
        free(posArray);
        }
    }
    if(sud->numbersFound == 0)
    {
        return 1;
    }

    return 0;
}

// Algo - Backtrack using robustAlgo

int robustBacktrackAlgo(struct sudoku *sud)
{

    /*
    Pseudocode:
    run robustAlgo
    if robustAlgo returns 0, then exit backTrackAlgo (single iteration completed)
    if robustAlgo returns 1:
    - find the most contstrained box in the sudoku
    - find the possibilities of that box
    - create a temp sudoku copy
    - fill in a possibility of that sudoku
    - run solveSudoku with robustAlgo on that field
    -- if found, return 0
    -- if not found, try other possibility
    -- if no possibilities found, return 1
    */

   int algoReturnCode = robustAlgo(sud); // run algo, save return code

   if (algoReturnCode == 0) // Found numbers
   {
       return 0;
   }

   else if (algoReturnCode == 1) // No numbers found
   {
       // Find most constrained box

       int minBoxVertical = MAXDIMENSION + 1;
       int minBoxHorizontal = MAXDIMENSION + 1;
       int minBoxUnsolvedCount = MAXDIMENSION + 1;
       int curBoxUnsolvedCount = -1;

       for (int boxVertical = 0; boxVertical < sud->boxDimension; boxVertical++)
       {
           for (int boxHorizontal = 0; boxHorizontal < sud->boxDimension; boxHorizontal++)
           {
               countBoxUnsolved(&(sud->boxList[boxVertical][boxHorizontal]));
               curBoxUnsolvedCount = sud->boxList[boxVertical][boxHorizontal].unsolvedCount;
               if (curBoxUnsolvedCount < minBoxUnsolvedCount && curBoxUnsolvedCount > 1) // There has to be more than 1 unsolved in the box
               // Optimization: If curBoxUnsolvedCount == 2, then break the loop; there will not be a lower value found.
               {
                   // If lowest count, make min box the current box
                   minBoxVertical = boxVertical;
                   minBoxHorizontal = boxHorizontal;
                   minBoxUnsolvedCount = curBoxUnsolvedCount;
               }

           }
       }

        if (minBoxVertical == MAXDIMENSION+1 || minBoxHorizontal == MAXDIMENSION+1) // If no constrained box is found
        {
            return 1;
        }

        int boxHorizontalBound = floor(sqrt((double)sud->colLength));
        int boxVerticalBound = floor(sqrt((double)sud->rowLength));

        // Within the most constrained box, look for the field with the lowest possibilities
        int lowestFieldPos = MAXDIMENSION+1;
        int lowestPosArray[sud->colLength + 1]; // Initialize this to all -1;

        for (int i = 0; i < sud->colLength + 1; i++)
        {
            lowestPosArray[i] = -1;
        }

        int lowestFieldRow = MAXDIMENSION + 1;
        int lowestFieldCol = MAXDIMENSION + 1;

        // In the box, find the field with min possibilities
        for (int rowBox = 0; rowBox < sud->boxDimension; rowBox++)
        {
            for (int colBox = 0; colBox < sud->boxDimension; colBox++)
            {

            // minbox
            int* posArray = (int*) saferCalloc(sud->colLength + 1,sizeof(int));
            int posCount = robustPoss(sud, (minBoxVertical*boxVerticalBound) + rowBox, (minBoxHorizontal*boxHorizontalBound) + colBox, posArray, minBoxHorizontal, minBoxVertical, boxHorizontalBound, boxVerticalBound);
            if (posCount < lowestFieldPos && posCount > 1)
            {
                lowestFieldPos = posCount;
                lowestFieldRow = (minBoxVertical*boxVerticalBound) + rowBox;
                lowestFieldCol = (minBoxHorizontal*boxHorizontalBound) + colBox;

                // Copy lowest pos array
                for (int i = 0; i < (sud->colLength + 1); i++)
                {
                    lowestPosArray[i] = posArray[i]; //BUG posarray does not get copied
                }
            }
            free(posArray);
            }
        }

        // We have lowest field now, try the possibilities




        int solveReturnCode;

        for (int number = 1; number <= sud->rowLength; number++)
            {
                if (lowestPosArray[number] == 0) // Number is still possible
                {
                    // Create temp matrix, fill it with number try
                    struct sudoku *sudTemp = (struct sudoku *) saferCalloc(1, sizeof(struct sudoku));
                    deepCopySud(sud,sudTemp);
                    sudTemp->matrix[lowestFieldRow][lowestFieldCol] = number;
                    // printf("Trying new Matrix via backtracking\n");
                    // printMatrix(sudTemp->matrix, sudTemp->rowLength, sudTemp->colLength, lowestFieldRow, lowestFieldCol);
                    sud->numbersFound += 1; // tries possibility
                    sudTemp->backtrackIterations += 1;
                    sudTemp->totalUnsolved -= 1;

                    int *iterations = (int *)saferCalloc(1,sizeof(int));
                    *iterations = MAXITERATIONS;
                    solveReturnCode = solveSudoku(sudTemp, 1);

                    if(solveReturnCode == 0) // Found a full sudoku!
                    {
                        deepCopySud(sudTemp, sud); // The temp sudoku is the real sudoku!
                        sud->numbersFound = 0;
                        return 0;
                    }
                    else
                    {
                        sud->numbersFound -= 1; // undo attempted possibility
                        sud->backtrackIterations += (sudTemp->backtrackIterations) - (sud->backtrackIterations);
                        sud->solveIterations += ((sudTemp->solveIterations) - (sud->solveIterations)); 
                        free(sudTemp);
                        continue;
                    }
                }
            }


    }

    else
    {
        printf("Unexpected return robustAlgo return code.");
        exit(1);
    }

    return 0;

}

int startSudoku(int algoChoice, int size, int dataDimension, int *sudokuArray){
    
    // Convert one-dimensional temporary array to 2D matrix in sudoku struct
    struct sudoku *sud = (struct sudoku *) saferCalloc(1, sizeof(struct sudoku)); // initialize sud pointer to struct sudoku

    initSudoku(&size,&dataDimension, sudokuArray, sud) ;
    solveSudoku(sud, algoChoice);
    outputSudoku(sud);
    free(sud);

    return 0; // main finished succesfully 
} 