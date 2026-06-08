/*
  WIFI CREDENTIALS:
  Create the file private_credentials.local.h to store private credentials that won't be uploaded to Github.
  Duplicate the template private_credentials.h and specify your WIFI_SSID and WIFI_PASSWORD there
 */
#include "private_credentials.local.h"

/* 
  OSC:
  Installation required!
  Go to Sketch > Include Library > Add .ZIP Library... > [Select OSC-master.zip from the Libraries folder of this repo]
  Source: https://github.com/CNMAT/OSC
 */
#include <OSCMessage.h>
const IPAddress OSC_BROADCAST_IP(255, 255, 255, 255);
const unsigned int OSC_PORT = 8000;

/*
  WIFI:
  No installation required: These WiFi libraries are already built into Arduino Core for ESP32.
 */
#include "WiFi.h"
#include <WiFiUdp.h>
const char* WIFI_SSID_STRING = WIFI_SSID;
const char* WIFI_PASSWORD_STRING = WIFI_PASSWORD;
WiFiUDP udp;

/*
  SD Card Reading:
  No installation required: SD and SPI are already built into Arduino Core for ESP32
 */
#include <SD.h>
#include <SPI.h>

/*
  Audio playback from SD Card:
  Installation Required!
  Install via Library Manager: ESP32-audioI2S by schreibfaul1
*/
#include <Audio.h>
Audio audio;



/*
  LED:
  Install via Library Manager: Adafruit NeoPixel by Adafruit
 */
#include <Adafruit_NeoPixel.h>

/* ------------------------------------------------------------ */


// Onboard LED
#define ONBOARD_RGB_LED_PIN 48
#define ONBOARD_RGB_LED_COUNT 1
#define ONBOARD_RGB_LED_BRIGHTNESS 15
Adafruit_NeoPixel onboardRGBLED(ONBOARD_RGB_LED_COUNT, ONBOARD_RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

/* PERFEXMOD PIN ASSIGNMENTS */

// Knobs
#define PERFEXMOD_KNOB_1_PIN 4
#define PERFEXMOD_KNOB_2_PIN 5
#define PERFEXMOD_KNOB_3_PIN 1
#define PERFEXMOD_KNOB_4_PIN 2

// Buttons
#define PERFEXMOD_BUTTON_A_PIN 42 // "VOICE RECORDER SWITCH"
#define PERFEXMOD_BUTTON_B_PIN 41 // "HOT SHOT ON"
#define PERFEXMOD_BUTTON_1_PIN 40 // "1" of "TRANSMITTER SWITCHES"
#define PERFEXMOD_BUTTON_2_PIN 39 // "2" of "TRANSMITTER SWITCHES"
#define PERFEXMOD_BUTTON_3_PIN 38 // "3" of "TRANSMITTER SWITCHES"
#define PERFEXMOD_BUTTON_4_PIN 7 // "4" of "TRANSMITTER SWITCHES"
#define PERFEXMOD_BUTTON_5_PIN 8 // "IC" of "TRANSMITTER SWITCHES"
#define PERFEXMOD_BUTTON_C_PIN 47 // "TONE"
#define PERFEXMOD_BUTTON_D_PIN 21 // "ALARM ON"
#define PERFEXMOD_BUTTON_E_PIN 3 // "CARD SLOT"

// LEDs
#define PERFEXMOD_LED_1_PIN 15 // "MICROPHONE" Indicator
#define PERFEXMOD_LED_2_PIN 16 // Red light next to "CARD SLOT"
#define PERFEXMOD_LED_3_PIN 17 // Yellow light next to "CARD SLOT"
#define PERFEXMOD_LED_4_PIN 18 // Green Light next to "CARD SLOT"

// Microphone (MAX9814): amazon.com/dp/B0B7SP6GYX
#define PERFEXMOD_MICROPHONE_PIN 6

// Speaker Amplifier (MAX98357A): amazon.com/dp/B0B4GK5R1R
#define PERFEXMOD_SPEAKER_DIN_PIN 46
#define PERFEXMOD_SPEAKER_BCLK_PIN 9
#define PERFEXMOD_SPEAKER_LRC_PIN 10


// SD Card Reader: amazon.com/dp/B0BV8ZQ81F
#define PERFEXMOD_SD_MOSI_PIN 11
#define PERFEXMOD_SD_CLK_PIN 12
#define PERFEXMOD_SD_MISO_PIN 13
#define PERFEXMOD_SD_CS_PIN 14


/* BUTTONS */
const int BUTTON_COUNT = 10;

const int BUTTON_PINS[BUTTON_COUNT] = {
  PERFEXMOD_BUTTON_A_PIN,
  PERFEXMOD_BUTTON_B_PIN,
  PERFEXMOD_BUTTON_C_PIN,
  PERFEXMOD_BUTTON_D_PIN,
  PERFEXMOD_BUTTON_E_PIN,
  PERFEXMOD_BUTTON_1_PIN,
  PERFEXMOD_BUTTON_2_PIN,
  PERFEXMOD_BUTTON_3_PIN,
  PERFEXMOD_BUTTON_4_PIN,
  PERFEXMOD_BUTTON_5_PIN
};
const bool IS_BUTTON_NORMALLY_OPEN[BUTTON_COUNT] = {
  true,  // BUTTON_A - Note: This switch maintains state
  false, // BUTTON_B
  true,  // BUTTON_C
  false, // BUTTON_D
  true,  // BUTTON_E
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
  "/perfexmod/button_E",
  "/perfexmod/button_1",
  "/perfexmod/button_2",
  "/perfexmod/button_3",
  "/perfexmod/button_4",
  "/perfexmod/button_5"
};

bool previousButtonStates[BUTTON_COUNT] = { false };

bool isNormallyOpenButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == LOW;
}

