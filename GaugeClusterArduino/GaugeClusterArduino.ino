/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <NS_Rainbow.h>

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha5 = Adafruit_AlphaNum4();
#define PIN 9
#define N_CELL 8

NS_Rainbow ns_stick = NS_Rainbow(N_CELL, PIN);

const int sensorPin = 2; 
const int sensorInterrupt = 0;
const int timeoutValue = 5; 
volatile unsigned long lastPulseTime; 
volatile unsigned long interval = 0; 
volatile int timeoutCounter; 

unsigned long rpm; 
int rpmlast = 700; 
int rpm_interval = 3000; 

int rpmled; 
int START_RPM = 2000; 
int SHIFT_RPM = 3000; 
int RPM_SEGMENT = (SHIFT_RPM - START_RPM) / 8;

int MAX_RPM = 7000;
int RPM_BUFFER = 1000;

char wmess[9] = "WELCOME ";
void welcomeMessage()
{
  int pos = 8;
  for(int i = 0; i < 17; i++)
  {
    for(int letter = 0; letter < 8; letter++)
    {
      if(letter < 4)
      {
        if(letter - pos < 8 && letter - pos >= 0)
          alpha5.writeDigitAscii(letter, wmess[letter-pos]);
        else
          alpha5.writeDigitAscii(letter, ' ');
      }
      else
      {
        if(letter - pos < 8 && letter - pos >= 0)
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
  Serial.println("End of Welcome");
}

void printRPM(long rpm)
{
  if(rpm < 0)
  {
    alpha4.writeDigitAscii(0, 'L');
    alpha4.writeDigitAscii(1, 'T');
    alpha4.writeDigitAscii(2, 'Z');
    alpha4.writeDigitAscii(3, 'O' );
    alpha4.writeDisplay();
  }
  else if(rpm < 1)
  {
    alpha4.writeDigitAscii(0, 'F');
    alpha4.writeDigitAscii(1, 'R');
    alpha4.writeDigitAscii(2, 'A');
    alpha4.writeDigitAscii(3, 'C' );
    alpha4.writeDisplay();
  }
  else if(rpm < 10)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, ' ');
    alpha4.writeDigitAscii(2, ' ');
    alpha4.writeDigitAscii(3, printc(rpm));
    alpha4.writeDisplay();
  }
  else if(rpm < 100)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, ' ');
    alpha4.writeDigitAscii(2, printc(rpm/10));
    alpha4.writeDigitAscii(3, printc(rpm%10));
    alpha4.writeDisplay();
  }
  else if(rpm < 1000)
  {
    alpha4.writeDigitAscii(0, ' ');
    alpha4.writeDigitAscii(1, printc(rpm/100));
    alpha4.writeDigitAscii(2, printc((rpm%100)/10));
    alpha4.writeDigitAscii(3, printc(rpm%10));
    alpha4.writeDisplay();
  }
  else if(rpm > 9999)
  {
    alpha4.writeDigitAscii(0, 'O');
    alpha4.writeDigitAscii(1, 'V');
    alpha4.writeDigitAscii(2, 'R');
    alpha4.writeDigitAscii(3, 'M');
    alpha4.writeDisplay();
    
  }
  else
  {
    alpha4.writeDigitAscii(0, printc(rpm/1000));
    alpha4.writeDigitAscii(1, printc((rpm%1000)/100));
    alpha4.writeDigitAscii(2, printc((rpm%100)/10));
    alpha4.writeDigitAscii(3, printc(rpm%10));
    alpha4.writeDisplay();
  }
}

