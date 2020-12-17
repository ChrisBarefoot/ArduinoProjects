/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */
#include <Wire.h>
#include <OBD.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha5 = Adafruit_AlphaNum4();
//#define PIN 9
//#define N_CELL 8

//NS_Rainbow ns_stick = NS_Rainbow(N_CELL, PIN); 

#define PIN            6
#define NUMPIXELS      16
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int rpm; 
int rpmlast = 700; 
int rpm_interval = 3000; 

int rpmled; 
int START_RPM = 2000; 
int SHIFT_RPM = 3000; 
int RPM_SEGMENT = (SHIFT_RPM - START_RPM) / 8;

int MAX_RPM = 7000;
int RPM_BUFFER = 1000;

COBD obd; /* for Model A (UART version) */

const int lButton = 2;
//const int rButton = 3;

int rButtonState = 0;
int lButtonState = 0;


char wmess[12] = "HONDA S2000";
void WelcomeMessage()
{
  int pos = 8;
  for(int i = 0; i < 21; i++)
  {
    for(int letter = 0; letter < 11; letter++)
    {
      if(letter < 4)
      {
        if(letter - pos < 11 && letter - pos >= 0)
          alpha5.writeDigitAscii(letter, wmess[letter-pos]);
        else
          alpha5.writeDigitAscii(letter, ' ');
      }
      else
      {
        if(letter - pos < 11 && letter - pos >= 0)
          alpha4.writeDigitAscii(letter-4, wmess[letter-pos]);
        else
          alpha4.writeDigitAscii(letter-4, ' ');
      }
    }
    alpha5.writeDisplay();
    alpha4.writeDisplay();
    delay(100);
    pos--;   
  }
}

// the setup routine runs once when you press reset:
void setup() 
{
  Serial.begin(38400);

  alpha4.begin(0x77);
  alpha5.begin(0x70);

  pixels.begin(); // This initializes the NeoPixel library.
  
  pinMode(lButton, INPUT); 
  //pinMode(rButton, INPUT); 
  
  // start communication with OBD-II adapter
  obd.begin();
  // initiate OBD-II connection until success
  
  while (!obd.init());
  
  WelcomeMessage();

}

void showLights(int rpm)
{
  if(rpm < 7000)
  {
    for(int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();  
    }
    return;
  }
  else if(rpm > 7000 && rpm < 7100)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(0,0,0));
    pixels.setPixelColor(12, pixels.Color(0,0,0));
    pixels.setPixelColor(13, pixels.Color(0,0,0));
    pixels.setPixelColor(14, pixels.Color(0,0,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7100 && rpm < 7200)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(0,0,0));
    pixels.setPixelColor(12, pixels.Color(0,0,0));
    pixels.setPixelColor(13, pixels.Color(0,0,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7200 && rpm < 7300)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(0,0,0));
    pixels.setPixelColor(12, pixels.Color(0,0,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7300 && rpm < 7400)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,255,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(0,0,0));
    pixels.setPixelColor(12, pixels.Color(0,255,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7400 && rpm < 7500)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,255,0));
    pixels.setPixelColor(4, pixels.Color(255,255,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(255,255,0));
    pixels.setPixelColor(12, pixels.Color(0,255,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7500 && rpm < 7600)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,255,0));
    pixels.setPixelColor(4, pixels.Color(255,255,0));
    pixels.setPixelColor(5, pixels.Color(255,255,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(255,255,0));
    pixels.setPixelColor(11, pixels.Color(255,255,0));
    pixels.setPixelColor(12, pixels.Color(0,255,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7600 && rpm < 7700)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,255,0));
    pixels.setPixelColor(4, pixels.Color(255,255,0));
    pixels.setPixelColor(5, pixels.Color(255,255,0));
    pixels.setPixelColor(6, pixels.Color(255,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(255,0,0));
    pixels.setPixelColor(10, pixels.Color(255,255,0));
    pixels.setPixelColor(11, pixels.Color(255,255,0));
    pixels.setPixelColor(12, pixels.Color(0,255,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm > 7700 && rpm < 7800)
  {
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(0,255,0));
    pixels.setPixelColor(4, pixels.Color(255,255,0));
    pixels.setPixelColor(5, pixels.Color(255,255,0));
    pixels.setPixelColor(6, pixels.Color(255,0,0));
    pixels.setPixelColor(7, pixels.Color(255,0,0));
    pixels.setPixelColor(8, pixels.Color(255,0,0));
    pixels.setPixelColor(9, pixels.Color(255,0,0));
    pixels.setPixelColor(10, pixels.Color(255,255,0));
    pixels.setPixelColor(11, pixels.Color(255,255,0));
    pixels.setPixelColor(12, pixels.Color(0,255,0));
    pixels.setPixelColor(13, pixels.Color(0,255,0));
    pixels.setPixelColor(14, pixels.Color(0,255,0));
    pixels.setPixelColor(15, pixels.Color(0,255,0));
    pixels.show();
  }
  else if(rpm >= 7800)
  {
    for(int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();  
    }
    delay(20);
    for(int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255,0,0));
      pixels.show();  
    } 
  }  
}

