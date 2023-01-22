#include <Arduino.h>
#include <SharpIR.h> 
#include <NewPing.h>

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
const int maxDistance = 80;
SharpIR sensor(SharpIR::GP2Y0A02YK0F, A4);

//constants
int count = 0;
int preDirection = 0;
int lineLimit = 30; // The arena is black with white ring, lineLimit refers to the white value threshold to detect the ring.
int distanceLimit = 60;
// sensor
NewPing sonarLeft(ultraLeftTrig, ultraLeftEcho, maxDistance);
NewPing sonarRight(ultraRightTrig, ultraRightEcho, maxDistance);


int getValueIR();
int getValueUltraLeft();
int getValueUltraRight();
void attack();
void backOff(int);
void startRoutineLeft();
void startRoutineRight();
void runMotor(int, int);
void mainProgram();
int lineCheck();

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
  mainProgram();
}


void mainProgram(){
  if (count == 0){
    int buttonStatusLeft = digitalRead(buttonLeft);
    int buttonStatusRight = digitalRead(buttonRight);   
    Serial.println(buttonStatusLeft);
    Serial.println(buttonStatusRight);
    delay(100);                       

    if (buttonStatusLeft == 1){
      startRoutineLeft();
      count++;
    }
    else if (buttonStatusRight == 1){
      startRoutineRight();
      count++;
    }
  }
  else{
    if (lineCheck()) {
      int IRValue = getValueIR();
      int UltraLeftValue=getValueUltraLeft();
      int UltraRightValue=getValueUltraRight();
      if (IRValue && UltraLeftValue && UltraRightValue ) {
        attack();
      } else if (IRValue && UltraLeftValue == 0 && UltraRightValue == 0){
        attack();
      } else if (IRValue == 0 && UltraLeftValue && UltraRightValue){
        attack();
      } else if (IRValue && UltraLeftValue && UltraRightValue == 0){
        runMotor(255, 170);
        preDirection = 1;
      } else if (IRValue && UltraLeftValue == 0 && UltraRightValue){
        runMotor(170, 255);
        preDirection = -1;
      } else if (IRValue == 0 && UltraLeftValue && UltraRightValue == 0) {
        runMotor(255, -255);
      } else if (IRValue == 0 && UltraLeftValue == 0 && UltraRightValue) {
        // Because the right ultra sonic sensor has a longer range than the other two sensors, 
        // the bot moves slightly forward if the right sensor detects the opponent.
        runMotor(50, 255);
        delay(100);
      } 
      else {
        if (preDirection == -1){
          runMotor(-255,255);
          preDirection = 0;
        } else if (preDirection == 1){
          runMotor(255,-255);
          preDirection = 0;
        }
        else{
          while (true){
            runMotor(255, -255);
            lineCheck();
            if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
              runMotor(0,0);
              break;
            }
          }
        }
      }
    }     
  }
}


int lineCheck() {
  /*
  Detect the ring
  */

  int lineLeftValue = analogRead(lineLeft);
  // // //Mid sensor white = 21
  int lineMidValue = analogRead(lineMid);
  // // //Right sensor white = 26
  int lineRightValue = analogRead(lineRight);

  if (getValueUltraLeft() && getValueUltraRight()) {
    return 1;
  }
  else if (lineLeftValue < lineLimit){
    backOff(-1);
    return 0;
  }
  else if (lineMidValue < lineLimit){
    runMotor(255,255);
    delay(500);
    return 0;
  }
  else if (lineRightValue < lineLimit){
    backOff(1);
    return 0;
  }
  return 1;
}


int getValueIR()
{
  /*
  To get values from the IR sensor
  */

  // Because there is a fluctuation in IR's values hence we have to take multiple samples and choose the longest distance to filter false values
  int maxDistance = 0;
  int samples = 10;
  int interval = 38;
  int delayBurst = 1.5;
  int count = 0;
  int IRValue;
  while (count < samples) {
    IRValue = sensor.getDistance();
    if (IRValue > maxDistance) {
      maxDistance = IRValue;
    }
    delay(delayBurst);
    count++;
  }
  delay(interval);
  // Any search distance which is longer than 45cm will result in false values. 
  if (maxDistance > 45) {
    digitalWrite(LED_BUILTIN, LOW);
    return 0;
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    return 1;
  }
}


int getValueUltraLeft() 
{ 
  /*
  To get values from the ultra sensor to the left.
  */

  int distance = sonarLeft.ping_cm();
  delay(10);
  // Any search distance which is longer than 45cm will result in false values. 
  if (distance > 45 || distance == 0) {
    return 0;
  }else{
    return 1;
  }
}


int getValueUltraRight() 
{
  /*
  To get values from the ultra sensor to the right.
  */

  int distance = sonarRight.ping_cm();
  delay(10);
  // Any search distance which is longer than 70cm will result in false values. 
  if (distance > 70 || distance == 0) {
    return 0;
  }else{
    return 1;
  }
}


void runMotor(int left, int right) {
  /*
  To control the motor. Positive values move the wheel forward and vice versa.
  */

  if (left > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
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


void attack() 
{
  runMotor(255, 255);
}


void backOff(int direction) 
{
  /*
  Back off when the bot reaches the ring.
  -1 if left sensor detected and 1 if right sensor detected
  */

  //Stop
  runMotor(0,0);
  
  //Run backward
  if (direction == -1){
  runMotor(-255, -100);
  }
  else if (direction == 1){
    runMotor(-100,-255);
  }
  
  delay(700);

  if (direction == -1){
    runMotor(255,-255);
    
  }
  else if (direction == 1){
    runMotor(-255,255);
  }
  
  while (true){
    // The bot spins around until it finds the opponent.
    if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
      runMotor(0,0);
      break;
    }
  }
}

void startRoutineLeft() 
{
  /*
  Start routine when hitting the left button.
  */

  delay(3000);
  runMotor(-255, 255); // Spin around until the bot detects the opponent.
  

  while (true){
    if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
      runMotor(0,0);
      break;
    }
  }
}

void startRoutineRight() 
{
  /*
  Start routine when hitting the right button.
  */

  delay(3000);
  runMotor(255, -255);

  while (true){
    if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
      runMotor(0,0);
      break;
    }
  }
}