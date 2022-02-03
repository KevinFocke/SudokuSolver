
/* Sudoku solver

Ver 1: 
- Create a program that solves simple sudokus inefficiently.
- Can handle square boards of different sizes.
- Can read in space-delimited text files.
- Is extendible with different algorithms.


Programmming goals:
- Become more efficient with vim and more familiar with visual studio code.
- Think about data representations and make sure the code is extendable.

Ver 2: 
- Refactor code into multiple subroutines
- Implement Databox structure
- Add command line recognizition of filename=mytext.txt using fgets and regex
https://www.quora.com/What-are-some-better-alternatives-to-scanf-in-C-and-what-do-they-do-exactly
- Increase portability by typedeffing eg. int becomes int 32
- Implement backtracking algorithm


Programming goals:
- Start tracking changes with git
- Become more efficient in compiling & running the code.

Ver 3:
- Implement more efficient possMatrix
- Graph the timings of different algorithms.
- Increase the portability of the code
- Refactor code to make more extendible.

Ver 4:
- Implement race conditions per box
- Add ability to queue multiple sudoku's
- Graph the timings accross sudoku's across algorithms

Ver 5:
- Add cloud computing support

Ver 6:
- Add photo mode, make picture of sudoku, translates into matrix

Programming goals:
- Completely mouse-free coding.


*/

// Header

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <regex.h>

// TODO: Check types of global variables
int MAXWIDTH = 20; // Max width of sudoku
int MAXLENGTH = 20; // Max length of sudoku
#define MAX(A,B) ((A)>(B))?(A):(B);
int readData(FILE *streamname){}

struct sudoku{
    int* dataMatrix;
     // a 9 x 9 matrix contains 3 boxes on top, 3 boxes in middle, 3 in bottom
    int dimensionWidth;
    int dimensionLength;
    int numberCount;
    };


int initialize_posSet(int *possMatrix, int row, int col)
{
    return 0;
}


int readFile(char *filename)
{

}


// TODO: Create a seperate function for reading in file
int main(int argc, char **argv){
    // TODO: Regex to set custom filename
    char defaultfilename[] = "sudoku_input.txt";
    // readGit source control manager in the file
    FILE *fp;
    fp = fopen(defaultfilename, "r");
    // initialize variablesto zero
    int buffer, numbercount;
    buffer = numbercount = 0;
    int MAXARRAY = MAXWIDTH * MAXLENGTH;
    int MAXDIMENSION = 0;
    if (MAXWIDTH == MAXLENGTH){
        MAXDIMENSION = MAXWIDTH;
    }
    else if (MAXWIDTH != MAXLENGTH){
        MAXDIMENSION = MAX(MAXWIDTH,MAXLENGTH);
    }
    else{
        printf("Dimension weirdness");
        return 1;
    }
    int sudoku_array[MAXARRAY]; // unsolved sudokus are zero. Unfilled sudoku elements are null.
    printf("variables initialized. \n");
    for (int i = 0; i < MAXARRAY; i++){
        sudoku_array[i] = -1; 
    }
    printf("scanning %s \n", defaultfilename);
    // TODO : Detect non-numeric characters
    while (fscanf(fp, "%d", &buffer) != EOF){
    if (buffer < 0){
            printf("\n Detected negative number. Exiting program.");
            return 1;
            }
        sudoku_array[numbercount] = numbercount;
        printf("%d", buffer);
        numbercount += 1;
    }
    printf("\nCounted %d numbers \n", numbercount);

    // Count the size of the input. Does it match known dimensions?

    int DataMatrixDimension = 0; // Figure out the square dimension of the data
    for (int i = 1; i <= MAXDIMENSION; i++)
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

    int possSpace[DataMatrixDimension]; // The total ranges of numbers that could be in any one element
    // create possibilityspace
    for (int i = 0; i < DataMatrixDimension; possSpace[i++] = i+1);
    /*printf("These are the total possabilities per element, before checking validity:");
    for (int i = 0; i < DataMatrixDimension; i++)
    {
        printf("%d", possSpace[i]);
    }*/









    // Go to the posSet with the lowest amount of unsolved

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