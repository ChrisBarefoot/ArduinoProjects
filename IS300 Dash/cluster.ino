
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Adafruit_NeoPixel.h>
#include <OBD.h>

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16}; // the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];


Adafruit_AlphaNum4 screen1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 screen2 = Adafruit_AlphaNum4();

#define PIN 6
Adafruit_NeoPixel ns_stick = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

int START_RPM = 5000; 
int SHIFT_RPM = 6100; 
int RPM_SEGMENT = (SHIFT_RPM - START_RPM) / 8;

bool firstTimeRun = true;

COBD obd; /* for Model A (UART version) */

char intro[] = {' ' , ' ', ' ', 'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', 'T', 'O', ' ', 'C', 'H', 'R', 'I', 'S', '\'', 'S', ' ', 'I', 'S', '3', '0', '0', ' ', ' ', ' ', ' '};

uint8_t brightnessArray[] = {255, 175, 125, 50};
uint8_t brightnessIndex = 0;
bool changeBrightness = false;

byte pidArray[] = {PID_RPM, PID_COOLANT_TEMP, PID_INTAKE_TEMP, PID_ENGINE_LOAD, PID_THROTTLE};
#define NUMPIDS sizeof(pidArray)
uint8_t screen1Index = 0;
uint8_t screen2Index = 1;

void writeToScreen(int screen, char one, char two, char three, char four) {
    if(screen == 0) {
      screen1.writeDigitAscii(0, one);
      screen1.writeDigitAscii(1, two);
      screen1.writeDigitAscii(2, three);
      screen1.writeDigitAscii(3, four);
      screen1.writeDisplay();
    } else if(screen == 1) {
      screen2.writeDigitAsciiFlipped(3, one);
      screen2.writeDigitAsciiFlipped(2, two);
      screen2.writeDigitAsciiFlipped(1, three);
      screen2.writeDigitAsciiFlipped(0, four);
      screen2.writeDisplay();
    }
}

void writeMessage(char *message) {

  while(*(message+4)!='\0') {
    writeToScreen(1, *(message), *(message+1), *(message+2), *(message+3));
    delay(140);
    message++;
  }

}

void stickColor(int pixel, int r, int g, int b) {
  ns_stick.setPixelColor(pixel, ns_stick.Color(r, g, b));
}

void clearStick() {
  for(int i = 0; i < ns_stick.numPixels(); i++) {
    stickColor(i, 0, 0, 0);
  }
  ns_stick.show();
}

void warn(int colorr, int colorg, int colorb) {
    for(int i = 0; i < 5; i++) {
      clearStick();
      delay(250);
      for(int j = 0; j < ns_stick.numPixels(); j++)
      {
        stickColor(j, colorr, colorg, colorb);
      }
      ns_stick.show(); 
      delay(250);
    }
    clearStick();
}

