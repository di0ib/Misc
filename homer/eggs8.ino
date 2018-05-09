#include <HID-Project.h>
#include <HID-Settings.h>

//debounce milliseconds
const int debounce = 5;
//Switch Pins
const byte k[8] = { 6, 7, 8, 9, 10, 16, 14, 15 };
//Switch status
boolean s[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//M for Media Key, K for Keyboard
const char codetype[8] = { 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K' };

//Keycodes
const ConsumerKeycode ccode[8] = { MEDIA_VOLUME_MUTE,
                                   MEDIA_VOLUME_MUTE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE
                                 };

const KeyboardKeycode kcode[8] = { KEY_1,
                                   KEY_2,
                                   KEY_3,
                                   KEY_4,
                                   KEY_5,
                                   KEY_6,
                                   KEY_7,
                                   KEY_8
                                 };

void setup() {
  Keyboard.begin();
  Consumer.begin();
  //setup inputs, turn on pullups
  for (int i = 0; i <= 7; i++) {
    pinMode(k[i], INPUT);
    digitalWrite(k[i], 1);
  }
}

void loop() {
  CheckKeys();
  delay(debounce);
}

void CheckKeys() {
  for (int i = 0; i <= 7; i++) {
    if (codetype[i] == 'M') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Consumer.press((ccode[i]));
          s[i] = 1;
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          Consumer.release((ccode[i]));
        }
      }
    }
    if (codetype[i] == 'K') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Keyboard.press((kcode[i]));
          s[i] = 1;
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          Keyboard.release((kcode[i]));
        }
      }
    }
  }
}
