// For debug
#include <stdio.h>
#include <stdlib.h>

// In original
#include <string.h>
//#include <Arduino.h>

#define NUM_LEDS 122
#define NB_ARRAYS 4
#define NB_ANIMATIONS 9
#define NB_MAX_PARAMS 10





void copy2DTo3D(int rows, int cols, int depth, int source[NB_ANIMATIONS][NB_MAX_PARAMS], int destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetDepth) {
    // Ensure targetDepth is within the depth range of the destination array
    if (targetDepth >= depth) {
        printf("Error: targetDepth exceeds destination array depth.\n");
        return;
    }

    // Copy elements from 2D array to the specified depth slice of the 3D array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            destination[i][j][targetDepth] = source[i][j];
        }
    }
}

/*
* Function used to update parameter values in the integer refference table.
*/
int outputAndReplace(int rows, int cols, int depth, int array[][3][3], int row, int col, int dep, int newValue) {
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
}

/*
* Function used to update parameter values in the float refference table.
*/
float outputAndReplace(int rows, int cols, int depth, float array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, float newValue) {
    // Check if the specified position is within the bounds of the array
    if (row >= rows || col >= cols || dep >= depth || row < 0 || col < 0 || dep < 0) {
        printf("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    float oldValue = array[row][col][dep];
    printf("Value at position [%d][%d][%d] = %d\n", row, col, dep, oldValue);

    // Replace the value at the specified position with the new value
    array[row][col][dep] = newValue;

  return oldValue;
}

/*
* Function used to update parameter values in the integer refference table.
*/
unsigned long outputAndReplace(int rows, int cols, int depth, unsigned long array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, unsigned long newValue) {
    // Check if the specified position is within the bounds of the array
    if (row >= rows || col >= cols || dep >= depth || row < 0 || col < 0 || dep < 0) {
        printf("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    unsigned long oldValue = array[row][col][dep];
    printf("Value at position [%d][%d][%d] = %d\n", row, col, dep, oldValue);

    // Replace the value at the specified position with the new value
    array[row][col][dep] = newValue;

  return oldValue;
}