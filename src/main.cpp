#include <Arduino.h>

int throttle = 0;
int steering = 0;

void drawDashboard() {
    Serial.print("Throttle: ");
    Serial.print(throttle);

    Serial.print(" | Steering: ");
    Serial.println(steering);
}

void setup() {
    Serial.begin(115200);

    Serial.println();
    Serial.println("RC Dashboard Starting...");
}

void loop() {

    throttle += 5;
    steering += 3;

    if (throttle > 100) throttle = 0;
    if (steering > 100) steering = 0;

    drawDashboard();

    delay(200);
}