#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define VIN_PIN 35
#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "dreamy128"

AsyncWebServer server(80);

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}

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

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Starting Setup");
  pinMode(VIN_PIN, INPUT);
  connectToWiFi(); 
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("Finished Setup");
  WebSerial.println("Finished Setup");
}

void loop() {
  Serial.printf("power read: %d\n", analogRead(VIN_PIN));
  WebSerial.print("power read: ");
  WebSerial.println(analogRead(VIN_PIN));
  delay(5000);
}
