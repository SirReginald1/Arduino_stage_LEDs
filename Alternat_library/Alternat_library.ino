#include <Adafruit_NeoPixel.h>

#define PIN_ARRAY1 4
#define NB_LEDS 122

Adafruit_NeoPixel *pixels;

void setup() {
  // put your setup code here, to run once:
  pixels = new Adafruit_NeoPixel(NB_LEDS, PIN_ARRAY1,  800000);
  pixels->begin();
  //pixels->clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  pixels->clear();
  for(int i=0; i<NB_LEDS; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 200));
  }
    pixels->show();
  delay(3000);
  
}
