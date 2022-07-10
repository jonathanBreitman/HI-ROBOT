#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <time.h>
#include <Arduino.h>

#define CHARGING_INTERVAL 1 // How much time between two chraging
#define CHARGING_TIME 3600     // How much time to charge
#define FORWARD_CHARGING_DELAY 350      // Enter charging station 
#define MANUAL 0
#define AUTONOMOUS 1
#define AUTONOMOUS_IN_CHARGE 2 // In charging in the autonomus mode

//------------------------------WiFi---------------------------
#include <WiFi.h>
//-------------------------------------------------------------

void connectToWiFi();

#endif
