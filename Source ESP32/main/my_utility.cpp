#include "my_utility.h"

// Network Credentials
#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "dreamy128"

int robotMode = MANUAL;
int cornerNumber = 4;
int currCornerNumber = 0; //  0 - (corrner_number-1)
time_t lastChargeTime;
time_t currentTime;
time_t startChargeTime;

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
