#ifndef Globals_h
#define Globals_h

/*The number of LEDs per array*/
#define NUM_LEDS 122
/*The number of arrays hooked up to the arduino*/
#define NB_ARRAYS 4
/*The total number of animations*/
#define NB_ANIMATIONS 9
/*The maiximum number of parameters present in an animation function*/
#define NB_MAX_PARAMS 10

/** Reference numbers for the switch conditions that govern the main application mode switching. */
#define MODE_SELECT_ANIMATION 1
#define MODE_PREPREP_ANIMATION 2
#define MODE_MIC_FFT_ON 3

/** The number of animations that are implemented */
#define NB_ANIMATIONS 9

/*Indicated if the program should be run by the real time animation interface or just animation numbers*/
#define USE_INTERFACE

#endif