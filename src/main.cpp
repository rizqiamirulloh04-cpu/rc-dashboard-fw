#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    pinMode(15, OUTPUT); // backlight
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(2);

    tft.drawString("ESP32-C6 OK", 20, 40);
    tft.drawString("Waveshare LCD", 20, 80);
}

void loop() {
}