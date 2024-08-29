#include <Adafruit_NeoPixel.h>

#define LED_PIN_0 19
#define LED_PIN_1 18
#define LED_PIN_2 17
#define LED_PIN_3 16
#define NB_ARRAYS 4
#define NB_LEDS 122

Adafruit_NeoPixel pixels[NB_ARRAYS];

void setup() {
  // put your setup code here, to run once:
  pixels[0] = Adafruit_NeoPixel(NB_LEDS, LED_PIN_0,  NEO_GRB + NEO_KHZ800);
  pixels[1] = Adafruit_NeoPixel(NB_LEDS, LED_PIN_1,  NEO_GRB + NEO_KHZ800);
  pixels[2] = Adafruit_NeoPixel(NB_LEDS, LED_PIN_2,  NEO_GRB + NEO_KHZ800);
  pixels[3] = Adafruit_NeoPixel(NB_LEDS, LED_PIN_3,  NEO_GRB + NEO_KHZ800);
  pixels[0].begin();
  //pixels->clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels[0].setPixelColor(10,0,255,0);
  pixels[0].show();
  /*
  pixels->clear();
  for(int i=0; i<NB_LEDS; i++) {
    pixels[0]->setPixelColor(i, pixels[0]->Color(0, 0, 200));
  }
    pixels[0]->show();
  delay(3000);
  */
}
