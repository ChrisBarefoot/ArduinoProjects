#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(256, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(80);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  showBlank();
}

// Fill the dots one after the other with a color
void showBlank()
{
  for(uint16_t i = 0; i < strip.numPixels(); i++)
  {
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}



