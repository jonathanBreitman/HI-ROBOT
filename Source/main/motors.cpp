#include "motors.h"

//Motor L298 PINS
const int motorA1  = 33;  // IN3 Input of L298N
const int motorA2  = 32;  // IN1 Input of L298N
const int motorB1  = 25;  // IN2 Input of L298N
const int motorB2  = 26;  // IN4 Input of L298N

int vSpeed = 255;   // Standard Speed can take a value between 0-255
bool backward = false;
bool right = false;
bool left = false;
bool forward = false;


void setupMotorPins(){
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);  
}


void readMotorsDB_Commends() {
  
  readRealTimeDB_Value_Int("speed", &vSpeed);
  
  readRealTimeDB_Value_Bool("forward", &forward);
  
  readRealTimeDB_Value_Bool("backward", &backward);
  
  readRealTimeDB_Value_Bool("left", &left);
  
  readRealTimeDB_Value_Bool("right", &right);  
}


void setMotorsValueByCommend() {
  if (forward && !left && !right){
    Serial.println("Go Forward");
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
  } else if (left && !forward && !backward) {
    Serial.println("Go Left");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
  } else if (right && !forward && !backward) {
    Serial.println("Go Right");
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, vSpeed);
  } else if (backward && !left && !right) {
    Serial.println("Go Backward");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, vSpeed);
  } else if (!forward && !backward && !left && !right){
    Serial.println("Stopping");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, 0);
  } //TODO: add multi-directional movment
}
