#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(9600); 
  while (!Serial) { }
  delay(1000);
  Serial.println("Starting");  
  for (int i = 0; i < 5; i++) {
    Serial.println("Index: " + String(i));
    delay(1000);
  }
  Serial.println("Enter deep sleep for 5 seconds...");
  ESP.deepSleep(5e6);
}

void loop() { 
}
