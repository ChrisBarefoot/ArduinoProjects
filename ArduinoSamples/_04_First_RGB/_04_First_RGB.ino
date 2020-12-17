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

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  redSVal = analogRead(redSensorPin);
  delay(5);
  greenSVal = analogRead(greenSensorPin);
  delay(5);
  blueSVal = analogRead(blueSensorPin);
  
  Serial.print("Raw Sensor Value \t Red: ");
  Serial.print(redSVal);
  Serial.print("\t Green: ");
  Serial.print(greenSVal);
  Serial.print("\t Blue: ");
  Serial.print(blueSVal);
  
  redVal = redSVal / 4;
  greenVal = greenSVal / 4;
  blueVal = blueSVal / 4;
  
  Serial.print("Mapped Sensor Values \t Red: ");
  Serial.print(redVal);
  Serial.print("\t Green: ");
  Serial.print(greenVal);
  Serial.print("\t Blue: ");
  Serial.print(blueVal);
  
  analogWrite(redLEDPin, redVal);
  analogWrite(greenLEDPin, greenVal);
  analogWrite(blueLEDPin, blueVal);
}
