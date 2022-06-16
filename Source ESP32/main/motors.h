#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#define MIN_DISTANCE_FRONT 100
#define MIN_DISTANCE_RIGHT 50
#define MAX_DISTANCE_RIGHT 100

#define CORNER_DELAY 600
#define WALL_DIST_CORRECTION_DELAY 60
#define MOVE_FORWARD_DELAY 800

extern int vSpeed;   // Standard Speed can take a value between 0-255
extern bool backward;
extern bool right;
extern bool left;
extern bool forward;

void setupMotorPins();
void setMotorsValueByCommand();
void setMotorsValueBySensors(int distance_right, int distance_front);
void stopEngine();

#endif