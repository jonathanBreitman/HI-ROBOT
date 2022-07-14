#include <TB6612FNG.h>
#include <Arduino.h>
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Firebase project API Key
#define API_KEY "AIzaSyAWjSDHTgJVYfuhwAVy9StRqDm_eqc0lwU"
// RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hi-robot-c64d7-default-rtdb.firebaseio.com/" 

// Path for file in Firebase
#define FILE_PATH "wirelessCar/" 


#include <Wire.h>
#include "SparkFun_VL53L1X.h"

#define AIN1 13
#define BIN1 23
#define AIN2 14
#define BIN2 19
#define PWMA 25
#define PWMB 18
#define STBY 33

#define SDA_1 21
#define SCL_1 22
#define SDA_2 27
#define SCL_2 26

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);
SFEVL53L1X distanceSensor1(I2Cone);
SFEVL53L1X distanceSensor2(I2Ctwo);


Tb6612fng* motors;

void motors_setup(){
  motors = new Tb6612fng(STBY, AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
  motors->begin();
}

void motors_loop(){
  motors->drive(1.0, 1.0, 500);
  delay(200);
  motors->drive(-1.0, -1.0, 500);
  delay(200);
}

void sensors_setup() {
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

int readDistanceFront() {
  distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor2.checkForDataReady())
    delay(1);
    
  int distance = distanceSensor2.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor2.clearInterrupt();
  distanceSensor2.stopRanging();

  Serial.print("Distance of sensor facing forward");
  Serial.print("(mm): ");
  Serial.println(distance);
  return distance;
}

int readDistanceRight() {
  distanceSensor1.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor1.checkForDataReady())
    delay(1);
    
  int distance = distanceSensor1.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor1.clearInterrupt();
  distanceSensor1.stopRanging();

  Serial.print("Distance of sensor facing right");
  Serial.print("(mm): ");
  Serial.println(distance);
  return distance;
}

void sensors_loop(){
  readDistanceRight();
  delay(50);
  readDistanceFront();
  delay(50);
}

const char* WIFI_SSID = "IOT";
const char* WIFI_PASSWORD = "dreamy128";

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void havhav(){
  pinMode(2, OUTPUT);
  for(int i=0; i<4; i++){
    delay(500);
    digitalWrite(2,HIGH);
    delay(500);
    digitalWrite(2,LOW);
  }
}

#define CHARGE_CONNECTION 34
#define BATTERY_VOLTAGE_PIN 39
#define BATTERY_VOLTAGE_MIN 3.5
#define MIN_CHARGE_READ_VAL 3000
#define VOLTAGE_RANGE 7.5
#define BATTERY_SAMPLE_SIZE 5

float battery_samples[BATTERY_SAMPLE_SIZE];
int sample_index = 0;

void setupChargeDetection(){
  pinMode(CHARGE_CONNECTION, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  for(int i=0; i < BATTERY_SAMPLE_SIZE; i++){
    battery_samples[i] = 0.0;
  }
}

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

void FirebaseSetup() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.println("Error: sign up to Firebase");
    //Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void readRealTimeDB_ValueInt(const char *param_name, int *output) {
      char start_str[150] = FILE_PATH;
      if (Firebase.RTDB.get(&fbdo, strcat(start_str, param_name))) {
        //Serial.println("READ: " + param_name + " succesfully");
        //Serial.println("PATH: " + fbdo.dataPath());
        //Serial.println("TYPE: " + fbdo.dataType());
        //Serial.print("VALUE : ");
        //Serial.println(fbdo.to<int>());
        *output = fbdo.to<int>(); 
      }
      else {
        Serial.print("FAILED reading ");
        Serial.println(param_name);
        Serial.printf("FAILED reading %s", param_name);
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}


bool isCharging(){
  Serial.print("charge_connection: ");
  Serial.println(analogRead(CHARGE_CONNECTION));
  return (analogRead(CHARGE_CONNECTION) > MIN_CHARGE_READ_VAL);
}

bool need_charging(){
  Serial.println("starting charging check");
  float v_read = get_voltage(BATTERY_VOLTAGE_PIN);
  Serial.println("Sampled charge: " + String(v_read, 3) + "V");
  battery_samples[sample_index] = v_read;
  sample_index = (sample_index + 1) % BATTERY_SAMPLE_SIZE;
  int count = 0;
  float sum = 0;
  Serial.println("calculating avg");
  for(int i=0; i < BATTERY_SAMPLE_SIZE; i++){
    if(battery_samples[i] != 0.0){
      count++;
      sum += battery_samples[i];
    }
  }
  Serial.println("done calculating avg");
  if(count == 0){
    return false;
  }
  if((sum/(float)count) < BATTERY_VOLTAGE_MIN){
    Serial.println("low battery");
    return true;
  }
  Serial.println("the robot doesn't need charging");
  return false;
}

float get_voltage(int pin_number){
  //Serial.printf("read from %d amount %d\n", pin_number, analogRead(pin_number));
  float battary_precentage = analogRead(pin_number) / 4095.0;
  return battary_precentage * VOLTAGE_RANGE;
}

int my_state;

void my_thread(void* param){
  readRealTimeDB_ValueInt("state", &my_state);
  Serial.printf("my_thread read %d", my_state);
  delay(3000);
}

void setup() {
  havhav();
  Serial.begin(115200);
  connectToWiFi();
  while(!signupOK)
    FirebaseSetup();
  motors_setup();
  sensors_setup();
  setupChargeDetection();
  TaskHandle_t xHandle = NULL;
  xTaskCreate(my_thread, "my_thread", 40000, NULL, tskIDLE_PRIORITY, &xHandle);
  configASSERT(xHandle);
}

void loop() {
  motors_loop();
  sensors_loop();
  isCharging();
  need_charging();
}
