#include "my_utility.h"
#include "cam.h"

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>

// Firebase project API Key
#define API_KEY "AIzaSyDRH9-Sf2FocGzuXt_8_nrcY4Gr0_jb440"
// Authorized Email and Corresponding Password
#define USER_EMAIL "itamar128@gmail.com"
#define USER_PASSWORD "Dreamy1208"
// Firebase storage bucket ID e.g bucket-name.appspot.com
#define STORAGE_BUCKET_ID "hi-robot-firebase.appspot.com"
// RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hi-robot-c64d7-default-rtdb.firebaseio.com/" 

#define FILE_PATH "wirelessCar/"

// Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
bool signupOK = false;

//-------------------------------Firebase--------------------------------------
void FirebaseSetup() {
  
  /* Assign the api key (required) */
  configF.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  configF.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&configF, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.println("Error: sign up to Firebase");
    //Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  configF.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);  
}


void FirebaseSetup_v2() {  
  // Assign the api key
  configF.api_key = API_KEY;
  //Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  //Assign the callback function for the long running token generation task
  configF.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&configF, &auth);
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

void updateRealTimeDB_ValueInt(const char *param_name, int value) {
  if (Firebase.RTDB.setInt(&fbdo, param_name, value)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}
//-----------------------------------------------------------------------------
//-------------------------------CAM-Upload-Photos-----------------------------
void capturePhotoAnUpload() {
  int photo_number;
  readRealTimeDB_ValueInt(PHOTO_NUMBER, &photo_number);
  char photo_name[100] = {0};
  strcat(photo_name, FILE_PHOTO);
  strcat(photo_name, "/photo");
  char sphoto_number[15];
  itoa(photo_number, sphoto_number, 10);
  strcat(photo_name, sphoto_number);
  strcat(photo_name, ".jpg");
  
  //update photo_number in real-time database FILE_PHOTO
  photo_number++;
  updateRealTimeDB_ValueInt(PHOTO_NUMBER, photo_number);

  capturePhotoSaveSpiffs();
  Serial.print("Uploading picture... ");
  //MIME type should be valid to avoid the download problem.
  //The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
  if (Firebase.Storage.upload(&fbdo, 
      STORAGE_BUCKET_ID /* Firebase Storage bucket id */, 
      "wirelessCar/image.jpg" /* path to local file */, 
      mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, 
      photo_name /* path of remote file stored in the bucket */, 
      "image/jpeg" /* mime type */)) {
    Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
  }
  else {
    Serial.println(fbdo.errorReason());
  }
}
//-----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200); // Serial port for debugging purposes
  Serial.println("**STARTING ESP-CAM SETUP**");
  
  // Connect to WiFi
  connectToWiFi();
  initSPIFFS();
  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();
  // Connect to Firebase 
  FirebaseSetup();
  
  Serial.println("**FINISHED ESP-CAM SETUP**");
}


void loop() {
  if (Firebase.ready() && signupOK) {
    capturePhotoAnUpload();
    delay(TAKE_PHOTO);
  }
  else {
    Serial.println("Error: Firebase connection error");
  }
}