char printc(int num)
{
  if(num == 0)
    return '0';
  else if(num == 1)
    return '1';
  else if(num == 2)
    return '2';
  else if(num == 3)
    return '3';
  else if(num == 4)
    return '4';
  else if(num == 5)
    return '5';
  else if(num == 6)
    return '6';
  else if(num == 7)
    return '7';
  else if(num == 8)
    return '8';
  else if(num == 9)
    return '9';
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis()) { // we wrapped around, lets just try again
     lasttime = millis();
  }
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  for (index = 0; index < NUMBUTTONS; index++) { // when we start, we clear out the "just" indicators
    justreleased[index] = 0;
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


// the setup routine runs once when you press reset:
void setup() 
{
  //Serial.begin(9600);

  // Make input & enable pull-up resistors on switch pins
  for (int i=0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }

  ns_stick.begin();
  clearStick();
  
  //init screens
  screen2.begin(0x70);
  screen1.begin(0x71);

  //clear screen
  writeToScreen(0, ' ', ' ', ' ', ' ');

  //write intro message to screen
  writeMessage(intro);

  //start up shift light and blink whtie
  warn(255, 255, 255);
  
  // start communication with OBD-II adapter
  writeToScreen(0, 'I', 'N', 'I', 'T');
  obd.begin();

  int numAttempts = 0;
  int screenCount = 0;
  int retCode = obd.initWithReturn();
  while (retCode != 3) {
    if(retCode == 1) {
      writeToScreen(1, '1', ' ', ' ', ' ');
      delay(300);
    } else if(retCode == 2) {
      writeToScreen(1, '2', ' ', ' ', ' ');
      delay(300);
    }
    writeToScreen(1, 'R', 'E', 'T', 'R');
    numAttempts++;
    if(numAttempts > 10) {
      writeToScreen(1, 'R', 'E', 'D', 'O');
      numAttempts = 0;
      obd.begin();
    }
    retCode = obd.initWithReturn();
  }
  writeToScreen(0, 'S', 'U', 'C', 'C');
}

int dtcCheckInterval = 0;
int coolantCheckInterval = 0;

void loop() {

  //Handle Button Press Code First
  check_switches();      // when we check the switches we'll get the current state

  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (justreleased[i]) {
      if(i == 0) {
        changeBrightness = true;
      } else if(i == 1) {
        screen1Index++;
        if(screen1Index >= NUMPIDS) {
          screen1Index = 0;
        }
      } else if(i == 2) {
        screen2Index++;
        if(screen2Index >= NUMPIDS) {
          screen2Index = 0;
        }
      }
    }
    if (pressed[i]) {
      // is the button pressed down at this moment
    }
  }

  if(changeBrightness) {
    brightnessIndex++;
    if(brightnessIndex > 3) {
      brightnessIndex = 0;
    }
    
    ns_stick.setBrightness(brightnessArray[brightnessIndex]);
    screen1.setBrightness(brightnessArray[brightnessIndex]);
    screen2.setBrightness(brightnessArray[brightnessIndex]);
  }
  


  //DTC CODES
  if(dtcCheckInterval == 0) {
    unsigned int codes[6];
    byte dtcCount = obd.readDTC(codes, 6);
    if (dtcCount == 0) {
      if(firstTimeRun) {
        writeToScreen(0, 'N', 'O', ' ', ' ');
        writeToScreen(1, 'D', 'T', 'C', ' ');
      }
    } else {
      warn(255, 255, 255);
      for (byte n = 0; n < dtcCount; n++) {
        writeToScreen(0, 'D', 'T', 'C', ' ');
        
        int thousands = codes[n] / 1000;
        int hundreds = (codes[n] % 1000) / 100;
        int tens = (codes[n] % 100) / 10;
        int ones = (codes[n] % 10);
    
        writeToScreen(1, printc(thousands), printc(hundreds), printc(tens), printc(ones));
        delay(1000);
      }
    } 
  }
  dtcCheckInterval++;
  if(dtcCheckInterval > 10000) {
    dtcCheckInterval = 0;
  }
  
  //RPM's
  int rpmValue = 1234;
  if (obd.readPID(PID_RPM, rpmValue)) {
    int thousands = rpmValue / 1000;
    int hundreds = (rpmValue % 1000) / 100;
    int tens = (rpmValue % 100) / 10;
    int ones = (rpmValue % 10);

    if(screen1Index == 0) {
      writeToScreen(0, printc(thousands), printc(hundreds), printc(tens), printc(ones));      
    }
    if(screen2Index == 0) {
      writeToScreen(1, printc(thousands), printc(hundreds), printc(tens), printc(ones));      
    }

  }

  //Coolant
  if(coolantCheckInterval == 0) {
    int coolantValue = 324;
    if (screen1Index != 0 && obd.readPID(pidArray[screen1Index], coolantValue)) {

      if(screen1Index == 1 || screen1Index == 2) {
        coolantValue = (coolantValue * 9 / 5) + 32;
        int hundreds = coolantValue / 100;
        int tens = (coolantValue % 100) / 10;
        int ones = (coolantValue % 10);
    
        writeToScreen(0, printc(hundreds), printc(tens), printc(ones), 'F'); 
        if(screen2Index == screen1Index) {
          writeToScreen(1, printc(hundreds), printc(tens), printc(ones), 'F'); 
        }
      } else if(screen1Index == 3 || screen1Index == 4) {
        int hundreds = coolantValue / 100;
        int tens = (coolantValue % 100) / 10;
        int ones = (coolantValue % 10);
    
        writeToScreen(0, ' ', printc(hundreds), printc(tens), printc(ones)); 
        if(screen2Index == screen1Index) {
          writeToScreen(1, ' ', printc(hundreds), printc(tens), printc(ones)); 
        }
      }

      if(screen1Index == 1) {
        if(coolantValue > 210) {
          warn(255, 0, 0);
          delay(500);
          warn(255, 0, 0);
        }
      }
    }

    if(screen2Index != 0 && screen1Index != screen2Index) {
      int secondValue = 324;
      if (obd.readPID(pidArray[screen2Index], secondValue)) {
  
        if(screen2Index == 1 || screen2Index == 2) {
          secondValue = (secondValue * 9 / 5) + 32;
          int hundreds = secondValue / 100;
          int tens = (secondValue % 100) / 10;
          int ones = (secondValue % 10);
      
          writeToScreen(1, printc(hundreds), printc(tens), printc(ones), 'F'); 
        } else if(screen2Index == 3 || screen2Index == 4) {
          int hundreds = secondValue / 100;
          int tens = (secondValue % 100) / 10;
          int ones = (secondValue % 10);
      
          writeToScreen(1, ' ', printc(hundreds), printc(tens), printc(ones)); 
        }

        if(screen2Index == 1) {
          if(secondValue > 210) {
            warn(255, 0, 0);
            delay(500);
            warn(255, 0, 0);
          }
        }
      }
    }
  }
  coolantCheckInterval++;
  if(coolantCheckInterval > 1000) {
    coolantCheckInterval = 0;
  }
  
  //shift light code
  if(rpmValue < START_RPM)
  {
    clearStick();
  }
  else if(rpmValue >= START_RPM && rpmValue < SHIFT_RPM) {
    clearStick();
    if(rpmValue >= (START_RPM + (RPM_SEGMENT*0))) {
      stickColor(0, 0, 255, 0);
      stickColor(15, 0, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*1))) {
      stickColor(1, 0, 255, 0);
      stickColor(14, 0, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*2))) {
      stickColor(2, 0, 255, 0);
      stickColor(13, 0, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*3))) {
      stickColor(3, 0, 255, 0);
      stickColor(12, 0, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*4))) {
      stickColor(4, 255, 255, 0);
      stickColor(11, 255, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*5))) {
      stickColor(5, 255, 255, 0);
      stickColor(10, 255, 255, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*6))) {
      stickColor(6, 255, 0, 0);
      stickColor(9, 255, 0, 0);
    }
    if (rpmValue >= (START_RPM + (RPM_SEGMENT*7))) {
      stickColor(7, 255, 0, 0);
      stickColor(8, 255, 0, 0);
    }
    ns_stick.show();
  }
  else if(rpmValue >= SHIFT_RPM)
  {
    clearStick();
    delay(60);
    for(int j = 0; j < ns_stick.numPixels(); j++)
    {
      stickColor(j, 255, 0, 0);
    }
    ns_stick.show();  
    delay(60);
  }
  
  firstTimeRun = false;
}
