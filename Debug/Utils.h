#ifndef Utils_h
#define Utils_h

#define NUM_LEDS 122
#define NB_ARRAYS 4
#define NB_ANIMATIONS 9
#define NB_MAX_PARAMS 10

#include <string.h>
//#include <Arduino.h>

void copy2DTo3D(int rows, int cols, int depth, int source[NB_ARRAYS][NB_ANIMATIONS], int destination[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int targetDepth);

int outputAndReplace(int rows, int cols, int depth, int array[3][3][3], int row, int col, int dep, int newValue);

float outputAndReplace(int rows, int cols, int depth, float array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, float newValue);

unsigned long outputAndReplace(int rows, int cols, int depth, unsigned long array[NB_ARRAYS][NB_ANIMATIONS][NB_MAX_PARAMS], int row, int col, int dep, unsigned long newValue);

#endif