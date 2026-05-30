/* Use this file to store private credentials that won't be uploaded to Github.
   Create this file private-credentials.local.h using the template private-credentials.h and specify your WIFI_SSID and WIFI_PASSWORD here
 */
#include "private-credentials.local.h"

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

// WIFI
const char* WIFI_SSID_STRING = WIFI_SSID;
const char* WIFI_PASSWORD_STRING = WIFI_PASSWORD;
WiFiUDP udp;

// OSC
const IPAddress OSC_BROADCAST_IP(255, 255, 255, 255);
const unsigned int OSC_PORT = 8000;

// Onboard LED
#define RGB_LED_PIN 48
#define RGB_LED_COUNT 1
#define RGB_LED_BRIGHTNESS 15
Adafruit_NeoPixel onboardRGBLED(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

/*
  PERFEXMOD PIN ASSIGNMENTS
  Analog: Prefer GPIO 1-10 (ADC1) for stability with WiFi active?
  Digital: Any GPIO except 19, 20 (USB) and 26-32 (internal flash)
 */

#define PERFEXMOD_KNOB_LOUDSPEAKER_VOLUME_PIN 4
#define PERFEXMOD_KNOB_3_PIN 5
#define PERFEXMOD_KNOB_2_PIN 6
#define PERFEXMOD_KNOB_1_PIN 7

#define PERFEXMOD_MICROPHONE_PIN 8
// #define PERFEXMOD_LOUDSPEAKER_PIN // TODO

#define PERFEXMOD_BUTTON_TONE_PIN 42  // Normally open switch
#define PERFEXMOD_BUTTON_ALARM_ON_PIN 41 // Normally closed switch
#define PERFEXMOD_BUTTON_VOICE_RECORDER_SWITCH_PIN 40
#define PERFEXMOD_BUTTON_HOT_SHOT_ON_PIN 39 // Normally closed switch

#define PERFEXMOD_BUTTON_INTERCOM_PIN 38 // Normally closed switch
#define PERFEXMOD_BUTTON_4_PIN 37 // Normally closed switch
#define PERFEXMOD_BUTTON_3_PIN 36 // Normally closed switch
#define PERFEXMOD_BUTTON_2_PIN 35 // Normally closed switch
#define PERFEXMOD_BUTTON_1_PIN 34 // Normally closed switch


void setup() {
  Serial.begin(115200);

  onboardRGBLED.begin();
  onboardRGBLED.setBrightness(RGB_LED_BRIGHTNESS);
  onboardRGBLED.show();

  // Connect to WiFi
  WiFi.begin(WIFI_SSID_STRING, WIFI_PASSWORD_STRING);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi! IP Address: ");
  Serial.println(WiFi.localIP());

  // Start UDP for OSC broadcasting
  udp.begin(OSC_PORT);
  Serial.print("Broadcasting OSC on port ");
  Serial.println(OSC_PORT);

  // Use INPUT_PULLUP — no need to use resistors!
  pinMode(PERFEXMOD_BUTTON_TONE_PIN, INPUT_PULLUP); 
}

bool isNormallyOpenButtonPressed(int buttonPin) {
  return !!(digitalRead(buttonPin) == LOW);
}

bool isNormallyClosedButtonPressed(int buttonPin) {
  return !!(digitalRead(buttonPin) == HIGH);
}

void sendOSCMessage(const char* address, int value) {
  OSCMessage msg(address);
  msg.add(value);
  udp.beginPacket(OSC_BROADCAST_IP, OSC_PORT);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
}

bool previousButtonStateTone = false;

void loop() {
  bool isButtonPressedTone = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_TONE_PIN);
  if (isButtonPressedTone != previousButtonStateTone) {
    previousButtonStateTone = isButtonPressedTone;
    if (isButtonPressedTone) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_tone", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_tone", 0);
    }
    onboardRGBLED.show();
  }
}