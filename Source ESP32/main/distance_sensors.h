#ifndef DISTANCE_SENSORS_H
#define DISTANCE_SENSORS_H

#include <Arduino.h>
//#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <WebSerial.h>

#define SDA_1 21
#define SCL_1 22
#define SDA_2 27
#define SCL_2 26

void setupDistanceSensors();
int readDistanceFront();  
int readDistanceRight();  

#endif