void writeValue(int val)
{
  if(val < 0)
  {
    alpha4.writeDigitAscii(0, 'L');
    alpha4.writeDigitAscii(1, 'T');
    alpha4.writeDigitAscii(2, 'Z');
    alpha4.writeDigitAscii(3, 'O' );
    alpha4.writeDisplay();
  }
  else if(val < 1)
  {
    alpha4.writeDigitAscii(0, 'F');
    alpha4.writeDigitAscii(1, 'R');
    alpha4.writeDigitAscii(2, 'A');
    alpha4.writeDigitAscii(3, 'C' );
    alpha4.writeDisplay();
  }
  else if(val < 10)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, ' ');
    alpha4.writeDigitAscii(2, ' ');
    alpha4.writeDigitAscii(3, printc(val));
    alpha4.writeDisplay();
  }
  else if(val < 100)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, ' ');
    alpha4.writeDigitAscii(2, printc(val/10));
    alpha4.writeDigitAscii(3, printc(val%10));
    alpha4.writeDisplay();
  }
  else if(val < 1000)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, printc(val/100));
    alpha4.writeDigitAscii(2, printc((val%100)/10));
    alpha4.writeDigitAscii(3, printc(val%10));
    alpha4.writeDisplay();
  }
  else if(val > 9999)
  {
    alpha4.writeDigitAscii(0, 'O');
    alpha4.writeDigitAscii(1, 'V');
    alpha4.writeDigitAscii(2, 'R');
    alpha4.writeDigitAscii(3, 'M');
    alpha4.writeDisplay();

  }
  else
  {
    alpha4.writeDigitAscii(0, printc(val/1000));
    alpha4.writeDigitAscii(1, printc((val%1000)/100));
    alpha4.writeDigitAscii(2, printc((val%100)/10));
    alpha4.writeDigitAscii(3, printc(val%10));
    alpha4.writeDisplay();
  }
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

int mode = 0;
int max_mode = 4;

void loop() {
  int rpm = 0;
  if (!obd.read(PID_RPM, rpm)) {
    return;
  }
  /*lButtonState = digitalRead(lButton);
  //rButtonState = digitalRead(rButton);
  
  Serial.println(lButtonState == HIGH);
  if(lButtonState == HIGH)
  {
    mode--;
    if(mode < 0)
      mode = max_mode;
  }
  /*
//  if(rButtonState == HIGH)
//  {
//    mode++;
//    if(mode > max_mode)
//      mode = 0;
//  }
  
  int val = 0;
  int rpm = 0;
  if (!obd.read(PID_RPM, rpm)) {
    return;
  }
  
  if(mode == 0)
  {
    alpha5.writeDigitAscii(0, 'R');
    alpha5.writeDigitAscii(1, 'P');
    alpha5.writeDigitAscii(2, 'M');
    alpha5.writeDigitAscii(3, ' ');
    alpha5.writeDisplay();
    val = rpm;
  }
  else if(mode == 1)
  {   
    alpha5.writeDigitAscii(0, 'C');
    alpha5.writeDigitAscii(1, 'O');
    alpha5.writeDigitAscii(2, 'O');
    alpha5.writeDigitAscii(3, 'L');
    alpha5.writeDisplay();
    if (!obd.read(PID_COOLANT_TEMP, val)) {
      return;
    }
  }
  else if(mode == 2)
  {   
    alpha5.writeDigitAscii(0, 'S');
    alpha5.writeDigitAscii(1, 'P');
    alpha5.writeDigitAscii(2, 'E');
    alpha5.writeDigitAscii(3, 'D');
    alpha5.writeDisplay();
    if (!obd.read(PID_SPEED, val)) {
      return;
    }
    val = val * 5 / 8;
  }
  else if(mode == 3)
  {   
    alpha5.writeDigitAscii(0, 'I');
    alpha5.writeDigitAscii(1, 'N');
    alpha5.writeDigitAscii(2, 'T');
    alpha5.writeDigitAscii(3, 'K');
    alpha5.writeDisplay();
    if (!obd.read(PID_INTAKE_TEMP, val)) {
      return;
    }
  }
  else if(mode == 4)
  {   
    alpha5.writeDigitAscii(0, 'T');
    alpha5.writeDigitAscii(1, 'H');
    alpha5.writeDigitAscii(2, 'R');
    alpha5.writeDigitAscii(3, 'T');
    alpha5.writeDisplay();
    if (!obd.read(PID_THROTTLE, val)) {
      return;
    }
  }*/
  
  writeValue(rpm);
  showLights(rpm);
  //delay(100);
}


