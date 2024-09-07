#ifndef Com_interface_h
#define Com_interface_h

#include <string.h>
#include <Arduino.h>

/*Reads the contense of the serial buffer and sets all the appropriate variables.*/
void read_input();

void serialFlush();

void copy2DTo3D(int rows, int cols, int depth, int source[rows][cols], int destination[rows][cols][depth], int targetDepth);

//void showParsedData();

//void recvWithStartEndMarkers();

//void parseAnimationChangeData()

#endif