#ifndef CHARGE_DETECT_H
#define CHARGE_DETECT_H

#include <Arduino.h>
#include <WebSerial.h>

#define CHARGE_CONNECTION 34
#define BATTERY_VOLTAGE_PIN 39
#define BATTERY_VOLTAGE_MIN 3.5
#define MIN_CHARGE_READ_VAL 3000
#define VOLTAGE_RANGE 7.5
#define BATTERY_SAMPLE_SIZE 5

void setupChargeDetection();

bool isCharging();
bool need_charging();
float get_voltage(int pin_number);

#endif
