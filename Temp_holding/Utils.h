#ifndef Utils_h
#define Utils_h

#include <string.h>
#include <Arduino.h>

void copy2DTo3D(int nb_animations, int nb_max_params, int maxDepth, int source[NB_ANIMATIONS][NB_MAX_PARAMS], int destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetLedArray);

int outputAndReplace(int rows, int cols, int depth, int array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, int newValue);

float outputAndReplace(int rows, int cols, int depth, float array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, float newValue);

unsigned long outputAndReplace(int rows, int cols, int depth, unsigned long array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, unsigned long newValue);

#endif