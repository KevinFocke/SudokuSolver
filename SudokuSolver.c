// Sudoku Solver 

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

const int MAXDIMENSION = 36; // Max dimension of sudoku. >9 is experimental.
const int MAXITERATIONS = 100000000; 
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
    int numbersFoundTotal;
    int **matrix; // 2D matrix, list of pointers to arrays containing digits
    struct box **boxList; // list of pointers to Boxes, filling from top-left to right;
    int solveIterations; // How many iterations did solve run
    int backtrackIterations;
};

struct box
{
    int unsolvedCount; // How many elements are currently unsolved?
    int ***pointerMatrix; // (list of pointers to arrays containing pointers)
    int boxHorizontalBound;
    int boxVerticalBound;
};

// prototype

int solveSudoku(struct sudoku *sud, int algoChoice);
int outputSudoku(struct sudoku *sud);

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
    // Count unsolved entries
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

int initBoxMatrix (struct box *box, struct sudoku *sud, int boxPosVertical, int boxPosHorizontal, int boxWidth)
{
    /* Terminology: 
    
    boxPos refer to the position of the box within the matrix.
    Needed for initializing pointers to elements.
    
    a 9 x 9 Matrix will have 9 boxes:
    B B B
    B B B   (the first B in this row has boxPosVertical 0, boxPosHorizontal 0)
    B B B

    This functions creates 1 Box at pos boxPosVertical, boxPosHorizontal

    boxRow & boxCol refer to the cols and row INSIDE a box.

    Delving into box:
    sud->boxList[1][0]->boxMatrix:
    P P P
    P P P
    P P P
    */
    box->boxHorizontalBound = floor(sqrt((double)sud->colLength));
    box->boxVerticalBound = floor(sqrt((double)sud->rowLength));
   // Memory allocation
   int ***tempBoxMatrix = (int ***)saferCalloc(boxWidth, sizeof(int**));
   for (int i = 0; i < (boxWidth); i++)
   {
       tempBoxMatrix[i] = (int**)saferCalloc(boxWidth, sizeof(int*));
   }

    // Assign pointers + calculate unsolvedcount

    // TODO: Merge branch!!!
    // TODO: refactor boxWidth (part of the sud);

    for (int boxRow = 0; boxRow < boxWidth; boxRow++)
    {
        for (int boxCol = 0; boxCol < boxWidth; boxCol++)
        {
            tempBoxMatrix[boxRow][boxCol] = &(sud->matrix[boxRow+(boxPosVertical*(box->boxVerticalBound))][boxCol+(boxPosHorizontal*(box->boxHorizontalBound))]); // assign pointer to field
        }
    }

    box->pointerMatrix = tempBoxMatrix;
    box->unsolvedCount = countBoxUnsolved(box);
    // TODO: Snake matrix, customize boxWidth

    return 0;
}

int initBoxList(struct sudoku *sud, int dataDimension)
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
    sud->backtrackIterations = 0;
    sud->solveIterations = 0;
    // initialize Matrix via array of pointers to arrays
    int rowcount = *dataDimension;
    int colcount = *dataDimension;
    sud ->numbersFoundTotal = 0;
    // Initialize the matrix
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
    initBoxList(sud, *dataDimension);
    countSudUnsolved(sud); //how many entries unsolved?
    sud->initialUnsolved = sud->totalUnsolved;
    return 0;
}

int readFile(char *inputFilename, int *size, int *sudokuArray, int *dataDimension)
{
    // TODO: Regex to set custom inputFilename
    // readGit source control manager in the file
    FILE *fp;
    fp = fopen(inputFilename, "r");


    *size = 0;
    printf("Scanning %s\n", inputFilename);
    // TODO : Detect non-numeric characters
    int buffer = 0;
    while (fscanf(fp, "%d", &buffer) != EOF){
    if (buffer < 0){
            printf("\n Detected negative number.");
            exit(1);
            }
        sudokuArray[*size] = buffer; // put current int in array
        *size += 1;
    }

    for (int i = 1; i <= MAXDIMENSION; i++)
    {
        if (*size == i * i)
        {
            *dataDimension = i;
            break;
        }
    }

    if (*dataDimension == 0)
    {
        printf("\n Invalid size. Numbers received: %d. \n Expected a square of a number between 0 - %d. \n",*size,MAXDIMENSION);
        exit(1);
    }

    return 0;
}

