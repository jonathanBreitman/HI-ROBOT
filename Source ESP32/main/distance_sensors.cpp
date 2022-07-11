#include "distance_sensors.h"

//Sensor 1 - right facing Sensor (no loop on the sensor)
//Sensor 2 - front facing Sensor (red loop on the sensor)
  
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);
SFEVL53L1X distanceSensor1(I2Cone);
SFEVL53L1X distanceSensor2(I2Ctwo);

//sets up 2 sda and scl in the define pins
void setupDistanceSensors() {
  Serial.println("Setting up Two VL53L1X - distance sensors");
  uint32_t my_freq = 100000;
  int my_sda = SDA_1;
  int my_scl = SCL_1;
  I2Cone.begin(my_sda, my_scl, my_freq);
  my_sda = SDA_2;
  my_scl = SCL_2;
  I2Ctwo.begin(my_sda, my_scl, my_freq);

  
  if (distanceSensor1.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 1 failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor 1 online!");
  
  if (distanceSensor2.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 2 failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor 2 online!");
}


// Returns the distance from the sensor in mm
int readDistanceFront() {
  //WebSerial.println("reading distance front");
  distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor2.checkForDataReady())
    delay(1);
    
  int distance = distanceSensor2.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor2.clearInterrupt();
  distanceSensor2.stopRanging();

  Serial.print("Distance of sensor facing forward");
  Serial.print("(mm): ");
  Serial.println(distance);
  //WebSerial.println("done reading distance front");
  return distance;
}

int readDistanceRight() {
  //WebSerial.println("reading distance right");
  distanceSensor1.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor1.checkForDataReady())
    delay(1);
    
  int distance = distanceSensor1.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor1.clearInterrupt();
  distanceSensor1.stopRanging();

  Serial.print("Distance of sensor facing right");
  Serial.print("(mm): ");
  Serial.println(distance);
  //WebSerial.println("done reading distance right");
  return distance;
}
