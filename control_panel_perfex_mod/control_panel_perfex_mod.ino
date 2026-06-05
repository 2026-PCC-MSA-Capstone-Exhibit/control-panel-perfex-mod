/* Use this file to store private credentials that won't be uploaded to Github.
   Create this file private_credentials.local.h using the template private_credentials.h and specify your WIFI_SSID and WIFI_PASSWORD here
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

// Buttons
#define PERFEXMOD_BUTTON_A_PIN 42 // This switch maintains state
#define PERFEXMOD_BUTTON_B_PIN 41 // Normally closed switch
#define PERFEXMOD_BUTTON_C_PIN 40  // Normally open switch
#define PERFEXMOD_BUTTON_D_PIN 39 // Normally closed switch

#define PERFEXMOD_BUTTON_1_PIN 38 // Normally closed switch
#define PERFEXMOD_BUTTON_2_PIN 37 // Normally closed switch
#define PERFEXMOD_BUTTON_3_PIN 36 // Normally closed switch
#define PERFEXMOD_BUTTON_4_PIN 35 // Normally closed switch
#define PERFEXMOD_BUTTON_5_PIN 47 // Normally closed switch

// LEDs
#define PERFEXMOD_LED_1_PIN 15 // "Microphone" Indicator
#define PERFEXMOD_LED_2_PIN 16 // Red light
#define PERFEXMOD_LED_3_PIN 17 // Yellow light
#define PERFEXMOD_LED_4_PIN 18 // Green Light


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
  pinMode(PERFEXMOD_BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_4_PIN, INPUT_PULLUP);
  pinMode(PERFEXMOD_BUTTON_5_PIN, INPUT_PULLUP);

}

bool isNormallyOpenButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == LOW;
}

bool isNormallyClosedButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == HIGH;
}

void sendOSCMessage(const char* address, int value) {
  OSCMessage msg(address);
  msg.add(value);
  udp.beginPacket(OSC_BROADCAST_IP, OSC_PORT);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
}

bool previousButtonStateA = false;
bool previousButtonStateB = false;
bool previousButtonStateC = false;
bool previousButtonStateD = false;
bool previousButtonState1 = false;
bool previousButtonState2 = false;
bool previousButtonState3 = false;
bool previousButtonState4 = false;
bool previousButtonState5 = false;
int previousKnobSpeakerVolumeValue = -1;

void loop() {

  // Buttons

  // TODO: Refactor this code to have less repetition

  bool isButtonPressedA = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_A_PIN);
  if (isButtonPressedA != previousButtonStateA) {
    previousButtonStateA = isButtonPressedA;
    if (isButtonPressedA) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_A", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_A", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressedB = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_B_PIN);
  if (isButtonPressedB != previousButtonStateB) {
    previousButtonStateB = isButtonPressedB;
    if (isButtonPressedB) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_B", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_B", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressedC = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_C_PIN);
  if (isButtonPressedC != previousButtonStateC) {
    previousButtonStateC = isButtonPressedC;
    if (isButtonPressedC) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_C", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_C", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressedD = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_D_PIN);
  if (isButtonPressedD != previousButtonStateD) {
    previousButtonStateD = isButtonPressedD;
    if (isButtonPressedD) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_D", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_D", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressed1 = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_1_PIN);
  if (isButtonPressed1 != previousButtonState1) {
    previousButtonState1 = isButtonPressed1;
    if (isButtonPressed1) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_1", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_1", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressed2 = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_2_PIN);
  if (isButtonPressed2 != previousButtonState2) {
    previousButtonState2 = isButtonPressed2;
    if (isButtonPressed2) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_2", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_2", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressed3 = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_3_PIN);
  if (isButtonPressed3 != previousButtonState3) {
    previousButtonState3 = isButtonPressed3;
    if (isButtonPressed3) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_3", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_3", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressed4 = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_4_PIN);
  if (isButtonPressed4 != previousButtonState4) {
    previousButtonState4 = isButtonPressed4;
    if (isButtonPressed4) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_4", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_4", 0);
    }
    onboardRGBLED.show();
  }

  bool isButtonPressed5 = isNormallyOpenButtonPressed(PERFEXMOD_BUTTON_5_PIN);
  if (isButtonPressed5 != previousButtonState5) {
    previousButtonState5 = isButtonPressed5;
    if (isButtonPressed5) {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
      sendOSCMessage("/perfexmod/button_5", 1);
    } else {
      onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
      sendOSCMessage("/perfexmod/button_5", 0);
    }
    onboardRGBLED.show();
  }

  // Loudspeaker volume knob
  int rawKnobSpeakerVolumeValue = analogRead(PERFEXMOD_KNOB_4_PIN);
  int knobSpeakerVolumeValue = constrain(map(rawKnobSpeakerVolumeValue, 400, 4095, 0, 4095), 0, 4095);
  if (knobSpeakerVolumeValue != previousKnobSpeakerVolumeValue) {
    previousKnobSpeakerVolumeValue = knobSpeakerVolumeValue;
    Serial.print("knobSpeakerVolumeValue ");
    Serial.println(knobSpeakerVolumeValue);
    sendOSCMessage("/perfexmod/knob_4", knobSpeakerVolumeValue);
  }
}

// TODO: Pot values are jittery probably due to not being properly grounded?
// TODO: Knobs 1,2,3 are not responsive until 3rd value. verify after soldering and adjust knob position if needed?