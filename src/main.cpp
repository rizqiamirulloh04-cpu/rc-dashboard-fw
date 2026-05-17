#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    Serial.println("RC Dashboard Start");
}

void loop() {
    static int speed = 0;

    Serial.print("Speed: ");
    Serial.println(speed);

    speed++;

    if(speed > 200) {
        speed = 0;
    }

    delay(100);
}