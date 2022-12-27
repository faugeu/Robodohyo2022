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

int getValueIR(){
  int dis = sensor.getDistance(); //get distance from IR
  return dis;
}

int getValueUltraLeft(){
  long duration1, distance1;
  digitalWrite(ultraLeftTrig, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(ultraLeftTrig, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(ultraLeftTrig, LOW);
  duration1 = pulseIn(ultraLeftEcho, HIGH);
  distance1 = (duration1/2) / 29.1;
  return distance1;
}

int getValueUltraRight(){
  long duration2, distance2;
  digitalWrite(ultraRightTrig, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(ultraRightTrig, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(ultraRightTrig, LOW);
  duration2 = pulseIn(ultraRightEcho, HIGH);
  distance2 = (duration2/2) / 29.1;
  return distance2;
}
void runMotor(int right, int left) {
  digitalWrite(in1, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
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

int calibrate() {
  int lineAverage;
  while (millis() < 500) {
    int lineLeftValue = analogRead(lineLeft);
    int lineMidValue = analogRead(lineMid);
    int lineRightValue = analogRead(lineRight);
    lineAverage = (lineLeftValue + lineMidValue + lineRightValue) / 4;
  }
  return lineAverage;
}

int startRoutine() {
  delay(3000);
  // Calibrate the QTI
  calibrate();
}


void attack() {
  int valueIR = getValueIR();
  int valueUltraLeft = getValueUltraLeft();
  int valueUltraright = getValueUltraRight();
  //Move robot forward opponent
  
  if (valueIR <= 80){ //range for robot to attack
    accelerate();
  }
  else{//out of attack range
    //run or continue to detect
  } 


}


void backOff() {

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
  int calibrateValue = startRoutine();
  int lineLeftValue = analogRead(lineLeft);
  int lineMidValue = analogRead(lineMid);
  int lineRightValue = analogRead(lineRight);

  if ((lineLeftValue < calibrateValue) || (lineMidValue < calibrateValue) || (lineRightValue < calibrateValue)){
    runMotor(0,0); 
  }
  else{
    runMotor(75,80);
  }
  
  // put your main code here, to run repeatedly:

  /*
  Button is pressed --> Wait until button is released startRoutine
  If no opponents found --> search.
  If yes --> attack.
  Attack fail --> Tornado in 5s
  Inside search and attack there is back off function.
  */
//  while (digitalRead(buttonLeft) || digitalRead(buttonRight)) {
//     startRoutine();
//  }
 
//  while (!digitalRead(BUTTON)) {
//     if (!digitalRead(EDGE_L)) {
//       // Toggle the search direction.
//       searchDirection += 1;

//       // Back off and make a U-Turn to the right.
//       backOff();

//     }
    
//     // Edge is detected on the right.
//     else if (!digitalRead(EDGE_R)) {
//       // Toggle the search direction.
//       searchDirection += 1;
      
//       // Back off and make a U-Turn to the right.
//       backOff();
//     }

//     else {
//       // Keep searching if opponent is not detected.
//       if ( digitalRead(OPPONENT_FC) &&
//           digitalRead(OPPONENT_FL) &&
//           digitalRead(OPPONENT_FR) &&
//           digitalRead(OPPONENT_L) &&
//           digitalRead(OPPONENT_R) ) {
//         search();
//       }
      
//       // Attack if opponent is in view.
//       else {
//         attack();
//       }
//     }

//  }
}
