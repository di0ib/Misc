#include <HID-Project.h>
#include <HID-Settings.h>

//debounce milliseconds
const int debounce = 5;
//Switch Pins
const byte k[4] = { 4, 5, 6, 7 };
//Switch status
boolean s[4] = { 0, 0, 0, 0 };

//M for Media Key, K for Keyboard
const char codetype[4] = { 'K', 'K', 'K', 'K' };

//Keycodes
const ConsumerKeycode ccode[4] = { MEDIA_VOLUME_MUTE,
                                   MEDIA_VOLUME_MUTE,
                                   MEDIA_PLAY_PAUSE,
                                   MEDIA_PLAY_PAUSE
                                 };

const KeyboardKeycode kcode[4] = { KEY_DOWN_ARROW,
                                   KEY_LEFT_ARROW,
                                   KEY_UP_ARROW,
                                   KEY_RIGHT_ARROW
                                 };

void setup() {
  Keyboard.begin();
  Consumer.begin();

  //setup inputs, turn on pullups
  for (int i = 0; i < 4; i++) {
    pinMode(k[i], INPUT);
    digitalWrite(k[i], 1);
  }
}

void loop() {
  CheckKeys();
  delay(debounce);
}

void CheckKeys() {
  for (int i = 0; i < 4; i++) {
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



