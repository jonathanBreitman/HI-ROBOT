#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

extern int vSpeed;   // Standard Speed can take a value between 0-255
extern bool backward;
extern bool right;
extern bool left;
extern bool forward;

void setupMotorPins();
void setMotorsValueByCommand();

#endif
