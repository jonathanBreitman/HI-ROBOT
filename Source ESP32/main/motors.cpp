#include "motors.h"

//Motor L298 PINS
const int motorA1  = 32;  // IN3 Input of L298N
const int motorA2  = 33;  // IN1 Input of L298N
const int motorB1  = 18;  // IN2 Input of L298N
const int motorB2  = 19;  // IN4 Input of L298N

int vSpeed = 255;   // Standard Speed can take a value between 0-255
bool backward = false;
bool right = false;
bool left = false;
bool forward = false;

void setupMotorPins() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);  
}


void setMotorsValueByCommand() {
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
  } //TODO: add multi-directional movement
}

void setMororsValueBySensors(int distance_right, int distance_front) {
  if (distance_front < MIN_DISTANCE_FRONT) {//We are close to a corner, turn left
    Serial.println("Go Left");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(CORNER_DELAY);
    return;
  }

  if (distance_right > MAX_DISTANCE_RIGHT) {//We are far from the wall, change the direction right a little bit
    Serial.println("Go Right");
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, vSpeed);
    delay(RIGHT_CORRECTION_DELAY);
    return;
  }

  if (distance_right < MIN_DISTANCE_RIGHT) {//We are too close to the wall, change the direction left a little bit
    Serial.println("Go Left");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(RIGHT_CORRECTION_DELAY);
    return;
  }

  //If we got to this point, we are not in a corner and the robot is in an acceptable distance from the right wall, move forward
  Serial.println("Go Forward");
  digitalWrite(motorA1, vSpeed);
  digitalWrite(motorA2, 0);
  digitalWrite(motorB1, vSpeed);
  digitalWrite(motorB2, 0);
  delay(MOVE_FORWARD_DELAY);
}
