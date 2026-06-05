/* Create the file private_credentials.local.h to store private credentials that won't be uploaded to Github.
   Duplicate the template private_credentials.h and specify your WIFI_SSID and WIFI_PASSWORD there
 */
#include "private_credentials.local.h"

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

/* PERFEXMOD PIN ASSIGNMENTS */

// Knobs
#define PERFEXMOD_KNOB_1_PIN 4
#define PERFEXMOD_KNOB_2_PIN 5
#define PERFEXMOD_KNOB_3_PIN 1
#define PERFEXMOD_KNOB_4_PIN 2

// Buttons
#define PERFEXMOD_BUTTON_A_PIN 42
#define PERFEXMOD_BUTTON_B_PIN 41
#define PERFEXMOD_BUTTON_C_PIN 40
#define PERFEXMOD_BUTTON_D_PIN 39

#define PERFEXMOD_BUTTON_1_PIN 38
#define PERFEXMOD_BUTTON_2_PIN 37
#define PERFEXMOD_BUTTON_3_PIN 36
#define PERFEXMOD_BUTTON_4_PIN 35
#define PERFEXMOD_BUTTON_5_PIN 47

// LEDs
#define PERFEXMOD_LED_1_PIN 15 // "Microphone" Indicator
#define PERFEXMOD_LED_2_PIN 16 // Red light
#define PERFEXMOD_LED_3_PIN 17 // Yellow light
#define PERFEXMOD_LED_4_PIN 18 // Green Light

// Microphone (MAX9814): amazon.com/dp/B0B7SP6GYX
#define PERFEXMOD_MICROPHONE_PIN 6

// Speaker (MAX98357A): amazon.com/dp/B0B4GK5R1R
#define PERFEXMOD_SPEAKER_BCLK_PIN 46
#define PERFEXMOD_SPEAKER_LRC_PIN 9
#define PERFEXMOD_SPEAKER_DIN_PIN 10

// SD Card Reader: amazon.com/dp/B0BV8ZQ81F
#define PERFEXMOD_SD_MOSI_PIN 11
#define PERFEXMOD_SD_MISO_PIN 12
#define PERFEXMOD_SD_SCK_PIN 13
#define PERFEXMOD_SD_CS_PIN 14


const int BUTTON_COUNT = 9;

const int BUTTON_PINS[BUTTON_COUNT] = {
  PERFEXMOD_BUTTON_A_PIN,
  PERFEXMOD_BUTTON_B_PIN,
  PERFEXMOD_BUTTON_C_PIN,
  PERFEXMOD_BUTTON_D_PIN,
  PERFEXMOD_BUTTON_1_PIN,
  PERFEXMOD_BUTTON_2_PIN,
  PERFEXMOD_BUTTON_3_PIN,
  PERFEXMOD_BUTTON_4_PIN,
  PERFEXMOD_BUTTON_5_PIN
};
const bool IS_BUTTON_NORMALLY_OPEN[BUTTON_COUNT] = {
  true, // BUTTON_A - Note: This switch maintains state // TODO: Confirm this
  false, // BUTTON_B
  true,  // BUTTON_C
  false, // BUTTON_D
  false, // BUTTON_1
  false, // BUTTON_2
  false, // BUTTON_3
  false, // BUTTON_4
  false  // BUTTON_5
};
const char* BUTTON_OSC_ADDRESSES[BUTTON_COUNT] = {
  "/perfexmod/button_A",
  "/perfexmod/button_B",
  "/perfexmod/button_C",
  "/perfexmod/button_D",
  "/perfexmod/button_1",
  "/perfexmod/button_2",
  "/perfexmod/button_3",
  "/perfexmod/button_4",
  "/perfexmod/button_5"
};

bool previousButtonStates[BUTTON_COUNT] = { false };
int previousKnobSpeakerVolumeValue = -1;

bool isNormallyOpenButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == LOW;
}

bool isNormallyClosedButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == HIGH;
}

int previousMicValue = -1;


void sendOSCMessage(const char* address, int value) {
  OSCMessage msg(address);
  msg.add(value);
  udp.beginPacket(OSC_BROADCAST_IP, OSC_PORT);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
}


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
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

}

void loop() {

  // BUTTONS
  for (int i = 0; i < BUTTON_COUNT; i++) {
    // TODO: Confirm button A is normally open?
    bool isPressed = IS_BUTTON_NORMALLY_OPEN[i]
      ? isNormallyOpenButtonPressed(BUTTON_PINS[i])
      : isNormallyClosedButtonPressed(BUTTON_PINS[i]);
    if (isPressed != previousButtonStates[i]) {
      previousButtonStates[i] = isPressed;
      if (isPressed) {
        onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
        sendOSCMessage(BUTTON_OSC_ADDRESSES[i], 1);
      } else {
        onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
        sendOSCMessage(BUTTON_OSC_ADDRESSES[i], 0);
      }
      onboardRGBLED.show();
    }
  }

  // KNOBS
  // TODO: Refactor to work for all knobs
  int rawKnobSpeakerVolumeValue = analogRead(PERFEXMOD_KNOB_1_PIN);
  Serial.print("rawKnobSpeakerVolumeValue: ");
  Serial.println(rawKnobSpeakerVolumeValue);
  int knobSpeakerVolumeValue = constrain(map(rawKnobSpeakerVolumeValue, 400, 4095, 0, 127), 0, 127);
  if (knobSpeakerVolumeValue != previousKnobSpeakerVolumeValue) {
    previousKnobSpeakerVolumeValue = knobSpeakerVolumeValue;
    Serial.print("knobSpeakerVolumeValue: ");
    Serial.println(knobSpeakerVolumeValue);
    sendOSCMessage("/perfexmod/knob_1", knobSpeakerVolumeValue);
  }

  // MICROPHONE (MAX9814)
  int rawMicValue = analogRead(PERFEXMOD_MICROPHONE_PIN);
  int micValue = constrain(map(rawMicValue, 0, 4095, 0, 127), 0, 127);
  if (micValue != previousMicValue) {
    previousMicValue = micValue;
    Serial.print("micValue: ");
    Serial.println(micValue);
    sendOSCMessage("/perfexmod/mic", micValue);
  }

}

// TODO: Pot values are jittery probably due to not being properly grounded?
// TODO: Knobs 1,2,3 are not responsive until 3rd value. verify after soldering and adjust knob position if needed?