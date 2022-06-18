#include "motors.h"

//Motor L298 PINS
const int motorA1  = 32;  // IN3 Input of L298N //A is left wheel
const int motorA2  = 33;  // IN1 Input of L298N
const int motorB1  = 18;  // IN2 Input of L298N //B is right wheel
const int motorB2  = 19;  // IN4 Input of L298N
/*
const int motorA1Channel = 0;
const int motorA2Channel = 1;
const int motorB1Channel = 2;
const int motorB2Channel = 3;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 200;
*/
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

  /*
  ledcSetup(pwmChannel,freq,resolution);

  ledcAttachPin(motorA1, OUTPUT);
  ledcAttachPin(motorA2, OUTPUT);
  ledcAttachPin(motorB1, OUTPUT);
  ledcAttachPin(motorB2, OUTPUT);
  */
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
  } else if (forward && left && !backward && !right){
    Serial.println("forward and left");
    digitalWrite(motorA1, (int)vSpeed / 2);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
  } else if (forward && !left && !backward && right){
    Serial.println("forward and right");
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, (int)vSpeed / 2);
    digitalWrite(motorB2, 0);
  } else if (!forward && !left && backward && right){
    Serial.println("backwards and right");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, (int)vSpeed / 2);
  } else if (!forward && left && backward && !right){
    Serial.println("backwards and left");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, (int)vSpeed / 2);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, vSpeed);
  }
}

void setMotorsValueBySensors(int distance_right, int distance_front) {
  if (distance_front < MIN_DISTANCE_FRONT) {//We are close to a corner, turn left
    Serial.println("Go Left (90 degree corner)");
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(CORNER_DELAY);
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(MOVE_FORWARD_DELAY/2);
  }
  else if (distance_right > MAX_DISTANCE_RIGHT) {//We are far from the wall, change the direction right a little bit
    Serial.println("Go Right");
    /*ledcWrite(motorA1Channel, vSpeed);
    ledcWrite(motorA2Channel, vSpeed);
    ledcWrite(motorA1, vSpeed);
    ledcWrite(motorA1, vSpeed);
    */
    //turn to the right
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, 0);
    digitalWrite(motorB2, 0);
    delay(WALL_DIST_CORRECTION_DELAY);
    //continue slightly forward
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(WALL_DIST_CORRECTION_DELAY*5/2);
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(WALL_DIST_CORRECTION_DELAY/2);
  }
  else if (distance_right < MIN_DISTANCE_RIGHT) {//We are too close to the wall, change the direction left a little bit
    Serial.println("Go Left");
    //turn to the left
    digitalWrite(motorA1, 0);
    digitalWrite(motorA2, vSpeed);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(WALL_DIST_CORRECTION_DELAY);
    //continue slightly forward
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(WALL_DIST_CORRECTION_DELAY);
    
  }
  else {//If we got to this point, we are not in a corner and the robot is in an acceptable distance from the right wall, move forward
    Serial.println("Go Forward");
    digitalWrite(motorA1, vSpeed);
    digitalWrite(motorA2, 0);
    digitalWrite(motorB1, vSpeed);
    digitalWrite(motorB2, 0);
    delay(MOVE_FORWARD_DELAY);
  }
}

void stopEngine(){
  digitalWrite(motorA1, 0);
  digitalWrite(motorA2, 0);
  digitalWrite(motorB1, 0);
  digitalWrite(motorB2, 0);
}