int checkRow(struct sudoku *sud, int number, int matrixRow)
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

int checkCol(struct sudoku *sud, int number, int matrixCol)
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

int checkBox(struct sudoku *sud, int number, int currentBoxHorizontal, int currentBoxVertical, int horizontalBound, int verticalBound)
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

int simplePoss(struct sudoku *sud, int row, int col, int *posArray, int currentBoxHorizontal, int currentBoxVertical, int boxHorizontalBound, int boxVerticalBound)
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
            else if (checkRow(sud, number, row) == 1) // 
            {
                posArray[number] = MAXDIMENSION+1; //posArray is one-indexed for consistency with number
                posCounter -= 1;
                continue;
            }

            // Check the col
            else if (checkCol(sud, number, col) == 1) // found mathcing number   
            {
                posArray[number] = MAXDIMENSION+1;
                posCounter -= 1;
                continue;
            }

            // Check the box
            else if (checkBox(sud, number, currentBoxHorizontal, currentBoxVertical, boxHorizontalBound, boxVerticalBound) == 1) // found matching number
            {
                posArray[number] = MAXDIMENSION+1;
                posCounter -= 1;
                continue;
            } // remember: Potential bug in pointer arithmetic
        }

    return posCounter;
}

int simpleAlgo(struct sudoku *sud, int *numbersFound)
{
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

        int posCounter = simplePoss(sud, row, col, posArray, currentBoxHorizontal, currentBoxVertical, boxHorizontalBound,boxVerticalBound); // returns the amount of possibilities; -1 is an error.
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
                    *numbersFound += 1;
                    sud->numbersFoundTotal += 1;
                    sud->totalUnsolved -= 1;
                }
            }
        }

        // Reset the possibility array
        free(posArray);
        }
    }
    if(*numbersFound == 0)
    {
        return 1;
    }

    return 0;
}

int deepCopySud(struct sudoku *sudToCopy, struct sudoku *sudTarget)
{
    // *sudToCopy will be the copied sud, saved in *sudTarget

    sudTarget->backtrackIterations = sudToCopy ->backtrackIterations;
    sudTarget->boxWidth = sudToCopy->boxWidth;
    sudTarget->colLength = sudToCopy->colLength;
    sudTarget->initialUnsolved = sudToCopy->initialUnsolved;
    sudTarget->numbersFoundTotal = sudToCopy->numbersFoundTotal;
    sudTarget->rowLength = sudToCopy->rowLength;
    sudTarget->size = sudToCopy->size;
    sudTarget->solveIterations = sudToCopy->solveIterations;
    sudTarget->totalUnsolved = sudToCopy ->totalUnsolved;
    
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

    initBoxList(sudTarget, sudTarget->rowLength);

    /*printf("Original matrix: \n");
    printMatrix(sudToCopy->matrix, sudToCopy->rowLength, sudToCopy->colLength, MAXDIMENSION+1, MAXDIMENSION+1);
    printf("Copied matrix: \n");
    printMatrix(sudTarget->matrix, sudToCopy->rowLength, sudToCopy->colLength, MAXDIMENSION+1, MAXDIMENSION+1);*/

    return 0;
}

