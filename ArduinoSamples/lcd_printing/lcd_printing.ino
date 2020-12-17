/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int seconds = 55;

void setup() {
  lcd.begin(16, 2);
 
}

// the loop routine runs over and over again forever:
void loop() {
  lcd.setCursor(0, 0);
  int minutes = seconds/60;
    lcd.print(minutes);
    lcd.print(":");
    if((seconds - ((seconds/60)*60)) < 10)
      lcd.print("0");
    lcd.print(seconds - ((seconds/60)*60));
    
    seconds++;
    delay(1000);
}
