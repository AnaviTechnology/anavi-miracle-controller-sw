//this needs to be first, or it all crashes and burns...
#include <FS.h>

//https://github.com/esp8266/Arduino
#include <ESP8266WiFi.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>

#define LED_PIN1    12
#define LED_PIN2    14
#define NUM_LEDS    8
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

// rotating "base color" used by many of the patterns
uint8_t gHue1 = 0;
uint8_t gHue2 = 120;

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Loading...");

  // Power-up safety delay
  delay(3000);
  Serial.println("Hello addressable LED strip!");
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  // Turn on lights
  rainbow(leds1, gHue1);
  rainbow(leds2, gHue2);
  FastLED.show();
  delay(100);
  Serial.println("Ready, steady, go!");
}

void loop()
{
  // Set different animations for each LED strip
  rainbow(leds1, gHue1);
  sinelon(leds2, gHue2);

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20)
  {
    // slowly cycle the "base color" through the rainbow
    gHue1++;
    gHue2++;
    // Bring back hue in range
    if (360 <= gHue1)
    {
      gHue1 = 0;
    }
    if (360 <= gHue1)
    {
      gHue2 = 0;
    }
  }
}

void rainbow(CRGB *leds, uint8_t gHue)
{
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void sinelon(CRGB *leds, uint8_t gHue)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void confetti(CRGB *leds, uint8_t gHue) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void bpm(CRGB *leds, uint8_t gHue)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle(CRGB *leds, uint8_t gHue) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++)
  {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
