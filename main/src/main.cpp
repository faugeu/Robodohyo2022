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
const int maxDistance = 200;
SharpIR sensor(SharpIR::GP2Y0A02YK0F, A4);

//constants
int count = 0;
// sensor
int colourThreshold;
NewPing sonarLeft(ultraLeftTrig, ultraLeftEcho, maxDistance);
NewPing sonarRight(ultraRightTrig, ultraRightEcho, maxDistance);


int getValueIR();
int getValueUltraLeft();
int getValueUltraRight();
void calibrate();
void accelerate(int direction, int maxSpeed);
void attack();
void backOff(int);
void startRoutineLeft();
void startRoutineRight();
void runMotor(int, int);

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
    int lineLeftValue = analogRead(lineLeft);
  Serial.print("Left:");
  Serial.println(lineLeftValue);

  //Mid sensor white = 21
  int lineMidValue = analogRead(lineMid);
  Serial.print("Mid: ");
  Serial.println(lineMidValue);

  //Right sensor white = 26
  int lineRightValue = analogRead(lineRight);
  Serial.print("Right: ");
  Serial.println(lineRightValue);

  if (lineLeftValue > 100){
    Serial.println("Backoff");
    backOff(-1);
  }
  else if (lineMidValue > 100){
    runMotor(70,70);
  }
  else if (lineRightValue > 100){
    Serial.println("Backoff");
    backOff(1);
  }
  else{
    if (getValueIR()) {
      attack();
    } else if (getValueUltraLeft()) {
      runMotor(255, -255);
    } else if (getValueUltraRight()) {
      runMotor(-255, 255);
    } else {
      runMotor(255, -255);
      int timeStamp = millis();
      while (millis() - timeStamp < 1000){ // Test time to turn 270 or 135 degrees
        if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
          runMotor(0,0);
          break;
        }
      }
      runMotor(70,70);
    }
  }     
    }
}

int getValueIR()
{
  // int distance = sensor.getDistance(); //get distance from IR
  // return distance;
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
  if (maxDistance > 120) {
    maxDistance = 0;
  }
  return maxDistance;
}


int getValueUltraLeft() 
{
  int distance = sonarLeft.ping_cm();
  delay(100);
  if (distance > 150) {
    distance = 0;
  }
  return distance;
}


int getValueUltraRight() 
{
  int distance = sonarRight.ping_cm();
  delay(100);
  if (distance > 150) {
    distance = 0;
  }
  return distance;
}

void runMotor(int left, int right) {
  if (left > 0) {
    Serial.println("Left forwarded");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    Serial.println("Left backwarded");
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  if (right > 0) {
    Serial.println("Right forwarded");
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    Serial.println("Right backwarded");
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  analogWrite(enRight, abs(right)); 
  analogWrite(enLeft, abs(left));
}


void accelerate(int direction, int maxSpeed) {
  // -1 is reverse, 1 is straight
  Serial.println("Accelerate");
  int delayTime = 10; // milliseconds between each speed step
  // accelerate the motor
  if (direction == 1) {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      runMotor(speed, speed); // set the new speed
      delay(delayTime); // delay between speed steps
    }
  }
  else {
    for(int speed = 0; speed <= maxSpeed; speed++) { // counts from 0 to 255 (max speed) using the variable "speed"
      runMotor(-speed, -speed); // set the new speed
      delay(delayTime); // delay between speed steps
    }
  }
  
}


void attack() 
{
  Serial.println("Attack");
   runMotor(255, 255);
}


void backOff(int direction) 
{
  //-1 if left sensor detected and 1 if right sensor detected
  //Stop
  Serial.println("Back off");
  runMotor(0,0);
  
  //Run backward
  runMotor(-255, -255);
  delay(300);

  //Turn and scan
  if (direction == -1)
  {
    runMotor(-255,255);
  }
  else{
    runMotor(255,-255);
  }
  int timeStamp = millis();
    while (millis() - timeStamp < 1000){ // Test time to turn 270 or 135 degrees
      if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
        runMotor(0,0);
        return;
      }
    }
}

void startRoutineLeft() 
{
  Serial.println("Start routine Left");
  delay(3000);

  //Spin left
  runMotor(-255, 255);
  delay(250);

  //Run straight
  runMotor(100,100);
  delay(1500);

  //Turn right until opponent is detect
  runMotor(255, -255);
  int timeStamp = millis();
    while (millis() - timeStamp < 1000){ // Test time to turn 270 or 135 degrees
      if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
        runMotor(0,0);
        return;
      }
    }
}

void startRoutineRight() 
{
  Serial.println("Start routine Right");
  delay(3000);

  //Spin left
  runMotor(100, -100);
  delay(400);

  //Run straight
  accelerate(70, 58);
  delay(400);

  //Turn left until detected
  runMotor(255, -255);
  int timeStamp = millis();
    while (millis() - timeStamp < 1000){ // Test time to turn 270 or 135 degrees
      if (getValueIR() || getValueUltraLeft() || getValueUltraRight()){
        runMotor(0,0);
        return;
      }
    }
}
