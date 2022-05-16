#include "motors.h"

#define MANUAL 0
#define AUTONOMOUS 1
int robotMode = MANUAL;

void setup() {
  Serial.begin(115200);
  Serial.println("**STARTING SETUP**");
  
  // Setting up pins
  setupMotorPins();
  // Connect to WiFi
  connectToWiFi(); 
  // Connect to Firebase 
  FirebaseSetup();
  
  Serial.println("**FINISHED SETUP**");
}

void loop() {
  if (Firebase.ready() && signupOK) {
    readRealTimeDB_Value_Int("state", &robotMode); //read robot  state
    if (robotMode == MANUAL) {
      readMotorsDB_Commends();
      setMotorsValueByCommend();
    }
    //else if (robotMode == AUTONOMOUS) {  
    //sample distance sensors
    //initinalize motors accordingly to the sensors (correction)
    //delay(150); //The autonomus correction extra delay     
    //}
  }
  else {
    Serial.println("Error: Firebase connection error");
  }
  delay(150); //The manual sample delay     
}
