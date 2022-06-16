#include "my_utility.h"
#include "motors.h"
#include "distance_sensors.h"

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

#define MANUAL 0
#define AUTONOMOUS 1

#define MANUAL_DRIVE_DELAY 300
#define NO_FIREBASE_DELAY 150

int robotMode = MANUAL;

// Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;


//-------------------------------Firebase--------------------------------------
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
        Serial.println("FAILED reading");
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}

void readRealTimeDB_ValueBool(const char *param_name, bool *output) {
  char start_str[150] = FILE_PATH;
      if (Firebase.RTDB.getBool(&fbdo, strcat(start_str, param_name))) {
        //Serial.println("READ: " + param_name + " succesfully");
        //Serial.println("PATH: " + fbdo.dataPath());
        //Serial.println("TYPE: " + fbdo.dataType());
        //Serial.print("VALUE : ");
        //Serial.println(fbdo.to<int>());
        *output = fbdo.to<bool>(); 
      }
      else {
        Serial.println("FAILED reading");
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}
//-----------------------------------------------------------------------------
//------------------------------Motors-----------------------------------------
void readMotorsDB_Commands() {

  readRealTimeDB_ValueInt("state", &robotMode);
  if(robotMode == MANUAL) {
    readRealTimeDB_ValueInt("speed", &vSpeed);   
    
    readRealTimeDB_ValueBool("forward", &forward);
    
    readRealTimeDB_ValueBool("backwards", &backward);
    
    readRealTimeDB_ValueBool("left", &left);
    
    readRealTimeDB_ValueBool("right", &right);
  }  
  else{
    vSpeed = 255;
  }
}
//-----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200); // Serial port for debugging purposes
  Serial.println("**STARTING ESP SETUP**");
  //pinMode(2, OUTPUT);// Setup led pin
  //digitalWrite(2, true)
  // Setting up pins
  setupMotorPins();
  // Setting up distance sensors
  setupDistanceSnsors();
  // Connect to WiFi
  connectToWiFi(); 
  // Connect to Firebase 
  while(!signupOK)
    FirebaseSetup();
  
  Serial.println("**FINISHED ESP SETUP**");
}

void loop() {
  stopEngine();
  if (Firebase.ready()) {
    Serial.println("firebase is ready");
    readMotorsDB_Commands();
    
    Serial.println("read robot state");
    if (robotMode == MANUAL) {
      setMotorsValueByCommand();
      delay(MANUAL_DRIVE_DELAY);
    }
    else if (robotMode == AUTONOMOUS) {
      Serial.println("entering autonomous movement");  
      // Sample distance sensors
      int distanceRightSense = readDistanceRight(); //distance of sensor 1
      int distanceFrontSense = readDistanceFront(); //distance of sensor 2
      // Initinalize motors accordingly to the sensors (correction of movement according to the data)
      setMotorsValueBySensors(distanceRightSense, distanceFrontSense);
      delay(MANUAL_DRIVE_DELAY);
    }
  }
  else {
    Serial.println("Error: Firebase connection error");
    delay(NO_FIREBASE_DELAY); // The state sample delay
  }
}
