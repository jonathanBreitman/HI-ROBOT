#include "charge_detect.h"

void setupChargeDetection(){
  pinMode(CHARGE_CONNECTION, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
}

bool isCharging(){
  return (analogRead(CHARGE_CONNECTION) > MIN_CHARGE_READ_VAL);
}

float battery_samples[BATTERY_SAMPLE_SIZE] = {0};
int sample_index = 0;

bool need_charging(){
  float v_read = get_voltage(BATTERY_VOLTAGE_PIN);
  WebSerial.println("Sampled charge: " + String(v_read, 3) + "V");
  battery_samples[sample_index] = v_read;
  sample_index = (sample_index + 1) % BATTERY_SAMPLE_SIZE;
  int count = 0;
  float sum = 0;
  for(int i=0; i < BATTERY_SAMPLE_SIZE; i++){
    if(battery_samples[i] != 0){
      count++;
      sum += battery_samples[i];
    }
  }
  if(count == 0){
    return false;
  }
  return ((sum/(float)count) < BATTERY_VOLTAGE_MIN);
}

float get_voltage(int pin_number){
  float battary_precentage = analogRead(pin_number) / 4095.0;
  return battary_precentage * VOLTAGE_RANGE;
}
