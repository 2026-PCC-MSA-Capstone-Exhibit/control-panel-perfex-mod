#include <Adafruit_NeoPixel.h>

#define RGB_LED_PIN 48
#define PERFEX_INTERCOM_BUTTON_PIN  4
#define RGB_LED_COUNT    1
#define RGB_LED_BRIGHTNESS  15

Adafruit_NeoPixel onboardRGBLED(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  onboardRGBLED.begin();
  onboardRGBLED.setBrightness(RGB_LED_BRIGHTNESS);
  onboardRGBLED.show();

  // Use INPUT_PULLUP — no need to use resistors!
  pinMode(PERFEX_INTERCOM_BUTTON_PIN, INPUT_PULLUP); 
}

bool isMomentaryButtonPressed(int buttonPinState) {
  return !!(digitalRead(buttonPinState) == HIGH);
}

void loop() {
  if (isMomentaryButtonPressed(PERFEX_INTERCOM_BUTTON_PIN)) {
    onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
  } else {
    onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
  }
  onboardRGBLED.show();
}