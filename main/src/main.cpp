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


void accelerate() {
  int delayTime = 20; // milliseconds between each speed step
  // accelerate the motor
  for(int speed = 0; speed <= 255; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
    runMotor(speed, speed); // set the new speed
    delay(delayTime); // delay between speed steps
  }
}


void startRoutine() {
  delay(3000);
  // Calibrate the QTI
}


void attack() {

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
