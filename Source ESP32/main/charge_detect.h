#ifndef CHARGE_DETECT_H
#define CHARGE_DETECT_H

#include <Arduino.h>


#define CHARGE_CONNECTION 34
#define MIN_CHARGE_READ_VAL 3000

void setupChargeDetection();

bool isCharging();

#endif