bool isNormallyClosedButtonPressed(int buttonPin) {
  return digitalRead(buttonPin) == HIGH;
}

/* KNOBS */
const int KNOB_COUNT = 4;

const int KNOB_PINS[KNOB_COUNT] = {
  PERFEXMOD_KNOB_1_PIN,
  PERFEXMOD_KNOB_2_PIN,
  PERFEXMOD_KNOB_3_PIN,
  PERFEXMOD_KNOB_4_PIN
};

const char* KNOB_OSC_ADDRESSES[KNOB_COUNT] = {
  "/perfexmod/knob_1",
  "/perfexmod/knob_2",
  "/perfexmod/knob_3",
  "/perfexmod/knob_4"
};

int previousKnobValues[KNOB_COUNT] = { -1, -1, -1, -1 };
int previousMicValue = -1;


/* AUDIO FILES (SOUND LIBRARIES FOR FEATURES) */

// BUTTON: "VOICE RECORDER SWITCH"
const int BUTTON_A_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_A_SOUND_LIBRARY[BUTTON_A_SOUND_LIBRARY_COUNT] = {
  "/freesound_community-microphone-feedback-67484_edited.wav"
};

// BUTTON: "HOT SHOT ON"
const int BUTTON_B_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_B_SOUND_LIBRARY[BUTTON_B_SOUND_LIBRARY_COUNT] = {
  "/wavsource-disconnect-911d.wav"
};

// BUTTON: "TONE"
const int BUTTON_C_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_C_SOUND_LIBRARY[BUTTON_C_SOUND_LIBRARY_COUNT] = {
  "/mixkit-dial-phone-tone-2862_edited-2sec.wav"
};

// BUTTON: "ALARM ON"
const int BUTTON_D_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_D_SOUND_LIBRARY[BUTTON_D_SOUND_LIBRARY_COUNT] = {
  "/mixkit-classic-alarm-995.wav"
};

// BUTTON: "CARD SLOT" physical input
const int BUTTON_E_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_E_SOUND_LIBRARY[BUTTON_E_SOUND_LIBRARY_COUNT] = {
  "/mixkit-shop-scanner-beeps-1073_edited-single.wav"
};

// BUTTON: "1", "2", "3", "4" of "TRANSMITTER SWITCHES"
const int BUTTON_1_4_SOUND_LIBRARY_COUNT = 0;
const char* BUTTON_1_4_SOUND_LIBRARY[BUTTON_1_4_SOUND_LIBRARY_COUNT] = {
};

// BUTTON: 5, "IC (INTERCOM)" of "TRANSMITTER SWITCHES"
const int BUTTON_5_SOUND_LIBRARY_COUNT = 1;
const char* BUTTON_5_SOUND_LIBRARY[BUTTON_5_SOUND_LIBRARY_COUNT] = {
  "/mixkit-microphone-hit-2181.wav"
};

/* Card Slot Flashing Behaviour */
const int FLASH_LED_PINS[3] = {
  PERFEXMOD_LED_2_PIN,  // Red
  PERFEXMOD_LED_3_PIN,  // Yellow
  PERFEXMOD_LED_4_PIN   // Green
};

bool isCardSlotFlashLEDSequenceActive = false;
int cardSlotFlashLEDStepNumber = 0;
unsigned long cardSlotFlashLEDStepNumberStartMilliseconds = 0;
int currentCardSlotFlashLEDSequenceColorIndex = -1;

