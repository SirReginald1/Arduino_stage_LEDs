
#include "driver/uart.h"
#include "SD_manager.h"
#include "FS.h"
#include "SD_MMC.h"


void setup(){
  Serial.begin(115200);
  
  SDManager::setup();

  Serial.println("listing directories:\n");
  SDManager::listDir(SD_MMC, "/", 1);

  Serial.println("Reading file:\n");
  // Variable to hold the number of floats read from the file
    int numFloats = 0;

    // Binary file containing the floats
    const char* filename = "/Vibe Chemistry & HARLEE - Same Old Song_mp3.txt";  // Path to the binary file on the SD card

    // Read floats from the binary file on the SD card
    float* floats = SDManager::readTimingTxtFile<float>(SD_MMC, filename, &numFloats);

    // If successful, print the floats
    if (floats != NULL) {
        Serial.println("Floats read from the binary file on SD card:");
        for (int i = 0; i < numFloats; i++) {
            Serial.println(floats[i], 6);  // Print floats with 6 decimal places
        }

        // Free the allocated memory after use
        free(floats);
    }

}

void loop(){

}