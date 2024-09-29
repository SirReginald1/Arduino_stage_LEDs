#ifndef Com_interface_h
#define Com_interface_h

#include <string.h>
#include <Arduino.h>
#include "Globals.h"

// Declaring that this exists here instread of including to avoide circular imports
struct animParamRef;


class ComInterface{
  public:
    //static int animation;

    //static animParamRef animParamRefs[NB_ARRAYS];

    
    /**The mode used to determin how receved data is parsed. In sync with the curently used functionality.*/
    static int dataParsingMode;

    /*Reads the contense of the serial buffer and sets all the appropriate variables.*/
    static void readInput();

    static void serialFlush();

    //static void setAnimation(int value);

    //static int getAnimation();

    /*static void initParametersArray();*/

    //void showParsedData();

    //void recvWithStartEndMarkers();

    //void parseData()
};

//int ComInterface::animation = 1;
#endif