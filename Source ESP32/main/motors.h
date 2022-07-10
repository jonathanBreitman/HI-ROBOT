#ifndef MOTORS_H
#define MOTORS_H
#include "my_utility.h"
#include <Arduino.h>
#define MIN_DISTANCE_FRONT 200
#define MIN_DISTANCE_RIGHT 130
#define MAX_DISTANCE_RIGHT 200

#define CORNER_DELAY 500
#define WALL_DIST_CORRECTION_DELAY 130
#define MOVE_FORWARD_DELAY 600
#define CORNER_DELAY_FORWARD 400
#define RIGHT_CORRECTION_FORWARD 240
#define LEFT_CORRECTION_FORWARD 120
#define LEFT_CORRECTION 80
#define SHAKE_DELAY 20

#include <TB6612FNG.h>
#include <WebSerial.h>

#define AIN1 13
#define BIN1 23
#define AIN2 14
#define BIN2 19
#define PWMA 25
#define PWMB 18
#define STBY 33

extern int vSpeed;   // Standard Speed can take a value between 0-255
extern bool backward;
extern bool right;
extern bool left;
extern bool forward;

void setupMotorPins();
void setMotorsValueByCommand(int delay_movement);
void setMotorsValueBySensors(int distance_right, int distance_front);
void stopEngine();

bool chargingHandle(int distanceFront);
void turn_90_degree_left();
void shake_to_charge(int iteration);

#endif
