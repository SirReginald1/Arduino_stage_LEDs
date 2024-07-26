#include "Animation.h"
#include <FastLED.h>

#define NUM_LEDS 122

  // FlameHeight - Use larger value for shorter flames, default=50.
  // Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
  // DelayDuration - Use larger value for slower flame speed, default=10.

  void Fire(CRGB* leds, int FlameHeight, int Sparks, int DelayDuration, float intensity) {
    static byte heat[NUM_LEDS];
    int cooldown;

    // Cool down each cell a little
    for(int i = 0; i < NUM_LEDS; i++) {
      cooldown = random(0, ((FlameHeight * 10) / NUM_LEDS) + 2);

      if(cooldown > heat[i]) {
        heat[i] = 0;
      }
      else {
        heat[i] = heat[i] - cooldown;
      }
    }

    // Heat from each cell drifts up and diffuses slightly
    for(int k = (NUM_LEDS - 1); k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Randomly ignite new Sparks near bottom of the flame
    if(random(255) < Sparks) {
      int y = random(7);
      heat[y] = heat[y] + random(160, 255);
    }

    // Convert heat to LED colors
    for(int j = 0; j < NUM_LEDS; j++) {
      setPixelHeatColor(leds, j, heat[j], intensity);
    }

    FastLED.show();
    delay(DelayDuration);
  }

  void setPixelHeatColor(CRGB* leds, int Pixel, byte temperature, float intensity) {
    // Rescale heat from 0-255 to 0-191
    byte t192 = round((temperature / 255.0) * 191);

    // Calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0...63
    heatramp <<= 2; // scale up to 0...252

    // Figure out which third of the spectrum we're in:
    if(t192 > 0x80) {                    // hottest
      leds[Pixel].setRGB(255*intensity, heatramp*intensity, 255*intensity);
    }
    else if(t192 > 0x40) {               // middle
      leds[Pixel].setRGB(heatramp*intensity, 255*intensity, 0);
    }
    else {                               // coolest
      leds[Pixel].setRGB(0, heatramp*intensity, 0);
    }
  }

  /* 
   *  ========== Shooting Star Animation ==========
   *  red, green, blue - Choose a color with RGB values (0 to 255).
   *  tail_length - A larger value results in shorter tail.
   *  delay_duration - A larger value results in slower movement speed.
   *  interval - Time interval between new shooting stars (in milliseconds).
  */

  unsigned long shootingStarPreviousMillis = 0;           // Stores last time LEDs were updated
  int shootingStarCount = 0;                              // Stores count for incrementing up to the NUM_LEDs

  void shootingStarAnimation(CRGB* leds, int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction, unsigned long currentMillis){
    /*
     * red - 0 to 255 red color value
     * green - 0 to 255 green color value
     * blue - 0 to 255 blue color value
     * tail_length - value which represents number of pixels used in the tail following the shooting star
     * delay_duration - value to set animation speed. Higher value results in slower animation speed.
     * interval - time between each shooting star (in miliseconds)
     * direction - value which changes the way that the pixels travel (uses -1 for reverse, any other number for forward)
    */
    //unsigned long currentMillis = millis();   // Get the time
    if (currentMillis - shootingStarPreviousMillis >= interval) {
      shootingStarPreviousMillis = currentMillis;         // Save the last time the LEDs were updated
      shootingStarCount = 0;                              // Reset the count to 0 after each interval
    }
    if (direction == -1) {        // Reverse direction option for LEDs
      if (shootingStarCount < NUM_LEDS) {
        leds[NUM_LEDS - (shootingStarCount % (NUM_LEDS+1))].setRGB(red, green, blue);    // Set LEDs with the color value
        shootingStarCount++;
      }
    }
    else {
      if (shootingStarCount < NUM_LEDS) {     // Forward direction option for LEDs
        leds[shootingStarCount % (NUM_LEDS+1)].setRGB(red, green, blue);    // Set LEDs with the color value
        shootingStarCount++;
      }
    }
    fadeToBlackBy(leds, NUM_LEDS, tail_length);                 // Fade the tail LEDs to black
    FastLED.show();
    delay(delay_duration);                                      // Delay to set the speed of the animation
  }

  /*
    *  ========== Twinkle_Pixels_Animation ==========
    *  PARAMETERS:
    *   leds - The array of leds to make twinkle
    *   Color - select the color of the animation with one of the following numbers (or anything in between) [range = 0 to 255; default = random(256)]:
          Red: 0
          Orange: 30
          Yellow: 42
          Green: 85
          Cyan: 127
          Blue: 170
          Purple: 210
          Pink: 218
    *   ColorSaturation - Sets the color saturation according to HSV color code. Set to 0 for white and set to 255 for max saturation
    *                     [max range = 0 to 255; default = 255]
    *   PixelVolume - Use a larger value for less LEDs to twinkle [default = 20]
    *   FadeAmount - Use a larger value for a faster fade amount [max range = 0 to 255; default = 50]
    *   DelayDuration - Use a larger value for a slower animation speed [default = 50]
    *  
    *  NOTES:
    *  - You can combine twinkling colors by putting two of the function calls together in the loop() function. Try it out!
    *  - You can get many unique effects by changing the various arguments. Here are some examples to try:
    *      TwinklePixels(170, 255, 70, 20, 50);   // Blue color with lower pixel volume and slower speed
    *      TwinklePixels(0, 255, 40, 80, 80);     // Red color with moderate pixel volume and moderate speed
    *      TwinklePixels(85, 200, 15, 50, 30);    // Green color with high pixel volume and faster speed
    *      TwinklePixels(255, 0, 100, 120, 0);    // White color with low pixel volume and max speed
  */

  void TwinklePixels(CRGB* leds, int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration) {
    for (int i = 0; i < 122; i++) {
      // Draw twinkling pixels
      if (random(PixelVolume) < 2) {     // Chance for pixel to twinkle
        uint8_t intensity = random(100, 255);     // Configure random intensity
        CRGB set_color = CHSV(Color, ColorSaturation, intensity);     // Configure color with max saturation and variable value (intensity)
        leds[i] = set_color;    // Set the pixel color
      }

      // Fade LEDs
      if (leds[i].r > 0 || leds[i].g > 0 || leds[i].b > 0) {    // Check if pixel is lit
        leds[i].fadeToBlackBy(FadeAmount);    // Fade pixel to black
      }
    }

    FastLED.show();
    delay(DelayDuration);
  }

  /*
    * Clears the whole strip sequentialy in the specified direction.
    * 
    * @param leds The array of leds on which to opperate.
    * @param direction If 0 will run from plug to end. Else will run from top to plug.
    * @param speed Length of the delay in millisecs between each led turning off.
  */
  void clear_sequential(CRGB* leds, int direction, int speed){
    if(direction == 0){
      for(int i=0;i<NUM_LEDS;i++){
        leds[i] =  CRGB::Black;
        FastLED.show();
        delay(speed);
      }
    }
    else{
      for(int i=NUM_LEDS;i>-1;i--){
        leds[i] =  CRGB::Black;
        FastLED.show();
        delay(speed);
      }
    }

  }

  void strobe(CRGB* leds, int speed, CRGB color){
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    FastLED.clear();
    FastLED.show();
    delay(speed);
  }


  void fadeAnimation(CRGB* leds, int red, int green, int blue){
    float r, g, b;

    // FADE IN
    for(int i = 0; i <= 255; i++) {
      r = (i/256.0)*red;
      g = (i/256.0)*green;
      b = (i/256.0)*blue;
      fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
      FastLED.show();
      delay(2);
    }

    // FADE OUT
    for(int i = 255; i >= 0; i--) {
      r = (i/256.0)*red;
      g = (i/256.0)*green;
      b = (i/256.0)*blue;
      for(int i=0; i<NUM_LEDS;i++){
        leds[i].setRGB(r, g, b);// = CRGB(r,g,b);
      }
      //fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
      FastLED.show();
      delay(2);
    }
  }

  byte *Wheel(byte WheelPosition) {
    static byte c[3];

    if(WheelPosition < 85) {
     c[0] = WheelPosition * 3;
     c[1] = 255 - WheelPosition * 3;
     c[2] = 0;
    }
    else if(WheelPosition < 170) {
     WheelPosition -= 85;
     c[0] = 255 - WheelPosition * 3;
     c[1] = 0;
     c[2] = WheelPosition * 3;
    }
    else {
     WheelPosition -= 170;
     c[0] = 0;
     c[1] = WheelPosition * 3;
     c[2] = 255 - WheelPosition * 3;
    }

    return c;
  }

  unsigned long rainbowCyclePreviousMillis = 0;  

  void rainbowCycle(CRGB* leds, int DelayDuration, int millisecs) {
    byte *c;
    uint16_t i, j;
    if(millisecs - rainbowCyclePreviousMillis >= DelayDuration){
      for(j=0; j < 256; j++) {
        for(i=0; i < NUM_LEDS; i++) {
          c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
          leds[NUM_LEDS - 1 - i].setRGB(*c, *(c+1), *(c+2));
        }
        FastLED.show();
        //delay(DelayDuration);
      }
    }
  }

  unsigned long zip_animation_prev_mills = 0;
int zip_animation_pos_counter = 0;
void zip_animation(CRGB* leds, int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color){
  int start_pos, end_pos;
  if(current_time - zip_animation_prev_mills > speed){
    // This is the start of the strip
    start_pos = zip_animation_pos_counter + start;
    if(start_pos <= NUM_LEDS && start_pos < end){
      leds[start_pos] = color;
    }

    // This is the end of the strip
    end_pos = zip_animation_pos_counter + start - size;
    if(end_pos >= 0 && end_pos <= NUM_LEDS){
      leds[end_pos] = CRGB::Black;
    }
    FastLED.show();
    zip_animation_pos_counter = ++zip_animation_pos_counter % (end - start + size);
    zip_animation_prev_mills = current_time;
  }
  
}

  void Sparkle(CRGB* leds, int red, int green, int blue, int delayDuration) {
  int pixel = random(NUM_LEDS);
  leds[pixel].setRGB(red, green, blue);
  FastLED.show();
  delay(delayDuration);
  leds[pixel].setRGB(0, 0, 0);
}
/*
void electromagneticSpectrum(int transitionSpeed) {
  switch(colorTime) {
    case 1:
      greenColor += transitionSpeed;
      blueColor -= transitionSpeed;
      if (greenColor >= 255 or blueColor <= 0) {
        greenColor = 255;
        blueColor = 0;
        colorTime = 2;
      }
      break;
    case 2:
      redColor += transitionSpeed;
      greenColor -= transitionSpeed;
      if (redColor >= 255 or greenColor <= 0) {
        redColor = 255;
        greenColor = 0;
        colorTime = 3;
      }
      break;
    case 3:
      redColor -= transitionSpeed;
      blueColor += transitionSpeed;
      if (redColor <= 0 or blueColor >= 255) {
        redColor = 0;
        blueColor = 255;
        colorTime = 1;
      }
      break;
  }
}*/


