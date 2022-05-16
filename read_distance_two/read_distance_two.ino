#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

SFEVL53L1X distanceSensor1(I2Cone);
SFEVL53L1X distanceSensor2(I2Ctwo);

#define SDA_1 21
#define SCL_1 22
#define SDA_2 27
#define SCL_2 26

void setup(void)
{
  int my_sda = SDA_1;
  int my_scl = SCL_1;
  uint32_t my_freq = 100000;
  I2Cone.begin(my_sda, my_scl, my_freq);
  my_sda = SDA_2;
  my_scl = SCL_2;
  I2Ctwo.begin(my_sda, my_scl, my_freq);

  Serial.begin(115200);
  Serial.println("Two VL53L1X Qwiic Test");

  if (distanceSensor1.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 1 failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }

  Serial.println("Sensor 2 online!");
  if (distanceSensor2.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 2 failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor 2 online!");
}

void loop(void)
{
  distanceSensor1.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor1.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor1.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor1.clearInterrupt();
  distanceSensor1.stopRanging();

  Serial.print("Distance 1(mm): ");
  Serial.print(distance);

  distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor2.checkForDataReady())
  {
    delay(1);
  }
  int distance2 = distanceSensor2.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor2.clearInterrupt();
  distanceSensor2.stopRanging();

  Serial.print("Distance 2(mm): ");
  Serial.print(distance2);
  
  Serial.println();
  delay(2000);
}