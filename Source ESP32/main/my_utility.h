#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <time.h>
#include <Arduino.h>

#define MANUAL 0
#define AUTONOMOUS 1
#define AUTONOMOUS_IN_CHARGE 2 // In charging in the autonomus mode

//------------------------------WiFi---------------------------
#include <WiFi.h>
//-------------------------------------------------------------

void connectToWiFi();

#endif
