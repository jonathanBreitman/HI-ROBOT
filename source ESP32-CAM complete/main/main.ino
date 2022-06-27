//important, in order to avoid failed compilation goto Tools->Board and make sure the selected board is "AI-Thinker ESP32-CAM"
#include "my_utility.h"
#include "cam.h"

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>

// Firebase project API Key
#define API_KEY "AIzaSyAWjSDHTgJVYfuhwAVy9StRqDm_eqc0lwU"
// Authorized Email and Corresponding Password
#define USER_EMAIL "itamar128@gmail.com"
#define USER_PASSWORD "Dreamy1208"
// Firebase storage bucket ID e.g bucket-name.appspot.com
#define STORAGE_BUCKET_ID "hi-robot-c64d7.appspot.com"
// RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hi-robot-c64d7-default-rtdb.firebaseio.com/" 

#define FILE_PATH "wirelessCar/"

// Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
bool signupOK = false;
int delay_on_loop = 0;
int snap_pic = 0;

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
        //Serial.printf("READ: %s succesfully!\n", param_name);
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
  char start_str[150] = FILE_PATH;
  if (Firebase.RTDB.setInt(&fbdo, strcat(start_str, param_name), value)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void updateRealTimeDB_ValueString(const char *param_name, const char* value){
  char start_str[150] = FILE_PATH;
  if (Firebase.RTDB.setString(&fbdo, strcat(start_str, param_name), value)){
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  } else {
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
  Serial.println("updating photo_number");
  updateRealTimeDB_ValueInt(PHOTO_NUMBER, photo_number);
  Serial.printf("Saving picture, picture name is: %s\n", photo_name);
  capturePhotoSaveSpiffs();
  Serial.print("Uploading picture... ");
  //MIME type should be valid to avoid the download problem.
  //The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
  if (Firebase.Storage.upload(&fbdo, 
      STORAGE_BUCKET_ID /* Firebase Storage bucket id */, 
      FILE_PHOTO /* path to local file */, 
      mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, 
      photo_name /* path of remote file stored in the bucket */, 
      "image/jpeg" /* mime type */)) {
    Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
  }
  else {
    Serial.println(fbdo.errorReason());
  }
}

void setUpDelay(){
  int delay_in_minutes;
  Serial.println("reading snap interval");
  readRealTimeDB_ValueInt("snap_interval", &delay_in_minutes);
  delay_on_loop = delay_in_minutes * 60;
}
//-----------------------------------------------------------------------------

void setup() {
  delay(10000);
  Serial.begin(115200); // Serial port for debugging purposes
  Serial.println("**STARTING ESP-CAM SETUP**");
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  // Connect to WiFi
  connectToWiFi();
  initSPIFFS();
  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();
  //print video
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  startCameraServer();
  
  // Connect to Firebase 
  while(!signupOK){
    FirebaseSetup();
  }

  Serial.println("updating feed_url");
  updateRealTimeDB_ValueString("feed_url", WiFi.localIP().toString().c_str());

  
  Serial.println("**FINISHED ESP-CAM SETUP**");
}


void loop() {
  setUpDelay();
  for(int i = 0; i < delay_on_loop; i++){
    readRealTimeDB_ValueInt("snap_pic", &snap_pic);
    if(snap_pic == 1){
      Serial.println("client requested snapping");
      updateRealTimeDB_ValueInt("snap_pic", 0);
      if (Firebase.ready()) {
        capturePhotoAnUpload();
        i += 20;
      } else {
        Serial.println("Error: Firebase connection error");
      }
    }
    delay(1000);
  }
  Serial.println("scheduled snapping");
  if (Firebase.ready()) {
    capturePhotoAnUpload();
  }
  else {
    Serial.println("Error: Firebase connection error");
  }
}
