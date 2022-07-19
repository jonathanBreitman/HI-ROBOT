#include "charge_detect.h"

float battery_samples[BATTERY_SAMPLE_SIZE];
int sample_index = 0;

void setupChargeDetection(){
  pinMode(CHARGE_CONNECTION, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  for(int i=0; i < BATTERY_SAMPLE_SIZE; i++){
    battery_samples[i] = 0.0;
  }
}

bool isCharging(){
  //WebSerial.print("charge_connection: ");
  //WebSerial.println(analogRead(CHARGE_CONNECTION));
  return (analogRead(CHARGE_CONNECTION) > MIN_CHARGE_READ_VAL);
}

bool need_charging(){
  //WebSerial.println("starting charging check");
  float v_read = get_voltage(BATTERY_VOLTAGE_PIN);
  //WebSerial.println("Sampled charge: " + String(v_read, 3) + "V");
  battery_samples[sample_index] = v_read;
  sample_index = (sample_index + 1) % BATTERY_SAMPLE_SIZE;
  int count = 0;
  float sum = 0;
  //WebSerial.println("calculating avg");
  for(int i=0; i < BATTERY_SAMPLE_SIZE; i++){
    if(battery_samples[i] != 0.0){
      count++;
      sum += battery_samples[i];
    }
  }
  //WebSerial.println("done calculating avg");
  if(count == 0){
    return false;
  }
  if((sum/(float)count) < BATTERY_VOLTAGE_MIN){
    WebSerial.println("low battery");
    return true;
  }
  //WebSerial.println("the robot doesn't need charging");
  return false;
}

float get_voltage(int pin_number){
  //Serial.printf("read from %d amount %d\n", pin_number, analogRead(pin_number));
  float battary_precentage = analogRead(pin_number) / 4095.0;
  return battary_precentage * VOLTAGE_RANGE;
}
