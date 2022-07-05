#ifndef CHARGE_DETECT_H
#define CHARGE_DETECT_H

#include <Arduino.h>

#define CHARGE_CONNECTION 4
#define MIN_CHARGE_READ_VAL 3900

void setupChargeDetection();

bool isCharging();

#endif