void startCardSlotFlashLEDSequence() {
  isCardSlotFlashLEDSequenceActive = true;
  cardSlotFlashLEDStepNumber = 0;
  cardSlotFlashLEDStepNumberStartMilliseconds = millis();
  currentCardSlotFlashLEDSequenceColorIndex = random(3);
  digitalWrite(FLASH_LED_PINS[currentCardSlotFlashLEDSequenceColorIndex], HIGH);
}

/* OSC Broadcasting */

void sendOSCMessage(const char* address, int value) {
  OSCMessage msg(address);
  msg.add(value);
  udp.beginPacket(OSC_BROADCAST_IP, OSC_PORT);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
}


/* SD Card and Audio */
void setupSDCardReader() {
  SPI.begin(PERFEXMOD_SD_CLK_PIN, PERFEXMOD_SD_MISO_PIN, PERFEXMOD_SD_MOSI_PIN, PERFEXMOD_SD_CS_PIN);
  if (!SD.begin(PERFEXMOD_SD_CS_PIN)) {
    Serial.println("SD card mount failed! :(");
    return;
  }
  Serial.println("SD card mounted successfully! :)");

}

void setupAudio() {
  // audio.setBufsize(4096, 4096); // Limit 4KB allocation towards the input buffer (reading from SD card) and output buffer (sending to MAX98357A).
  audio.setPinout(PERFEXMOD_SPEAKER_BCLK_PIN, PERFEXMOD_SPEAKER_LRC_PIN, PERFEXMOD_SPEAKER_DIN_PIN);
  audio.setVolume(15); // 0-21
}

void playAudioWAV(const char* filename) {
  audio.connecttoFS(SD, filename);
}

void serialPrintOSCData(const char* oscAddress, int value) {
  Serial.print(oscAddress);
  Serial.print(" ");
  Serial.println(value);
}


void setup() {
  Serial.begin(115200);

  onboardRGBLED.begin();
  onboardRGBLED.setBrightness(ONBOARD_RGB_LED_BRIGHTNESS);
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

  setupSDCardReader();
  setupAudio();

//   Serial.print("Free heap: ");
//   Serial.println(ESP.getFreeHeap());

  // Use INPUT_PULLUP — no need to use resistors!
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  pinMode(PERFEXMOD_LED_1_PIN, OUTPUT);
  pinMode(PERFEXMOD_LED_2_PIN, OUTPUT);
  pinMode(PERFEXMOD_LED_3_PIN, OUTPUT);
  pinMode(PERFEXMOD_LED_4_PIN, OUTPUT);

}

unsigned long lastKnobOSCSendMilliseconds = 0;
unsigned long lastMicOSCSendMilliseconds = 0;