int backtrackAlgo(struct sudoku *sud, int *numbersFound)
{
    
    /*
    Pseudocode:
    run simpleAlgo
    if simpleAlgo returns 0, then exit backTrackAlgo (single iteration completed)
    if simpleAlgo returns 1:
    - find the most contstrained box in the sudoku
    - find the possibilities of that box
    - create a temp sudoku copy
    - fill in a possibility of that sudoku
    - run solveSudoku with simpleAlgo on that field
    -- if found, return 0
    -- if not found, try other possibility
    -- if no possibilities found, return 1
    */

   int algoReturnCode = simpleAlgo(sud,numbersFound); // run algo, save return code
   
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

       for (int boxVertical = 0; boxVertical < sud->boxWidth; boxVertical++)
       {
           for (int boxHorizontal = 0; boxHorizontal < sud->boxWidth; boxHorizontal++)
           {
               countBoxUnsolved(&(sud->boxList[boxVertical][boxHorizontal]));
               curBoxUnsolvedCount = sud->boxList[boxVertical][boxHorizontal].unsolvedCount; // BUG: curBoxUnsolvedCount does not change when it's filled in
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
        for (int rowBox = 0; rowBox < sud->boxWidth; rowBox++)
        {
            for (int colBox = 0; colBox < sud->boxWidth; colBox++)
            {
            
            // minbox
            int* posArray = (int*) saferCalloc(sud->colLength + 1,sizeof(int));
            int posCount = simplePoss(sud, (minBoxVertical*boxVerticalBound) + rowBox, (minBoxHorizontal*boxHorizontalBound) + colBox, posArray, minBoxHorizontal, minBoxVertical, boxHorizontalBound, boxVerticalBound);
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
                    *numbersFound += 1;
                    sudTemp->backtrackIterations += 1;
                    sudTemp->solveIterations += 1;
                    sudTemp->numbersFoundTotal += 1;
                    sudTemp->totalUnsolved -= 1;

                    int *iterations = (int *)saferCalloc(1,sizeof(int));
                    *iterations = MAXITERATIONS;
                    // TODO: Think through iterations, is this fully correct?
                    solveReturnCode = solveSudoku(sudTemp, 1);

                    if(solveReturnCode == 0) // Found a full sudoku!
                    {
                        deepCopySud(sudTemp, sud); // The temp sudoku is the real sudoku!
                        *numbersFound = 0; // BUG: Overcounts number by 1, temp fix
                        return 0;
                    }
                    else
                    {
                        *numbersFound -= 1;
                        sud->backtrackIterations += (sudTemp->backtrackIterations) - (sud->backtrackIterations);
                        sud->solveIterations += (sudTemp->solveIterations) - (sud->solveIterations); 
                        free(sudTemp);
                        continue;
                    }
                }
            }


    }

    else
    {
        printf("Unexpected return simpleAlgo return code.");
        exit(1);
    }

    return 0;

}

int solveSudoku(struct sudoku *sud, int algoChoice)
{

    /* Multiple algorithms are available for solving sudokus. 
    
    The argument algoChoice refers to the chosen algorithm:

    0 = Simple Method (check every row, col, box)
    1 = Backtracking using the simple method


    */

    // Main algo method
    int (*algoMethod[])(struct sudoku *sud, int*) = {simpleAlgo, backtrackAlgo}; 
    /*array of functions
    
    The simpleAlgo is algoMethod[0], backtrack is algoMethod[1] etc.
    Takes arguments:
    - Struct sudoku
    - State variable for how many numbers were found in iteration


    */ 

    for (int i = sud->solveIterations; i < MAXITERATIONS; i++) // TODO: Refactor maxiterations per sudoku
    {
        int numbersFound = 0; // How many numbers were found this iteration?
        sud->solveIterations += 1;
        // printf("Current iteration: %i \n", sud->solveIterations);
    
        (*algoMethod[algoChoice])(sud, &numbersFound); // Solve using the chosen algoMethod

    if (numbersFound == 0)
    {

        if (sud->numbersFoundTotal == sud->initialUnsolved)
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


    return 0;}

int main(void){
    //TODO: Enable queuing sudokus

    int algoChoice = 1;

    // Init vars
    char inputFilename[] = "sudoku_input_medium3.txt"; //TODO: Rename to input, Allow command line recognition of flags
    
    // Process per Sudoku

    int size;
    int dataDimension; // DataDimension
    // Initialize the maximum possible sudoku array; one-dimensional
    int sudokuArray[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null. Bug value is -1.
    for (int i = 0; i < MAXARRAY; i++)
    {
        sudokuArray[i] = -1; 
    }

    //Function calls
    if (readFile(inputFilename, &size, sudokuArray, &dataDimension))
    {
        printf("Failed to read file.");
        exit(1); // TODO: change in case of queued sudoku's, break?
    }
    
    // Convert one-dimensional temporary array to 2D matrix in sudoku struct
    struct sudoku *sud = (struct sudoku *) saferCalloc(1, sizeof(struct sudoku)); // initialize sud pointer to struct sudoku
    
    initSudoku(&size,&dataDimension, sudokuArray, sud) ;
    solveSudoku(sud, algoChoice);
    outputSudoku(sud);
    free(sud);

    return 0; // main finished succesfully 
}