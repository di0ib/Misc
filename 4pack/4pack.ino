#include <HID-Project.h>
#include <HID-Settings.h>
#include <TimerOne.h>

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

const KeyboardKeycode kcode[4] = { KEY_LEFT_ARROW,
                                   KEY_DOWN_ARROW,
                                   KEY_UP_ARROW,
                                   KEY_RIGHT_ARROW
                                 };
byte col = 0;
byte leds[2][2];
const int keyfade = 40;
const char curve[]  = {
  0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1
};
const int fadecount = 2;
const unsigned long breathcount = 5000;
int fadecounter = 0;
unsigned long breathcounter = 0;
byte pass = 1;

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[6] = {
  -1, 18, 19, 20, 21
};

// col[xx] of leds = pin yy on led matrix
int cols[2] = {
  pins[2], pins[1]
};

// row[xx] of leds = pin yy on led matrix
int rows[2] = {
  pins[4], pins[3]
};



void setup() {
  Keyboard.begin();
  Consumer.begin();
  setupLeds();
  onLeds();
  //setup inputs, turn on pullups
  for (int i = 0; i < 4; i++) {
    pinMode(k[i], INPUT);
    digitalWrite(k[i], 1);
  }
}

void loop() {
  CheckKeys();
  delay(debounce);

  fadecounter++;
  if (fadecounter > fadecount) {
    fadecounter = 0;
    fade();
  }
  breathcounter++;
  if (breathcounter > breathcount) {
    breathcounter = 0;
    fadecounter = 0;
    breath();
  }
}

void CheckKeys() {
  for (int i = 0; i < 4; i++) {
    if (codetype[i] == 'M') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Consumer.press((ccode[i]));
          s[i] = 1;
          byte row = i / 2;
          byte col = i % 2;
          leds[col][row] = 60;
          breathcounter = 0;
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          Consumer.release((ccode[i]));
          byte row = i / 2;
          byte col = i % 2;
          leds[col][row] = 60;
          breathcounter = 0;
        }
      }
    }
    if (codetype[i] == 'K') {
      if (digitalRead(k[i]) == 0) {
        if (s[i] == 0) {
          Keyboard.press((kcode[i]));
          s[i] = 1;
          byte row = i / 2;
          byte col = i % 2;
          leds[col][row] = 60;
          breathcounter = 0;
        }
      }
      else {
        if (s[i] == 1) {
          s[i] = 0;
          Keyboard.release((kcode[i]));
          byte row = i / 2;
          byte col = i % 2;
          leds[col][row] = 60;
          breathcounter = 0;
        }
      }
    }
  }
}

void setupLeds() {
  // sets the pins as output
  for (int i = 1; i <= 4; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // set up cols and rows
  for (int i = 1; i <= 2; i++) {
    digitalWrite(cols[i - 1], HIGH);
  }

  for (int i = 1; i <= 2; i++) {
    digitalWrite(rows[i - 1], LOW);
  }

  clearLeds();

  // Set refresh rate (interrupt timeout period)
  Timer1.initialize(500);
  // Set interrupt routine to be called
  Timer1.attachInterrupt(display);
}

void clearLeds() {
  // Clear display array
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      leds[i][j] = 0;
    }
  }
}

void onLeds() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      leds[i][j] = keyfade;
    }
  }
}

void fade() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      if (leds[i][j] > 0) {
        leds[i][j] = leds[i][j] - 1;
      }
    }
  }
}

void breath() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      leds[i][j] = 60;
    }
  }
}

// Interrupt routine
void display() {
  digitalWrite(cols[col], HIGH);  // Turn whole previous column off
  col++;
  if (col == 2) {
    col = 0;
    pass++;
    if (pass > 8) {
      pass = 1;
    }
  }
  for (int row = 0; row < 2; row++) {
    if (curve[leds[col][row]] > pass) {
      digitalWrite(rows[row], HIGH);  // Turn on this led
    }
    else {
      digitalWrite(rows[row], LOW); // Turn off this led
    }
  }
  digitalWrite(cols[col], LOW); // Turn whole column on at once (for equal lighting times)
}

