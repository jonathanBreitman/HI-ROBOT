#include "my_utility.h"
#include "motors.h"
#include "distance_sensors.h"
#include "charge_detect.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

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

#define MANUAL_DRIVE_DELAY 300
#define NO_FIREBASE_DELAY 150

int lastState;
extern int robotMode;
extern int cornerNumber;
extern int currCornerNumber; //  0 - (corner_number-1)
//time_t rawtime aka long int (%ld) contains the number of seconds since 1970
//time(&rawtime);
extern time_t lastChargeTime;
extern time_t currentTime;
extern time_t startChargeTime;
extern Tb6612fng* motors;
extern int charging_interval;
extern int charging_forward_delay;
extern int charging_time;

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
    WebSerial.println("ok");
    signupOK = true;
  }
  else{
    WebSerial.println("Error: sign up to Firebase");
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
        WebSerial.println("FAILED reading");
        Serial.println("FAILED reading");
        WebSerial.println("REASON: " + fbdo.errorReason());
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
        WebSerial.println("FAILED reading");
        Serial.println("FAILED reading");
        WebSerial.println("REASON: " + fbdo.errorReason());
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}

void updateRealTimeDB_ValueInt(const char *param_name, int value) {
  char start_str[150] = FILE_PATH;
  if (Firebase.RTDB.setInt(&fbdo, strcat(start_str, param_name), value)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    WebSerial.println("FAILED READING INT");
    Serial.println("FAILED READING INT");
    WebSerial.println("REASON: " + fbdo.errorReason());
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
AsyncWebServer server(80);

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}

void setup() {
  delay(5000);
  Serial.begin(115200); // Serial port for debugging purposes
  time(&lastChargeTime);
  Serial.println("**STARTING ESP SETUP**");
  // Connect to WiFi & setupWebSerial
  connectToWiFi(); 
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  // Setting up distance sensors
  setupDistanceSensors();
  //setup motors and charging sensor
  setupMotorPins();
  setupChargeDetection();
  // Connect to Firebase 
  while(!signupOK)
    FirebaseSetup();   
  //set corner number  
  readRealTimeDB_ValueInt("corners_number", &cornerNumber);
  readRealTimeDB_ValueInt("charge_interval", &charging_interval);
  readRealTimeDB_ValueInt("charge_forward_delay", &charging_forward_delay);
  readRealTimeDB_ValueInt("charging_time", &charging_time);
  delay(5000);
  WebSerial.println("**FINISHED ESP SETUP**");
}

void loop() {
  stopEngine();
  if (Firebase.ready()) {
    //WebSerial.println("firebase is ready");
    lastState = robotMode;
    WebSerial.println("read robot state");
    readMotorsDB_Commands();    
    if (robotMode == AUTONOMOUS && lastState == MANUAL){
      currCornerNumber = 0;
    }         
    if (robotMode == MANUAL) {
      setMotorsValueByCommand(MANUAL_DRIVE_DELAY);
    }
    else if (robotMode == AUTONOMOUS) {      
      //WebSerial.println("entering autonomous movement");  
      // Sample distance sensors
      int distanceRightSense = readDistanceRight(); //distance of sensor 1
      int distanceFrontSense = readDistanceFront(); //distance of sensor 2      
      // In case we in corner and needs to charge -> handle it.
      if (chargingHandle(distanceFrontSense)) {
        move_into_charging_position();
        updateRealTimeDB_ValueInt("state", AUTONOMOUS_IN_CHARGE);
        robotMode = AUTONOMOUS_IN_CHARGE;
      }              
      if (robotMode != AUTONOMOUS_IN_CHARGE) { // Initinalize motors accordingly to the sensors (correction of movement according to the data)
        setMotorsValueBySensors(distanceRightSense, distanceFrontSense);
      }       
    }
    else if (robotMode == AUTONOMOUS_IN_CHARGE) {
      time(&currentTime);                                  // Sample current time
      if ((currentTime - startChargeTime)>charging_time){  // Check if we done chargeing 
        lastChargeTime = currentTime;                      // Update last charge time             
        WebSerial.println("Go Backward exit charging station");
        motors->drive(-1.0, -1.0, charging_forward_delay); 
        void turn_90_degree_left();
        updateRealTimeDB_ValueInt("state", AUTONOMOUS);
        robotMode = AUTONOMOUS;
      }
    }      
  }
  else {
    WebSerial.println("Error: Firebase connection error");
    delay(NO_FIREBASE_DELAY); // The state sample delay
  }
}
