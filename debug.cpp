#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    /*
    printf("Start");

    char *serial_buffer = "20,60,255,255,255";
    printf("Serial: %s", serial_buffer);
    char *params = strtok(serial_buffer, ",");
    printf("Params:");

    while(params != NULL) {
       printf("%s \n", params);
       params = strtok(NULL, ",");
    }
    return 0;
    */
    char stng[100] = "20,60,255,255,255";
    char *params;
    int parameters[10];
    int count = 0;
    
    params = strtok(stng, ",");
    while(params != NULL) {
       printf("%s \n", params);
        parameters[count] = atoi(params);
       params = strtok(NULL, ",");
       count++;
    }

    printf("Integers");
    for(int i=0; i<count;i++){
        printf("Param: %d", parameters[i]);
    }
    

    return 0;
}