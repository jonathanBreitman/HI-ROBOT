#include "charge_detect.h"

#define CHARGE_CONNECTION 13
#define MIN_CHARGE_READ_VAL 3900

void setupChargeDetection(){
  pinMode(CHARGE_CONNECTION, INPUT);
}

bool isCharging(){
  return (analogRead(CHARGE_CONNECTION) > MIN_CHARGE_READ_VAL);
}
