#define HWSERIAL Serial1

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(9600);
}
const unsigned long sleeptime = 2500;
unsigned long sleep = 0;
int aread = 0;
boolean bounce = 0;

// array to hold analog values for each column
int col[12];

// array to hold high and low values for each range
int range[16][2] = {
  {1009, 1024},
  {204, 215},
  {343, 352},
  {146, 152},
  {507, 518},
  {170, 176},
  {255, 260},
  {126, 134},
  {686, 701},
  {186, 192},
  {295, 302},
  {137, 142},
  {409, 416},
  {157, 162},
  {227, 238},
  {118, 125}
};

// convert to switch positions
boolean bin[16][4] = {
  {0, 0, 0, 0},
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {1, 1, 0, 0},
  {0, 0, 1, 0},
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {1, 1, 1, 0},
  {0, 0, 0, 1},
  {1, 0, 0, 1},
  {0, 1, 0, 1},
  {1, 1, 0, 1},
  {0, 0, 1, 1},
  {1, 0, 1, 1},
  {0, 1, 1, 1},
  {1, 1, 1, 1}
};

// array to hold key positions
boolean keya[12][4] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

boolean keyb[12][4] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

void loop()
{
  LEDsleep();

  readcolumns();
  assignkeysA();
  delay(5);
  readcolumns();
  assignkeysB();

  compkeys();

  if (bounce == 0)
  {
    LEDupdate();
    SERIALupdate();
  }
}

void readcolumns()
{
  byte pass = 0;
  int comp = 0;
  for (int x = 0; x < 12; x++)
  {
    aread = 0;;
    comp = 0;
    pass = 0;

    while (pass < 3)
    {
      aread = analogRead(x + 14);
      if (aread != comp)
      {
        pass = 0;
      }
      comp = aread;
      pass++;
    }
    col[x] = aread;
  }
}

void assignkeysA()
{
  for (int c = 0; c < 12; c++)
  {
    for (int v = 0; v < 16; v++)
    {
      if ((col[c] >= range[v][0]) && (col[c] <= range[v][1]))
      {
        for (byte x = 0; x < 4; x++)
        {
          keya[c][x] = bin[v][x];
        }
      }
    }
  }
}

void assignkeysB()
{
  for (int c = 0; c < 12; c++)
  {
    for (int v = 0; v < 16; v++)
    {
      if ((col[c] >= range[v][0]) && (col[c] <= range[v][1]))
      {
        for (byte x = 0; x < 4; x++)
        {
          keyb[c][x] = bin[v][x];
        }
      }
    }
  }
}

void compkeys()
{
  for (int c = 0; c < 12; c++)
  {
    for (int v = 0; v < 16; v++)
    {
      bounce = 0;
      for (byte x = 0; x < 4; x++)
      {
        if (keya[c][x] != keyb[c][x])
        {
          bounce = 1;
        }
      }
    }
  }
}

void LEDupdate()
{
  for (int c = 0; c < 12; c++)
  {
    for (byte r = 0; r < 4; r++)
    {
      if (keya[c][r] == 1)
      {
        sleep = 0;
        HWSERIAL.write((r * 16) + c);
        Serial.println((r * 16) + c);
      }
    }
  }
}

void SERIALupdate()
{
  for (int y = 0; y < 12; y++)
  {
    Serial.print(col[y]);
    Serial.print(", ");
  }
  Serial.println();
}

void LEDsleep()
{
  if (sleep < sleeptime)
  {
    sleep++;
  }

  if (sleep == sleeptime)
  {
    HWSERIAL.write(random(0, 63));
    delay(5);
  }
}

