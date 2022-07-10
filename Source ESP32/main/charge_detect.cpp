#include "charge_detect.h"


void setupChargeDetection(){
  pinMode(CHARGE_CONNECTION, INPUT);
}

bool isCharging(){
  return (analogRead(CHARGE_CONNECTION) > MIN_CHARGE_READ_VAL);
}
