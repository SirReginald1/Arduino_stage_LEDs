#ifndef Globals_h
#define Globals_h

//#define USE_BLUETOOTH

//######################################################################################################
//########################################## LED VARIABLES #############################################
//######################################################################################################

/*The number of LEDs per array*/
#define NUM_LEDS 122
/*The number of arrays hooked up to the arduino*/
#define NB_ARRAYS 4
/*The maiximum number of parameters present in an animation function*/
#define NB_MAX_PARAMS 10 // Depricated ?????

//######################################################################################################
//################################### OPERATING MODE VARIABLES #########################################
//######################################################################################################

/** Reference numbers for the switch conditions that govern the main application mode switching. */
#define MODE_SELECT_ANIMATION 1
#define MODE_PREPREP_ANIMATION 2
#define MODE_MIC_FFT_ON 3

//######################################################################################################
//###################################### ANIMATION VARIABLES ###########################################
//######################################################################################################

/** The number of animations that are implemented */
#define NB_ANIMATIONS 9
/** Codes that represent the placement of the animation function in the animation function array */
#define ANIM_CODE_RAINBOWCYCLE 0
#define ANIM_CODE_FADE 1
#define ANIM_CODE_SPARKLE 2
#define ANIM_CODE_FIRE 3
#define ANIM_CODE_SHOUTING_STAR 4
#define ANIM_CODE_TWINKLE_PIXELS 5
#define ANIM_CODE_STROBE 6
#define ANIM_CODE_ZIP 7
#define ANIM_CODE_FLASH_TO_BEAT 8

//######################################################################################################
//##################################### BEAT DETECTION VARIABLES #######################################
//######################################################################################################

/** Beat detection audio feature codes */
#define NO_AUDIO_FEATURE_DETECTED 0
#define KICK_DETECTED 1
#define SNAIRE_DETECTED 2
#define HIGH_HAT_DETECTED 3


/*Indicated if the program should be run by the real time animation interface or just animation numbers*/
//#define USE_INTERFACE // Depricated ?????

#endif