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

int rpm; 
int rpmlast = 700; 
int rpm_interval = 3000; 

int rpmled; 
int START_RPM = 2000; 
int SHIFT_RPM = 3000; 
int RPM_SEGMENT = (SHIFT_RPM - START_RPM) / 8;

int MAX_RPM = 7000;
int RPM_BUFFER = 1000;

// the setup routine runs once when you press reset:
void setup() 
{
  Serial.begin(9600);
  
  alpha4.begin(0x70);
  alpha5.begin(0x71);
  
  ns_stick.begin();
  
  // config for the Tach 
  pinMode(sensorPin, INPUT); 
  attachInterrupt(sensorInterrupt, sensorIsr, FALLING); 
  
  lastPulseTime = micros(); 

}

//each time the interrupt receives a rising tach signal, it'll run this subroutine 
void sensorIsr() 
{ 
    unsigned long now = micros(); 
    interval = now - lastPulseTime; 
    lastPulseTime = now; 
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
int avgs[20];
int badrpmcounter = 0;

// the loop routine runs over and over again forever:
void loop() {
  alpha5.writeDigitAscii(0, 'R');
  alpha5.writeDigitAscii(1, 'P');
  alpha5.writeDigitAscii(2, 'M');
  alpha5.writeDigitAscii(3, ' ');
  alpha5.writeDisplay();


  
  int rawrpm = 0;
  rawrpm = 21e6/interval;
  
  rawrpm = constrain (rawrpm, 0, MAX_RPM); 
  if(rawrpm == MAX_RPM)
    return;
    
  if((rawrpm > (rpmlast + RPM_BUFFER)) || (rawrpm < (rpmlast - RPM_BUFFER)))
  {
    //discard value
    badrpmcounter++;
    if(badrpmcounter > 15)
    {
      rpmlast = rawrpm;
      badrpmcounter = 0;
    }
  }
  else
  {
    avgs[avgcnt] = rawrpm;
    avgcnt++;
    rpmlast = rawrpm;
    badrpmcounter = 0;
  }
  
//  

  if(avgcnt < 20)
    return;

  double avgrpm = 0;
  for(int i = 0; i < 20; i++)
  {
    avgrpm = avgrpm + avgs[i];
  }
  rpm = avgrpm / 20;
  
  avgcnt = 0;




  //Let's keep this RPM value under control, between 0 and 8000 
  //rpm = constrain (rpm, 0, MAX_RPM); 

  // given the nature of the RPM interrupt reader, a zero reading will produce a max result 
  // this fixes this quirk 


    
//    // if the engine is running, print the rpm value to the 7-seg display 
//    //if not, shut her down 
//    if((micros() - lastPulseTime) < 5e6 )
//    { 
////        // Serial.print(rpm); 
////        dis1.setBrightness(2);  //set brightness val
////        printnum(dis1, rpm);
//        //matrix.println(rpm); 
//        
//        
//        delay(10); 
//    } 
//    else 
//    { 
//        matrix.clear(); 
//        matrix.writeDisplay(); 
//    } 
    
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
    alpha4.writeDigitAscii(0, 'M');
    alpha4.writeDigitAscii(1, 'I');
    alpha4.writeDigitAscii(2, 'N');
    alpha4.writeDigitAscii(3, 'D' );
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
    alpha4.writeDigitAscii(0, 'M');
    alpha4.writeDigitAscii(1, 'A');
    alpha4.writeDigitAscii(2, 'X');
    alpha4.writeDigitAscii(3, 'D');
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
  
  delay(10);
}
