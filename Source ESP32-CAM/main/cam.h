/*********
  Complete instructions about uploading photos to Firebase 
  at: https://RandomNerdTutorials.com/esp32-cam-save-picture-firebase-storage/
*********/

#ifndef CAM_H
#define CAM_H

#include "esp_camera.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <FS.h>

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define TAKE_PHOTO 1000*60*10  // Take a photo each 10 minutes 
// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/wirelessCarFootage"
#define PHOTO_NUMBER "wirelessCar/photo_number"

void initSPIFFS();
void initCamera();
bool checkPhoto( fs::FS &fs );
void capturePhotoSaveSpiffs();
void capturePhotoAnUpload();


#endif
