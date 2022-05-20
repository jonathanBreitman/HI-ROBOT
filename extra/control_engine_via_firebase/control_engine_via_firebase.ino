#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAWjSDHTgJVYfuhwAVy9StRqDm_eqc0lwU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hi-robot-c64d7-default-rtdb.firebaseio.com/" 

#define WIFI_SSID "POCO"
#define WIFI_PASSWORD "1234567890"

//movment kind
#define AUTONOMOUS 1
#define MANUAL 0

int movment_type = 0;

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

//L298 PINS
const int motorA1  = 33;  // IN3 Input of L298N
const int motorA2  = 32;  // IN1 Input of L298N
const int motorB1  = 25; //  IN2 Input of L298N
const int motorB2  = 26;  // IN4 Input of L298N

int vSpeed = 255;   // Standard Speed can take a value between 0-255
bool back = false;
bool right = false;
bool left = false;
bool go = false;

void setup() {
   // Let's set our pins
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("FINISHED SETUP");
}

void readAndSetSpeed(){
      if (Firebase.RTDB.get(&fbdo, "wirelessCar/speed")){
//        Serial.println("READ speed SUCCESS");
//        Serial.println("PATH: " + fbdo.dataPath());
//        Serial.println("TYPE: " + fbdo.dataType());
//        Serial.print("VALUE : ");
//        Serial.println(fbdo.to<int>());
        vSpeed = fbdo.to<int>(); 
      }
      else {
        Serial.println("FAILED reading");
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}

void read_movment_state(){
  if (Firebase.RTDB.get(&fbdo, "wirelessCar/state")){
//        Serial.println("READ speed SUCCESS");
//        Serial.println("PATH: " + fbdo.dataPath());
//        Serial.println("TYPE: " + fbdo.dataType());
//        Serial.print("VALUE : ");
//        Serial.println(fbdo.to<int>());
        movment_type = fbdo.to<int>(); 
      }
      else {
        Serial.println("FAILED reading");
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}

void readAndSetDirection(char *param_name, bool *output){
      char dst[150] = "wirelessCar/";
      if (Firebase.RTDB.getBool(&fbdo, strcat(dst, param_name))){
//        Serial.println("READ direction SUCCESS");
//        Serial.println("PATH: " + fbdo.dataPath());
//        Serial.println("TYPE: " + fbdo.dataType());
//        Serial.print("VALUE : ");
//        Serial.println(fbdo.to<bool>());
        *output = fbdo.to<bool>(); 
      }
      else {
        Serial.println("FAILED reading");
        Serial.println("REASON: " + fbdo.errorReason());
      }  
}

void manual_movment(){
  if (Firebase.ready() && signupOK){
      readAndSetSpeed();
      readAndSetDirection("go", &go);
      readAndSetDirection("back", &back);
      readAndSetDirection("left", &left);
      readAndSetDirection("right", &right);
      if (go && !left && !right){
        //go forward
        digitalWrite(motorA1, vSpeed);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, vSpeed);
        digitalWrite(motorB2, 0);
      } else if (left && !go && !back) {
        //go left
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, vSpeed);
        digitalWrite(motorB1, vSpeed);
        digitalWrite(motorB2, 0);
      } else if (right && !go && !back) {
        //go right
        digitalWrite(motorA1, vSpeed);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, vSpeed);
      } else if (back && !left && !right) {
        //go right
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, vSpeed);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, vSpeed);
      } else if (!go && !back && !left && !right){
        Serial.println("Stopping");
        digitalWrite(motorA1, 0);
        digitalWrite(motorA2, 0);
        digitalWrite(motorB1, 0);
        digitalWrite(motorB2, 0);
      }
  }
}

void loop() {
  read_movment_state();
  if( movment_type == MANUAL){
    manual_movment();
  } else {
    //algorithm with sensors
  }
  delay(1000);
}
