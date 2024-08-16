#include <LiquidCrystal.h>
#include <LCD_I2C.h>
#include <string.h>
#define NUM_LEDS 122
#define NB_ARRAYS 2
#define NB_ANIMATIONS 9
#define NB_MAX_PARAMS 10

//#include "Com_interface.h"
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LCD_I2C lcd(0x27);

int current_array;
int animation;

// ################### Reading data code ##################
/*The maximum number of bytes that the buffer can recive.*/
const byte numChars = 32;

/*The buffer in which receved strings stored. Has a terminating character (\0).*/
char receivedChars[numChars];

/*Temporary array for use when parsing*/
char tempChars[numChars];        

/*Variable that holds a string present in the message.*/
char messageFromPC[numChars] = {0};

/*Integer that is extracted from the messageFromPC variable*/
int integerFromPC = 0;
/*Float that is extracted from the messageFromPC variable*/
float floatFromPC = 0.0;

/*Boolean indicating if there is data being proccesed.
If true will not read and parse from the buffer. Is set back to false once all data reved 
hase been delt with.*/
boolean newData = false;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

/**
* Function that takes the animation number and the number of the current parameter for that animation function and returns the code for the type of that variable.
* !!!!!!! Any parameter that is not directly related to the changing how the annimation runs such as current millis value or total number of LEDs in array is not 
* is skiped when counting the parameter number !!!!!!!
* 
* @param animation_in The animation code for the current animation.
*
*@param param_buffer_count_in The position of the current parameter in the specified animation function. Non animation related parameters are not counted in the count. 
*
* @return The code for the type of the current parameter. {0: int, 1: float, 2: unsigned long}
*/
int find_param_type(int animation_in, int param_buffer_count_in){
  // TODO: Add functions in animation.cpp for each animation that return their type so that all animation modifiaction do not necesitate modifications in this file.
  // TODO: Add a dictionary referencing annimation codes and type indicator codes so as to make them flexible.

  /*
  * Parameter type value indicator.
  * 0: int.
  * 1: float.
  * 2: unsigned long.
  */
  int type_indicator_out;
  // This switch identifies the type of the parameter in question.
    switch (animation_in) {
      // ################# The Sparkle animation #######################
      case 3: 
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) { // Starts at 0
          case 3:
            type_indicator_out = 0; // !!!!!!!!!!!!!!!!!!!!!!!! Change this to = 2 when you change the delay functioning !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            break;
          default:
            type_indicator_out = 0;
        }
        break;
      // ###########################################################

      // ################# The Fire animation #######################
      case 4:
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) {
          case 3:
            type_indicator_out = 1;
            break;
          default:
            type_indicator_out = 0;
        }
        break;
      // ###########################################################

      // ################# The zip animation #######################
      case 9:
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) {
          case 4:
            type_indicator_out = 2;
            break;
          default:
            type_indicator_out = 0;
        }
        break;
      // ###########################################################
      default:
        type_indicator_out = 0;
    }

  return type_indicator_out;
}


void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

int separate(String str, char **p, int size){
    int n;
    char s[100];

    strcpy(s, str.c_str());

    *p++ = strtok (s, ",");
    for (n = 1; NULL != (*p++ = strtok (NULL, ",")); n++)
        if (size == n)
            break;

    return n;
}

//============
/*This function parses the data that hase been receved*/
void parseData() {      // split the data into its parts
  /*Value extracted from buffer*/
  char * strtokIndx;
  /*Counts the number of parameters after the anomation value in current data parsing.*/
  int param_buffer_count = 0;
  /*
  * Parameter type value indicator.
  * 0: int
  * 1: float
  * 2: unsigned long
  */
  int type_indicator = 0;
  /*
  * The variable that holds the value of the parameter once casted to an int.
  */
  int parameter_value_int;
  /*
  * The variable that holds the value of the parameter once casted to an float.
  */
  float parameter_value_float;
  /*
  * The variable that holds the value of the parameter once casted to an unsigned long.
  */
  unsigned long parameter_value_unsigned_long;

  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  current_array = atoi(strtokIndx); // The current array being manipulated
  // The second element in the list
  strtokIndx = strtok(NULL, ",");
  animation = atoi(strtokIndx); // The animation to be switched to
  
  strtokIndx = strtok(NULL, ",");
  // For each subsequent parameter define apropriate type and sets its value in the the appropriate reference table.
  while(strtokIndx != NULL) {

    // Finds the type of the current parameter value
    type_indicator = find_param_type(animation, param_buffer_count);

    // This switch casts the parameter to the appropriate type and sets the appropriate possition in the appropriat table with that value.
    switch (type_indicator) {
      case 1:
        parameter_value_float = atof(strtokIndx);
        lcd.clear();
        lcd.print("param_float:");
        lcd.setCursor(0, 1);
        delay(3000);
        lcd.print(parameter_value_float);        
        Serial.print("param_float:"); Serial.println(parameter_value_float);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parameters_float, current_array, animation, param_buffer_count, parameter_value_float);
        break;
      case 2:
        parameter_value_unsigned_long  = (unsigned long)atoi(strtokIndx);
        parameter_value_int  = atoi(strtokIndx);
        lcd.clear();
        lcd.print("param_long:");
        lcd.setCursor(0, 1);
        delay(3000);
        lcd.print(parameter_value_unsigned_long);
        Serial.print("param_long:"); Serial.println(parameter_value_unsigned_long);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parameters_unsigned_long, current_array, animation, param_buffer_count, parameter_value_unsigned_long);
        break;
      default:
        parameter_value_int  = atoi(strtokIndx);
        lcd.clear();
        lcd.print("param_int:");
        lcd.setCursor(0, 1);
        lcd.print(parameter_value_int);
        delay(3000);
        Serial.print("param_int:"); Serial.println(parameter_value_int);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parameters_int, current_array, animation, param_buffer_count, parameter_value_int);
    }
    

    // For each iteration of the loop will tell 
    param_buffer_count++;
    // Leave this at end
    strtokIndx = strtok(NULL, ",");
  }

  /* Original code
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  integerFromPC = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ",");
  floatFromPC = atof(strtokIndx);     // convert this part to a float
  */
}

// ###########################################


void read_input(){
  recvWithStartEndMarkers();
  if (newData == true) {
      strcpy(tempChars, receivedChars);
          // this temporary copy is necessary to protect the original data
          //   because strtok() used in parseData() replaces the commas with \0
      parseData();
      //showParsedData();
      newData = false;
  }
}

int counter = 0;
void setup(){
  // set up the LCD's number of columns and rows:
  lcd.begin();
  lcd.backlight();
  // Print a message to the LCD.
  lcd.print("hello, world!");
  // Setting up console output.
  Serial.begin(9600);//9600
  // Setting the time the program will wait for input.
  Serial.setTimeout(100);
}

void loop() {
  if(Serial.available() > 0){
    Serial.print("Got message!");
    counter = 0;
    lcd.clear();
    //lcd.print(Serial.readString());
    read_input();
    lcd.print("anim:");
    lcd.setCursor(0, 1);
    lcd.print(animation);
    delay(3000);
  }
  lcd.setCursor(0, 1);
  lcd.print(counter);
  counter++;


}