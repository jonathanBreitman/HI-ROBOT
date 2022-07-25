#include "my_utility.h"

// Network Credentials
//#define WIFI_SSID "IOT"
//#define WIFI_PASSWORD "dreamy128"

#define WIFI_SSID "ICST"
#define WIFI_PASSWORD "arduino123"


void connectToWiFi() {
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
