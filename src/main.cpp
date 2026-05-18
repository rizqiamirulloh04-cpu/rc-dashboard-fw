#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    pinMode(22, OUTPUT);
    digitalWrite(22, HIGH);

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);

    tft.drawString("RC DASHBOARD", 40, 40);
    tft.drawString("ESP32-C6", 70, 80);

    tft.drawRect(20, 140, 280, 30, TFT_WHITE);
    tft.fillRect(20, 140, 180, 30, TFT_GREEN);

    tft.drawString("Throttle", 90, 180);
}

void loop()
{
}