#include <Arduino.h>
#include "LGFX_Config.h"

LGFX tft;

int speed = 0;
int battery = 92;
int rssi = 98;
int temp = 35;

void drawDashboard()
{
    tft.fillScreen(TFT_BLACK);

    // Header
    tft.fillRect(0, 0, 320, 30, TFT_DARKGREEN);

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("RC DASHBOARD", 80, 6);

    // Speed
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(5);
    tft.drawString(String(speed), 105, 45);

    tft.setTextSize(2);
    tft.drawString("KM/H", 125, 105);

    // RSSI
    tft.setTextColor(TFT_GREEN);
    tft.drawString("RSSI", 10, 50);
    tft.drawString(String(rssi) + "%", 10, 75);

    // Battery
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("BAT", 250, 50);
    tft.drawString(String(battery) + "%", 240, 75);

    // Temperature
    tft.setTextColor(TFT_RED);
    tft.drawString("TEMP", 235, 125);
    tft.drawString(String(temp) + "C", 250, 150);

    // RPM Bar
    tft.drawRect(20, 140, 180, 20, TFT_WHITE);

    int bar = map(speed, 0, 120, 0, 176);

    tft.fillRect(22, 142, bar, 16, TFT_GREEN);

    // Link Status
    tft.setTextColor(TFT_GREEN);
    tft.drawString("LINK OK", 10, 10);
}

void setup()
{
    tft.init();

    tft.setRotation(1);

    drawDashboard();
}

void loop()
{
    speed++;

    if (speed > 120)
        speed = 0;

    drawDashboard();

    delay(100);
}