void showShiftLights(int rpm)
{
 if(rpm < 5000)
  {
    ns_stick.clear();
    ns_stick.show();
  }
  else if(rpm > 5500 && rpm < 5600)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 0, 0);
    ns_stick.setColor(2, 0, 0, 0); 
    ns_stick.setColor(3, 0, 0, 0);
    ns_stick.setColor(4, 0, 0, 0);
    ns_stick.setColor(5, 0, 0, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 5600 && rpm < 5700)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 0, 0); 
    ns_stick.setColor(3, 0, 0, 0);
    ns_stick.setColor(4, 0, 0, 0);
    ns_stick.setColor(5, 0, 0, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 5700 && rpm < 5800)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 0, 0);
    ns_stick.setColor(4, 0, 0, 0);
    ns_stick.setColor(5, 0, 0, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 5800 && rpm < 5900)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 255, 0);
    ns_stick.setColor(4, 0, 0, 0);
    ns_stick.setColor(5, 0, 0, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 5900 && rpm < 6000)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 255, 0);
    ns_stick.setColor(4, 255, 255, 0);
    ns_stick.setColor(5, 0, 0, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 6000 && rpm < 6100)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 255, 0);
    ns_stick.setColor(4, 255, 255, 0);
    ns_stick.setColor(5, 255, 255, 0);
    ns_stick.setColor(6, 0, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 6100 && rpm < 6200)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 255, 0);
    ns_stick.setColor(4, 255, 255, 0);
    ns_stick.setColor(5, 255, 255, 0);
    ns_stick.setColor(6, 255, 0, 0);
    ns_stick.setColor(7, 0, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm > 6200 && rpm < 6300)
  {
    ns_stick.clear();
    ns_stick.setColor(0, 0, 255, 0);
    ns_stick.setColor(1, 0, 255, 0);
    ns_stick.setColor(2, 0, 255, 0); 
    ns_stick.setColor(3, 0, 255, 0);
    ns_stick.setColor(4, 255, 255, 0);
    ns_stick.setColor(5, 255, 255, 0);
    ns_stick.setColor(6, 255, 0, 0);
    ns_stick.setColor(7, 255, 0, 0);
    ns_stick.show(); 
  }
  else if(rpm >= 6300)
  {
    ns_stick.clear();
    for(int j = 0; j < 8; j++)
    {
          ns_stick.setColor(j, 0, 0, 0);      // Black (clear) 
    }
    ns_stick.show();
    delay(20);
    ns_stick.clear();
    for(int j = 0; j < 8; j++)
    {
          ns_stick.setColor(j, 255, 0, 0);      // Black (clear) 
    }
    ns_stick.show();  
  } 
}

int cnt = 0;
void sensorIsr() 
{ 
  //unsigned long now = micros(); 
  //interval = now - lastPulseTime; 
  //lastPulseTime = now;
  cnt++;
} 

// the setup routine runs once when you press reset:
void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup Begin");
  attachInterrupt(sensorInterrupt, sensorIsr, FALLING); 

 
  alpha4.begin(0x70);
  alpha5.begin(0x71);
  
  ns_stick.begin();
  
  welcomeMessage();

  lastPulseTime = micros(); 
  Serial.println("Setup End");
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

int avgcnt = 0;
unsigned long avgs[10];
int badrpmcounter = 0;

// the loop routine runs over and over again forever:
void loop() 
{
  alpha5.writeDigitAscii(0, 'R');
  alpha5.writeDigitAscii(1, 'P');
  alpha5.writeDigitAscii(2, 'M');
  alpha5.writeDigitAscii(3, ' ');
  alpha5.writeDisplay();
  
  unsigned long now = micros();                    //current time
  double millisecs = (now - lastPulseTime) / 1000; //milliseconds since last loop
  unsigned long rate = cnt / (millisecs/1000);     //given the count calculate the rate
  //reset vars
  Serial.println(rate);
  lastPulseTime = now;
  cnt = 0;
  
  //rpm = 21e6/rate;
  unsigned long rawrpm = rate;// * 2.6;
  delay(30);
  
  if(avgcnt >= 10)
  {
    for(int i = 0; i < 9; i++)
    {
      avgs[i] = avgs[i + 1];
    }
    avgs[9] = rate;
  }
  else
  {
    avgs[avgcnt] = rate;
    avgcnt++;
    return;  
  }

  double avgrate = 0;
  for(int i = 0; i < 10; i++)
  {
    avgrate = avgrate + avgs[i];
  }
  rpm = (avgrate / 10) * 2.6;

  printRPM(rpm);
  showShiftLights(rpm);
}

//200 hz at idle
//430 hz at 2100 rpm
