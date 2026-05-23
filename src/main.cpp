#include <Arduino.h>
#include "LGFX_Config.h"

LGFX tft;

void setup()
{
    tft.init();

    tft.setRotation(1);

    tft.fillScreen(TFT_RED);
    delay(1000);

    tft.fillScreen(TFT_GREEN);
    delay(1000);

    tft.fillScreen(TFT_BLUE);
    delay(1000);

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE);

    tft.setTextSize(2);

    tft.drawString("LCD OK", 30, 100);
}

void loop()
{
}