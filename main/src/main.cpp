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


int getValueIR(){
  int distance = sensor.getDistance(); //get distance from IR
  return distance;
}


int getValueUltra(int side) {
  // 0 is right, 1 is left
  long duration, distance;
  int trig = ultrasPin[side][0];
  int echo = ultrasPin[side][1];
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


void runMotor(int right, int left) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
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


void accelerate() {
  int delayTime = 20; // milliseconds between each speed step
  // accelerate the motor
  for(int speed = 0; speed <= 255; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
    runMotor(speed, speed); // set the new speed
    delay(delayTime); // delay between speed steps
  }
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


void attack() {
  int valueIR = getValueIR();
  int valueUltraLeft = getValueUltra(1);
  int valueUltraright = getValueUltra(0);
  //Move robot forward opponent
  
  if (valueIR <= 80){ //range for robot to attack
    accelerate();
  }
  else{//out of attack range
    //run or continue to detect
  } 
}


void backOff(int direction) {

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
  // int calibrateValue = startRoutine();
  // int lineLeftValue = analogRead(lineLeft);
  // int lineMidValue = analogRead(lineMid);
  // int lineRightValue = analogRead(lineRight);

  // Serial.println(calibrateValue);

  // if ((lineLeftValue < calibrateValue) || (lineMidValue < calibrateValue) || (lineRightValue < calibrateValue)){
  //   runMotor(0,0); 
  // }
  // else{
  //   runMotor(75,80);
  // }

  // put your main code here, to run repeatedly:

  /*
  Button is pressed --> Wait until button is released startRoutine
  If no opponents found --> search.
  If yes --> attack.
  Attack fail --> Tornado in 5s
  Inside search and attack there is back off function.
  */
 while (digitalRead(buttonLeft) || digitalRead(buttonRight)) {
    startRoutine();
 }
 
 while (!digitalRead(buttonLeft) || digitalRead(buttonRight)) {
    // Edge is detected on the right.
    if (analogRead(lineRight) < colourThreshold) {
      // Back off and make a U-Turn to the left.
      backOff(0);

    }
    
    // Edge is detected on the left.
    else if (analogRead(lineLeft) < colourThreshold) {      
      // Back off and make a U-Turn to the right.
      backOff(1);
    }

    else if (analogRead(lineMid) < colourThreshold) {      
      // Go backward and turn back.
      backOff(2);
    }

    else {
      
    }

 }
}
