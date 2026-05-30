// No installation required: These WiFi libraries are already built into Arduino Core for ESP32.
#include "WiFi.h"
#include <WiFiUdp.h>

/* 
  Install via Sketch > Include Library > Add .ZIP Library... > [Select OSC-master.zip from the Libraries folder of this repo]
  Source: https://github.com/CNMAT/OSC
*/
#include <OSCMessage.h>

// Install via Library Manager: Adafruit NeoPixel by Adafruit
#include <Adafruit_NeoPixel.h>

// Onboard LED
#define RGB_LED_PIN 48
#define RGB_LED_COUNT    1
#define RGB_LED_BRIGHTNESS  15

// Pins dedicated to buttons
#define PERFEX_TONE_BUTTON_PIN  4
#define PERFEX_ALARM_ON_BUTTON_PIN 5
#define PERFEX_VOICE_RECORDER_SWITCH_BUTTON_PIN 6
#define PERFEX_HOT_SHOT_ON_BUTTON_PIN 7

#define PERFEX_INTERCOM_BUTTON_PIN 15
#define PERFEX_BUTTON_4_PIN 16
#define PERFEX_BUTTON_3_PIN 17
#define PERFEX_BUTTON_2_PIN 18
#define PERFEX_BUTTON_1_PIN 8

// Pins dedicated to Perfex knobs
#define PERFEX_KNOB_LOUDSPEAKER_VOLUME_PIN 3
#define PERFEX_KNOB_3_PIN 46
#define PERFEX_KNOB_2_PIN 9
#define PERFEX_KNOB_1_PIN 10

// Pins dedicated to other Perfex features



Adafruit_NeoPixel onboardRGBLED(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  onboardRGBLED.begin();
  onboardRGBLED.setBrightness(RGB_LED_BRIGHTNESS);
  onboardRGBLED.show();

  // Use INPUT_PULLUP — no need to use resistors!
  pinMode(PERFEX_TONE_BUTTON_PIN, INPUT_PULLUP); 
}

bool isNormallyOpenButtonPressed(int buttonPinState) {
  return !!(digitalRead(buttonPinState) == LOW);
}

bool isNormallyClosedButtonPressed(int buttonPinState) {
  return !!(digitalRead(buttonPinState) == HIGH);
}

void loop() {
  if (isNormallyOpenButtonPressed(PERFEX_TONE_BUTTON_PIN)) {
    onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
  } else {
    onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
  }
  onboardRGBLED.show();
}