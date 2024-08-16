#include <string.h>
#include <Arduino.h>
#include "Globals.h"


void copy2DTo3D(int nb_animations, int nb_max_params, int maxDepth, int source[NB_ANIMATIONS][NB_MAX_PARAMS], int destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetLedArray) {
    // Ensure targetDepth is within the depth range of the destination array
    if (targetLedArray >= maxDepth) {
        Serial.println("Error: targetDepth exceeds destination array depth.\n");
        return;
    }

    // Copy elements from 2D array to the specified depth slice of the 3D array
    for (int i = 0; i < nb_animations; i++) {
        for (int j = 0; j < nb_max_params; j++) {
            destination[targetLedArray][i][j] = source[i][j];
        }
    }
}

void copy2DTo3D(int nb_animations, int nb_max_params, int maxDepth, float source[NB_ANIMATIONS][NB_MAX_PARAMS], float destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetLedArray) {
    // Ensure targetDepth is within the depth range of the destination array
    if (targetLedArray >= maxDepth) {
        Serial.println("Error: targetDepth exceeds destination array depth.\n");
        return;
    }

    // Copy elements from 2D array to the specified depth slice of the 3D array
    for (int i = 0; i < nb_animations; i++) {
        for (int j = 0; j < nb_max_params; j++) {
            destination[targetLedArray][i][j] = source[i][j];
        }
    }
}

void copy2DTo3D(int nb_animations, int nb_max_params, int maxDepth, unsigned long source[NB_ANIMATIONS][NB_MAX_PARAMS], unsigned long destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetLedArray) {
    // Ensure targetDepth is within the depth range of the destination array
    if (targetLedArray >= maxDepth) {
        Serial.println("Error: targetDepth exceeds destination array depth.\n");
        return;
    }

    // Copy elements from 2D array to the specified depth slice of the 3D array
    for (int i = 0; i < nb_animations; i++) {
        for (int j = 0; j < nb_max_params; j++) {
            destination[targetLedArray][i][j] = source[i][j];
        }
    }
}

/*
* Function used to update parameter values in the integer refference table.
*/
int outputAndReplace(int rows, int cols, int depth, int array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, int newValue) {
    // Check if the specified position is within the bounds of the array
    if (row >= rows || col >= cols || dep >= depth || row < 0 || col < 0 || dep < 0) {
        Serial.println("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    int oldValue = array[row][col][dep];
    Serial.print("Value at position [");Serial.print(row);Serial.print("][");Serial.print(col);Serial.print("][");Serial.print(dep);Serial.print("] = ");Serial.println(oldValue);

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
        Serial.println("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    float oldValue = array[row][col][dep];
    Serial.print("Value at position [");Serial.print(row);Serial.print("][");Serial.print(col);Serial.print("][");Serial.print(dep);Serial.print("] = ");Serial.println(oldValue);

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
        Serial.println("Error: Specified position is out of bounds.");
        return -1;
    }

    // Output the current value at the specified position
    unsigned long oldValue = array[row][col][dep];
    Serial.print("Value at position [");Serial.print(row);Serial.print("][");Serial.print(col);Serial.print("][");Serial.print(dep);Serial.print("] = ");Serial.println(oldValue);

    // Replace the value at the specified position with the new value
    array[row][col][dep] = newValue;

  return oldValue;
}