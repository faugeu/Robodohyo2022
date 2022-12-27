#include <Arduino.h>
#include <SharpIR.h> 

// Button
const int buttonLeft = 11;
const int buttonRight = 12;

// Motor left
const int enLeft = 3;
const int in1 = 5;
const int in2 = 6;
// Motor right 
const int enRight = 9;
const int in3 = 7;
const int in4 = 8;

// Line sensors
const int lineLeft = A2;
const int lineMid = A1;
const int lineRight = A0;
// Distance sensors
const int ultraLeftTrig = 2;
const int ultraLeftEcho = A3;
const int ultraRightTrig = 13;
const int ultraRightEcho = A5;
SharpIR sensor(SharpIR::GP2Y0A02YK0F, A4);

int searchDirection = 0;

void runMotor(int right, int left) {
  digitalWrite(in1, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enRight, right); 
  analogWrite(enLeft, left);
}

void runMotorReverse(int right, int left) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enRight, right); 
  analogWrite(enLeft, left);
}

void distanceSensors() {
  long duration1, distance1;
  digitalWrite(ultraLeftTrig, LOW);  // Added this line
  delayMicroseconds(2);         // Added this line
  digitalWrite(ultraLeftTrig, HIGH);
  delayMicroseconds(10);  // Added this line
  digitalWrite(ultraLeftTrig, LOW);
  duration1 = pulseIn(ultraLeftEcho, HIGH);
  distance1 = (duration1 / 2) / 29.1;

  if (distance1 >= 500 || distance1 <= 0) {
    Serial.println("Out of range");
  } else {
    Serial.print("SensorLeft: ");
    Serial.print(distance1);
    Serial.println("cm");
  }

  long duration2, distance2;
  digitalWrite(ultraRightTrig, LOW);  // Added this line
  delayMicroseconds(2);         // Added this line
  digitalWrite(ultraRightTrig, HIGH);
  delayMicroseconds(10);  // Added this line
  digitalWrite(ultraRightTrig, LOW);
  duration2 = pulseIn(ultraRightEcho, HIGH);
  distance2 = (duration2 / 2) / 29.1;

  if (distance2 >= 500 || distance2 <= 0) {
    Serial.println("Out of range");
  } else {
    Serial.print("SensorRight ");
    Serial.print(distance2);
    Serial.println("cm");
  }
  int dis = sensor.getDistance();
  Serial.print("IR: ");
  Serial.println(dis);
  delay(1000);
}

int calibrate() {
  int lineAverage;
  while (millis() < 500) {
    int lineLeftValue = analogRead(lineLeft);
    int lineMidValue = analogRead(lineMid);
    int lineRightValue = analogRead(lineRight);
    lineAverage = (lineLeftValue + lineMidValue + lineRightValue) / 12;
  }
  return lineAverage;
}

void setup() {
  Serial.begin(9600);   
  //button
  pinMode(buttonLeft, INPUT);  
  pinMode(buttonRight, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //motor
  pinMode(enLeft,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enRight,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  //distance sensor
  pinMode(ultraLeftTrig,OUTPUT);
  pinMode(ultraLeftEcho,INPUT);
  pinMode(ultraRightTrig,OUTPUT);
  pinMode(ultraRightEcho,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  runMotorReverse();
}