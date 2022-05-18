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

#define SIDE_WALL_SENSOR 0
#define FRONT_WALL_SENSOR 1

#define MANUAL 0
#define AUTONOMOUS 1

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
  char start_str[150] = "wirelessCar/";
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
  
  readRealTimeDB_ValueInt("speed", &vSpeed);
  
  readRealTimeDB_ValueBool("forward", &forward);
  
  readRealTimeDB_ValueBool("backward", &backward);
  
  readRealTimeDB_ValueBool("left", &left);
  
  readRealTimeDB_ValueBool("right", &right);  
}
//-----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("**STARTING SETUP**");
  
  // Setting up pins
  setupMotorPins();
  // Setting up distance sensors
  setupDistanceSnsors();
  // Connect to WiFi
  connectToWiFi(); 
  // Connect to Firebase 
  FirebaseSetup();
  
  Serial.println("**FINISHED SETUP**");
}

void loop() {
  if (Firebase.ready() && signupOK) {
    readRealTimeDB_ValueInt("state", &robotMode); //read robot  state
    if (robotMode == MANUAL) {
      readMotorsDB_Commands();
      setMotorsValueByCommand();
    }
    else if (robotMode == AUTONOMOUS) {  
      //sample distance sensors
      int distance1 = readDistance(SIDE_WALL_SENSOR); //distance of sensor 1
      int distance2 = readDistance(FRONT_WALL_SENSOR); //distance of sensor 2

      //initinalize motors accordingly to the sensors (correction of movement according to the data)
      setMororsValueBySensors(distance1, distance2);
    }
  }
  else {
    Serial.println("Error: Firebase connection error");
  }
  delay(150); //The state sample delay     
}
