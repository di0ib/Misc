#include <HID-Project.h>
#include <HID-Settings.h>

//number of keys
const int keys = 12;
//debounce milliseconds
const int debounce = 5;
//Switch Pins
const byte k[12] = { 5, 6, 7, 8, 21, 20, 19, 18, 15, 14, 16, 10 };
//Switch status
boolean s[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//M for Media Key, K for Keyboard
const char codetype[12] = { 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K', 'K' };

//Keycodes
const ConsumerKeycode ccode[keys] = { MEDIA_VOLUME_MUTE,
                                      MEDIA_VOLUME_MUTE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE,
                                      MEDIA_PLAY_PAUSE
                                    };

const KeyboardKeycode kcode[keys] = { KEY_F1,
                                      KEY_F2,
                                      KEY_F3,
                                      KEY_F4,
                                      KEY_F5,
                                      KEY_F6,
                                      KEY_F7,
                                      KEY_F8,
                                      KEY_F9,
                                      KEY_F10,
                                      KEY_F11,
                                      KEY_F12
                                    };

void setup() {
  Keyboard.begin();
  Consumer.begin();
  //setup inputs, turn on pullups
  for (int i = 0; i < keys; i++) {
    pinMode(k[i], INPUT);
    digitalWrite(k[i], 1);
  }
}

void loop() {
  CheckKeys();
  delay(debounce);
}

void CheckKeys() {
  for (int i = 0; i < keys; i++) {
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


