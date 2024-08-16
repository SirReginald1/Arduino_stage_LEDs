#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// Testing lib
#include "Utils.h"

#define NUM_LEDS 122
#define NB_ARRAYS 4
#define NB_ANIMATIONS 9
#define NB_MAX_PARAMS 10


int array[3][3][3] = {{{111, 112, 113},
                       {121, 122, 123},
                       {131, 132, 133}},
                      {{211, 212, 213},
                       {221, 222, 223},
                       {231, 232, 233}},
                      {{311, 312, 313},
                       {321, 322, 323},
                       {331, 332, 333}}};

/*
* Function used to update parameter values in the integer refference table.
*/
/*int outputAndReplace(int rows, int cols, int depth, int array[3][3][3], int row, int col, int dep, int newValue) {
    // Check if the specified position is within the bounds of the array
    if (row >= rows || col >= cols || dep >= depth || row < 0 || col < 0 || dep < 0) {
        printf("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    int oldValue = array[row][col][dep];
    printf("Value at position [%d][%d][%d] = %d\n", row, col, dep, oldValue);

    // Replace the value at the specified position with the new value
    array[row][col][dep] = newValue;

  return oldValue;
}*/

int main(){

  int out;
  int row = 1, col = 1, dep = 1; 
  out = outputAndReplace(3,3,3,array,row,col,dep,000);

  printf("The outputed value: %d", out);

  printf("The new value: %d", array[row][col][dep]);

  return 0;
}








/*
* Function used to update parameter values in the float refference table.
*/
/*
int outputAndReplace(int rows, int cols, int depth, int ***array, int row, int col, int dep, int newValue) {
    // Check if the specified position is within the bounds of the array
    if (row >= rows || col >= cols || dep >= depth || row < 0 || col < 0 || dep < 0) {
         printf("Error: Specified position is out of bounds.\n");
        return -1;
    }

    // Output the current value at the specified position
    int oldValue = array[row][col][dep];
    printf("Value at position [%d][%d][%d] = %d\n", row, col, dep, oldValue);

    // Replace the value at the specified position with the new value
    array[row][col][dep] = newValue;

  return oldValue;
}
*/

