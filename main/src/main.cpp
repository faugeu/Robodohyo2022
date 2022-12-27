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

// sensor
int colourThreshold;

// motor
int ultrasPin[2][2] = {{ultraRightTrig, ultraRightEcho}, {ultraLeftTrig, ultraLeftEcho}};
double straightConst = 1; // Right is master wheel, left is slave wheel.
double reverseConst = 1;


int getValueIR(){
  int distance = sensor.getDistance(); //get distance from IR
  return distance;
}


int getValueUltra(int side) {
  // -1 is left, 1 is right
  long duration, distance;
  int trig, echo;
  if (side == -1) {
    trig = ultraLeftTrig;
    echo = ultraLeftEcho;
  }
  else {
    trig = ultraRightTrig;
    echo = ultraRightEcho;
  }
  digitalWrite(trig, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
  if (distance > 150) {
    distance = 0;
  }
  return distance;
}


void calibrate() {
  while (millis() < 500) {
    int lineLeftValue = analogRead(lineLeft);
    int lineMidValue = analogRead(lineMid);
    int lineRightValue = analogRead(lineRight);
    colourThreshold = (lineLeftValue + lineMidValue + lineRightValue) / 12;
  }
}


void startRoutine() {
  delay(3000);
  // Calibrate the QTI
  calibrate();
}


void runStraight(int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enRight, speed); 
  analogWrite(enLeft, speed * straightConst);
}


void runReverse(int direction = 0, int speed) {
  // -1 is left, 0 is mid and 1 is right
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enRight, speed); 
  analogWrite(enLeft, speed * pow(reverseConst, direction) * straightConst);
}


void accelerate(int direction, int maxSpeed) {
  // -1 is reverse, 1 is straight
  int delayTime = 20; // milliseconds between each speed step
  // accelerate the motor
  if (direction == 1) {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      runStraight(speed); // set the new speed
      delay(delayTime); // delay between speed steps
    }
  }
  else {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      runReverse(speed); // set the new speed
      delay(delayTime); // delay between speed steps
    }
  }
  
}


void attack() {
  int valueIR = getValueIR();
  int valueUltraLeft = getValueUltra(1);
  int valueUltraright = getValueUltra(0);
  //Move robot forward opponent
  
  if (valueIR <= 80){ //range for robot to attack
  }
  else{//out of attack range
    //run or continue to detect
  } 
}


void backOff(int direction) {
  accelerate(-1, 100);
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
  while (digitalRead(buttonLeft) || digitalRead(buttonRight)) {
    startRoutine();
  }
 
  while (!digitalRead(buttonLeft) || digitalRead(buttonRight)) {
    // Edge is detected on the right.
    if (analogRead(lineRight) < colourThreshold) {
      // Back off and make a U-Turn to the left.
      backOff(-1);
    }
    else if (analogRead(lineMid) < colourThreshold) {      
      // Go backward and turn back.
      backOff(0);
    }
    // Edge is detected on the left.
    else if (analogRead(lineLeft) < colourThreshold) {      
      // Back off and make a U-Turn to the right.
      backOff(1);
    }
    else {

    }

  }
}
