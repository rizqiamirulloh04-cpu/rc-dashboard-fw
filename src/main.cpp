#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

int speedValue = 0;
int throttleValue = 0;
int steeringValue = 0;

void drawGauge(int speed)
{
    int centerX = 160;
    int centerY = 120;
    int radius = 95;

    tft.drawCircle(centerX, centerY, radius, TFT_WHITE);

    for (int i = -120; i <= 120; i += 10)
    {
        float angle = radians(i);

        int x1 = centerX + cos(angle) * (radius - 10);
        int y1 = centerY + sin(angle) * (radius - 10);

        int x2 = centerX + cos(angle) * radius;
        int y2 = centerY + sin(angle) * radius;

        tft.drawLine(x1, y1, x2, y2, TFT_WHITE);
    }

    float needle = map(speed, 0, 240, -120, 120);
    float angle = radians(needle);

    int x = centerX + cos(angle) * (radius - 20);
    int y = centerY + sin(angle) * (radius - 20);

    tft.drawLine(centerX, centerY, x, y, TFT_RED);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);

    tft.drawString(String(speed), centerX, centerY - 10, 7);
    tft.drawString("KM/H", centerX, centerY + 40, 4);
}

void drawThrottle(int value)
{
    int barX = 20;
    int barY = 60;
    int barW = 30;
    int barH = 140;

    tft.drawRect(barX, barY, barW, barH, TFT_WHITE);

    int fill = map(value, 0, 100, 0, barH);

    tft.fillRect(barX + 2,
                 barY + barH - fill,
                 barW - 4,
                 fill,
                 TFT_GREEN);

    tft.setCursor(10, 20);
    tft.setTextSize(2);
    tft.print("THR ");
    tft.print(value);
    tft.print("%");
}

void drawSteering(int angle)
{
    int x = 280;
    int y = 120;

    tft.drawCircle(x, y, 40, TFT_WHITE);

    float a = radians(angle);

    int x2 = x + sin(a) * 30;
    int y2 = y - cos(a) * 30;

    tft.drawLine(x, y, x2, y2, TFT_GREEN);

    tft.setCursor(240, 20);
    tft.setTextSize(2);
    tft.print("STR ");
    tft.print(angle);
}

void updateDisplay()
{
    tft.fillScreen(TFT_BLACK);

    drawGauge(speedValue);
    drawThrottle(throttleValue);
    drawSteering(steeringValue);
}

void setup()
{
    Serial.begin(115200);

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    updateDisplay();
}

void loop()
{
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate > 100)
    {
        lastUpdate = millis();

        speedValue += 2;
        if (speedValue > 240)
            speedValue = 0;

        throttleValue += 1;
        if (throttleValue > 100)
            throttleValue = 0;

        steeringValue += 5;
        if (steeringValue > 45)
            steeringValue = -45;

        updateDisplay();
    }
}
