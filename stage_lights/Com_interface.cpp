#include "HardwareSerial.h"
#include <string.h>
#include <Arduino.h>
#include "Animation.h"
#include <math.h>

extern int animation;
extern int current_array;
extern int *parameters;
extern int param_count;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}



void read_input(){

  int count = 0;
  int power = 1;

  bool array_flag = true;
  bool animation_flag = true;
  param_count = 0;

  char letter;

  while(Serial.available()>0){

    String in = Serial.readString();

    Serial.println(in[4]);

    Serial.print("Letter: ");Serial.println(letter);
    if(((int)letter) > 47 && ((int)letter) < 58){
      count += (int)pow((double((int)letter)-48), (double)power);
      power++;
    }
    else{
      if(array_flag){
        Serial.print("Current_array: ");Serial.println(count);
        current_array = count;
        array_flag = false;
      }
      else if (animation_flag) {
        Serial.print("Anim: ");Serial.println(count);
        animation = count;
        animation_flag = false;
      }
      else{
        Serial.print("param ");Serial.print(param_count);Serial.print(": ");Serial.println(count);
        parameters[param_count] = count;
        param_count++;
      }

      count = 0;
      power = 1;
    }

  }

  Serial.print("Arrays: ");Serial.println(current_array);
  Serial.print("Animation: ");Serial.println(animation);
  for(int i=0; i<param_count;i++){
    Serial.println();
    Serial.print("Param: ");Serial.println(parameters[i]);
  }

  //Serial.println();Serial.println();
    //current_array = Serial.parseInt(SKIP_ALL);
    //Serial.print("Current array: ");Serial.println(current_array);
    //Serial.println();
    //animation = Serial.parseInt(SKIP_ALL);
    //Serial.print("Animation: ");Serial.println(animation);
    //Serial.println();
    //Serial.read();

    //String serial_buffer = Serial.readString();
    //Serial.print("Serial_buffer: ");Serial.println(serial_buffer);

    //while(Serial.available() > 0) {
    //  char t = Serial.read();
    //}

    //String params;
    //int parameters[MAX_NB_PARAM];
    //param_count = 0;
    //Serial.read();
//Serial.println();
    //while (Serial.available() > 0) {
    //  parameters[param_count] = Serial.parseInt(SKIP_ALL);
    //  param_count++;
    //}
//
    //for(int i=0; i<param_count+1; i++){
    //  Serial.print("Param: "); Serial.println(parameters[i]);
    //}
    //serialFlush();

    //params = strtok(serial_buffer.c_str(), ",");
    /*
    while(params != NULL) {
        //printf("%s \n", params);
        parameters[param_count] = atoi(params.c_str());
        Serial.println();
        Serial.print("Parameters: ");Serial.println(parameters[param_count]);
        Serial.println();
        params = strtok(NULL, ",");
        param_count++;
    }
    */
  //param_count--;
  //Serial.print("Final count: ");Serial.println(param_count);
}

void debug_read(){
  Serial.println();Serial.println();
    current_array = Serial.parseInt(SKIP_ALL);
    Serial.print("Current array: ");Serial.println(current_array);
}

