#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Dashboard RC Start");
}

void loop() {
    Serial.println("ESP32-C6 Running");
    delay(1000);
}