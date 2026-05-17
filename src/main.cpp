#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int speedValue = 0;
bool up = true;

void drawDashboard() {
    tft.fillScreen(TFT_BLACK);

    // Judul
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("RC DASHBOARD", 20, 10);

    // Speed bar background
    tft.drawRect(20, 60, 200, 25, TFT_WHITE);

    // Speed fill
    tft.fillRect(22, 62, speedValue * 2 - 4, 21, TFT_GREEN);

    // Speed text
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(3);
    tft.drawString(String(speedValue) + "%", 80, 110);

    // Steering animation
    int steeringX = map(speedValue, 0, 100, 40, 200);
    tft.fillCircle(steeringX, 180, 12, TFT_RED);

    // Label
    tft.setTextSize(2);
    tft.drawString("STEERING", 60, 210);
}

void setup() {
    Serial.begin(115200);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void loop() {

    drawDashboard();

    if (up) {
        speedValue++;
        if (speedValue >= 100) up = false;
    } else {
        speedValue--;
        if (speedValue <= 0) up = true;
    }

    delay(30);
}