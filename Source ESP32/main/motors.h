#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#define MIN_DISTANCE_FRONT 100
#define MIN_DISTANCE_RIGHT 50
#define MAX_DISTANCE_RIGHT 100

#define CORNER_DELAY 3000
#define RIGHT_CORRECTION_DELAY 500
#define MOVE_FORWARD_DELAY 1000

extern int vSpeed;   // Standard Speed can take a value between 0-255
extern bool backward;
extern bool right;
extern bool left;
extern bool forward;

void setupMotorPins();
void setMotorsValueByCommand();
void setMororsValueBySensors(int distance_right, int distance_front);
void stopEngine();

#endif
