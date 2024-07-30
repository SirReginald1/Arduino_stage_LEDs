#include <Serial.h>
#include <string.h>

#define MAX_NB_PARAM 10

extern int animation;
extern int current_array;
extern int *parameters;
extern int param_count;

void read_input(){
    current_array = Serial.parseInt();
    animation = Serial.parseInt();

    char *serial_buffer = Serial.readString();

    char *params;
    int parameters[MAX_NB_PARAM];
    int param_count = 0;
    
    params = strtok(serial_buffer, ",");
    while(params != NULL) {
        printf("%s \n", params);
        parameters[param_count] = atoi(params);
        params = strtok(NULL, ",");
        param_count++;
    }


}
