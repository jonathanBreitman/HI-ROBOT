#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <Arduino.h>
//------------------------------WiFi---------------------------
#include <WiFi.h>
// Network Credentials
#define WIFI_SSID "POCO"
#define WIFI_PASSWORD "1234567890"
void connectToWiFi();
//-------------------------------------------------------------

//-------------------------------Firebase----------------------
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Firebase project API Key
#define API_KEY "AIzaSyDRH9-Sf2FocGzuXt_8_nrcY4Gr0_jb440"
// RTDB URLefine the RTDB URL
#define DATABASE_URL "https://hi-robot-firebase-default-rtdb.europe-west1.firebasedatabase.app/" 

// Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

void FirebaseSetup();

void readRealTimeDB_Value_Int(char *param_name, int *output);

void readRealTimeDB_Value_Bool(char *param_name, bool *output);
//-------------------------------------------------------------

#endif
