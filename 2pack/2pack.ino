#include "HID-Project.h"

//debounce milliseconds
const int debounce = 10;
//Switch Pins
const byte k[2] = { 2, 3 };
//Switch status
boolean s[2] = { 0, 0 };
//LED Pins
const byte l[2] = { 10, 9 };

//M for Media Key, K for Keyboard
const char codetype[6] = { 'K', 'K' };

//Keycodes
const ConsumerKeycode ccode[2] = { MEDIA_VOLUME_MUTE,
                                   MEDIA_PLAY_PAUSE
                                 };

const KeyboardKeycode kcode[2] = { KEY_UP_ARROW,
                                   KEY_DOWN_ARROW
                                 };

void setup() {
  Keyboard.begin();
  Consumer.begin();

  //setup inputs, turn on pullups
  for (int i = 0; i <= 1; i++) {
    pinMode(k[i], INPUT);
    digitalWrite(k[i], 1);
    pinMode(l[i], OUTPUT);
  }
}

void loop() {
  CheckKeys();
  delay(debounce);
}

void CheckKeys() {
  for (int i = 0; i <= 1; i++) {
    if (codetype[i] == 'M') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Consumer.press((ccode[i]));
          s[i] = 1;
          digitalWrite(l[i], 1);
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          digitalWrite(l[i], 0);
          Consumer.release((ccode[i]));
        }
      }
    }
    if (codetype[i] == 'K') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Keyboard.press((kcode[i]));
          s[i] = 1;
          digitalWrite(l[i], 1);
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          digitalWrite(l[i], 0);
          Keyboard.release((kcode[i]));
        }
      }
    }
  }
}

