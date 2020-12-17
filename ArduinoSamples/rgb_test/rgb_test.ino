/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
const int greenLEDPin = 9;
const int redLEDPin = 11;
const int blueLEDPin = 10;

const int redSensorPin = A0;
const int greenSensorPin = A1;
const int blueSensorPin = A2;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;

int redSVal = 0;
int greenSVal = 0;
int blueSVal = 0;

int counter = 0;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {

  //redVal++;
  //greenVal++;
  //blueVal++;
  if(counter == 0)
  {
    analogWrite(redLEDPin, 0);
    analogWrite(greenLEDPin, 10);
    analogWrite(blueLEDPin, 0);
  }
  else if(counter == 1)
  {
    analogWrite(redLEDPin, 0);
    analogWrite(greenLEDPin, 0);
    analogWrite(blueLEDPin, 0);
  }
  else if(counter == 2)
  {
    analogWrite(redLEDPin, 0);
    analogWrite(greenLEDPin, 10);
    analogWrite(blueLEDPin, 10);
  }
  counter++;
  if(counter > 2)
    counter = 0;
  delay(400);
}
