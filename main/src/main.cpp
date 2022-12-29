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

//constants
int count = 0;
// sensor
int colourThreshold;

// motor
int ultrasPin[2][2] = {{ultraRightTrig, ultraRightEcho}, {ultraLeftTrig, ultraLeftEcho}};
double straightConst = 70/58; // Right is master wheel, left is slave wheel.
double reverseConst = 1;

int count = 0;

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
    Serial.println(lineLeftValue);
    int lineMidValue = analogRead(lineMid);
    Serial.println(lineMidValue);
    int lineRightValue = analogRead(lineRight);
    Serial.println(lineRightValue);
    colourThreshold = (lineLeftValue + lineMidValue + lineRightValue) / 12;
  }
}


//startRoutine Left

void runMotor(int left, int right) {
  Serial.println("Run forward");
  if (left > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  if (right > 0) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  analogWrite(enRight, abs(right)); 
  analogWrite(enLeft, abs(left));
}

void accelerate(int direction, int maxSpeed) {
  // -1 is reverse, 1 is straight
  int delayTime = 20; // milliseconds between each speed step
  // accelerate the motor
  if (direction == 1) {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      // runStraight(speed); // set the new speed
      delay(delayTime); // delay between speed steps
    }
  }
  else {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      runMotor(0, speed); // set the new speed
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


void backOff() {
  Serial.println("Back off");
  runMotor(0,0);
  
  runMotor(-150, -170);
  delay(300);

  runMotor(-255,0);
  delay(900);

}

void startRoutineLeft() {
  Serial.println("Start routine Left");
  delay(3000);

  //Spin left
  runMotor(0, 100);
  delay(100);

  //Run straight
  runMotor(70, 58);
  delay(400);

  //Turn and scan with millis

  // Calibrate the QTI
  // calibrate();
  // count++;
}

void startRoutineRight() {
  Serial.println("Start routine Right");
  delay(3000);

  //Spin left
  runMotor(100, 0);
  delay(1000);

  //Run straight
  runMotor(70, 58);
  delay(400);

  //Turn and scan with millis

  // Calibrate the QTI
  // calibrate();
  // count++;
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
  Serial.println("hello");
  delay(100);
  // if (count == 0){
  //   int button_status = digitalRead(buttonLeft);   
  //   Serial.println(button_status);              
  //   delay(100);                                
  //   if (button_status == 1){
      
  //   }
  // } 
  
  // if (count == 0){
  //   int signalLeft = digitalRead(buttonLeft);
  //   Serial.println(signalLeft);

  //   int signalRight = digitalRead(buttonRight);
  //   Serial.println(signalRight);
  //   delay(200);

  //   if (signalLeft == 1){
  //     startRoutineLeft();
  //     count++;
  //   }
  //   else if (signalRight == 1) {
  //     startRoutineRight();
  //     count++;
  //   }
  //   else{
  //       int lineLeftValue = analogRead(lineLeft);
  //   Serial.print("Left:");
  //   Serial.println(lineLeftValue);

  //   //Mid sensor white = 21
  //   int lineMidValue = analogRead(lineMid);
  //   Serial.print("Mid: ");
  //   Serial.println(lineMidValue);

  //   //Right sensor white = 26
  //   int lineRightValue = analogRead(lineRight);
  //   Serial.print("Right: ");
  //   Serial.println(lineRightValue);

  //   if (lineLeftValue < 30){
  //     backOff();
  //   }
  //   else if (lineMidValue < 30){
  //     backOff();
  //   }
  //   else if (lineRightValue < 30){
  //     backOff();
  //   }
  //   else{
  //     runStraight(150,170);
  //   }
  //     }
  // }
    // startRoutine();
  //Left sensor white = 25
  //Left sensor black = 
  


  // put your main code here, to run repeatedly:
  // while ((digitalRead(buttonLeft) || digitalRead(buttonRight)) && count == 0) {
  //   startRoutine();

  // }
  
  // while (!digitalRead(buttonLeft) && !digitalRead(buttonRight && count > 0)) {
  //   // Edge is detected on the right.
  //   if (analogRead(lineRight) < colourThreshold) {
  //     // Back off and make a U-Turn to the left.
  //     backOff(-1);
  //   }
  //   else if (analogRead(lineMid) < colourThreshold) {      
  //     // Go backward and turn back.
  //     backOff(0);
  //   }
  //   // Edge is detected on the left.
  //   else if (analogRead(lineLeft) < colourThreshold) {      
  //     // Back off and make a U-Turn to the right.
  //     backOff(1);
  //   }
  //   else {
  //     runStraight(58);
  //   }
  //   while (digitalRead(buttonLeft) || digitalRead(buttonRight)){
  //     while(1);
  //   }
  // }
}
