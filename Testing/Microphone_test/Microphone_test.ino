#include "Microphone.h"

long Buffer[64];
double vReal[64];
unsigned long newTime;

void setup(){
  Serial.begin(115200);
  Microphone::setup(150000);
}

void loop() {
  newTime = micros();
  for(int s=0;s<1000000;s++){
    Microphone::readMic(Buffer);
    for(int i=0;i<64;i++){
      vReal[i] = (double)Buffer[i];
      //Serial.println(Buffer[i]);
      s++;
    }
    for(int i=0;i<64;i++){
      vReal[i] = 0;
    }
  }
  float conversionTime = (micros() - newTime) / 1000000.0;

  Serial.print("Conversion time: ");
  Serial.print(conversionTime);
  Serial.println(" uS");

  Serial.print("Max sampling frequency: ");
  Serial.print((1.0 / conversionTime) * 1000000);
  Serial.println(" Hz");


}