void loop() {

  audio.loop();

  // BUTTONS
  for (int i = 0; i < BUTTON_COUNT; i++) {
    bool isButtonA = i == 0;
    bool isButtonB = i == 1;
    bool isButtonC = i == 2;
    bool isButtonD = i == 3;
    bool isButtonE = i == 4;
    bool isButton1 = i == 5;
    bool isButton2 = i == 6;
    bool isButton3 = i == 7;
    bool isButton4 = i == 8;
    bool isButton5 = i == 9;
    int buttonPin = BUTTON_PINS[i];
    bool isPressed = IS_BUTTON_NORMALLY_OPEN[i]
      ? isNormallyOpenButtonPressed(buttonPin)
      : isNormallyClosedButtonPressed(buttonPin);
    bool previousButtonState = previousButtonStates[i];
    if (isPressed != previousButtonState) {
      const char* buttonOSCAddress = BUTTON_OSC_ADDRESSES[i];
      serialPrintOSCData(buttonOSCAddress, isPressed);
      previousButtonStates[i] = isPressed;
      if (isPressed) {
        onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(255, 255, 255));
        sendOSCMessage(buttonOSCAddress, 1);
        if (isButtonA) {
          digitalWrite(PERFEXMOD_LED_1_PIN, HIGH);
          int randomIndexA = random(BUTTON_A_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_A_SOUND_LIBRARY[randomIndexA]);
        } else if (isButtonB) {
          int randomIndexB = random(BUTTON_B_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_B_SOUND_LIBRARY[randomIndexB]);
        } else if (isButtonC) {
          int randomIndexC = random(BUTTON_C_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_C_SOUND_LIBRARY[randomIndexC]);
        } else if (isButtonD) {
          int randomIndexD = random(BUTTON_D_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_D_SOUND_LIBRARY[randomIndexD]);
        } else if (isButtonE) {
          int randomIndexE = random(BUTTON_E_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_E_SOUND_LIBRARY[randomIndexE]);
          startCardSlotFlashLEDSequence();
        } else if (isButton5) {
          digitalWrite(PERFEXMOD_LED_1_PIN, HIGH);
          int randomIndexE = random(BUTTON_5_SOUND_LIBRARY_COUNT);
          audio.connecttoFS(SD, BUTTON_5_SOUND_LIBRARY[randomIndexE]);
        } else {
          
        }
      } else {
        onboardRGBLED.setPixelColor(0, onboardRGBLED.Color(0, 0, 0));
        sendOSCMessage(buttonOSCAddress, 0);
        if (isButtonA) {
          digitalWrite(PERFEXMOD_LED_1_PIN, LOW);
        } else if (isButtonE) {
          isCardSlotFlashLEDSequenceActive = false;
          currentCardSlotFlashLEDSequenceColorIndex = -1;
          digitalWrite(PERFEXMOD_LED_2_PIN, LOW);
          digitalWrite(PERFEXMOD_LED_3_PIN, LOW);
          digitalWrite(PERFEXMOD_LED_4_PIN, LOW);
          
        } else if (isButton5) {
          digitalWrite(PERFEXMOD_LED_1_PIN, LOW);
        }
      }
      onboardRGBLED.show();
    }
  }

  // FLASH SEQUENCE
  if (isCardSlotFlashLEDSequenceActive) {
    const int CARD_SLOT_FLASH_STEP_TOTAL_COUNT = 8;
    const unsigned long FLASH_QUICK_DURATION_MILLISECONDS = 180;
    bool isFinalCardSlotFlashLEDStep = cardSlotFlashLEDStepNumber == CARD_SLOT_FLASH_STEP_TOTAL_COUNT - 1;

    bool isCardSlotFlashLEDSequenceReadyForNextFlashStep = millis() - cardSlotFlashLEDStepNumberStartMilliseconds > FLASH_QUICK_DURATION_MILLISECONDS;
    if (!isFinalCardSlotFlashLEDStep && isCardSlotFlashLEDSequenceReadyForNextFlashStep) {
      if (currentCardSlotFlashLEDSequenceColorIndex >= 0) {
        digitalWrite(FLASH_LED_PINS[currentCardSlotFlashLEDSequenceColorIndex], LOW);
      }
      cardSlotFlashLEDStepNumber++;
      currentCardSlotFlashLEDSequenceColorIndex = random(3);
      digitalWrite(FLASH_LED_PINS[currentCardSlotFlashLEDSequenceColorIndex], HIGH);
      cardSlotFlashLEDStepNumberStartMilliseconds = millis();

      isFinalCardSlotFlashLEDStep = cardSlotFlashLEDStepNumber == CARD_SLOT_FLASH_STEP_TOTAL_COUNT - 1;
      if (isFinalCardSlotFlashLEDStep) {
      bool isFinalCardSlotFlashLEDStepColorRed = currentCardSlotFlashLEDSequenceColorIndex == 0;
      if (isFinalCardSlotFlashLEDStepColorRed) {
        audio.connecttoFS(SD, "/mixkit-system-beep-buzzer-fail-2964.wav");
      }
    }
    }
  }

  // KNOBS
  bool isKnobWaitElapsed = millis() - lastKnobOSCSendMilliseconds > 50;
  for (int i = 0; i < KNOB_COUNT; i++) {
    int rawKnobValue = analogRead(KNOB_PINS[i]);
    int knobValue = constrain(map(rawKnobValue, 0, 4095, 0, 127), 0, 127);
    bool isKnobValueDifferent = abs(knobValue - previousKnobValues[i]) > 4;
    if (isKnobValueDifferent && isKnobWaitElapsed) {
      previousKnobValues[i] = knobValue;
      const char* knobOSCAddress = KNOB_OSC_ADDRESSES[i];
      sendOSCMessage(knobOSCAddress, knobValue);
      serialPrintOSCData(knobOSCAddress, knobValue);
    }
  }

  // MICROPHONE (MAX9814)
  int rawMicValue = analogRead(PERFEXMOD_MICROPHONE_PIN);
  // Serial.print("rawMicValue: ");
  // Serial.println(rawMicValue);
  int micValue = constrain(map(rawMicValue, 1200, 3000, 0, 127), 0, 127);
  bool isMicValueDifferent = micValue != previousMicValue;
  bool isMicValueDifferentEnough = abs((micValue - previousMicValue)) > 50; // this is janky so come up with a better way to differentiate sounds from ambient
  bool isMicUpdateWaitElapsed = millis() - lastMicOSCSendMilliseconds > 50;
  if (isMicValueDifferent && isMicValueDifferentEnough && isMicUpdateWaitElapsed) {
    previousMicValue = micValue;
    lastMicOSCSendMilliseconds = millis();
    // sendOSCMessage("/perfexmod/mic", micValue);
  }

}