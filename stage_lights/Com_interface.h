#ifndef Com_interface_h
#define Com_interface_h

#include <string.h>
#include <Arduino.h>
#include "Globals.h"

class ComInterface{
  public:
    int animation;

    /*Reads the contense of the serial buffer and sets all the appropriate variables.*/
    static void readInput();

    static void serialFlush();

    static void setAnimation(int value);

    static int getAnimation();

    static void initParametersArray();

    //void showParsedData();

    //void recvWithStartEndMarkers();

    //void parseData()
};

//int ComInterface::animation = 1;
#endif