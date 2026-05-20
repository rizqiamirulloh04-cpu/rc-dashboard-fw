#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
#include <Fonts/FreeSansBold24pt7b.h>

LGFX tft;

void setup()
{
    tft.init();

    // coba 1 atau 3 kalau posisi terbalik
    tft.setRotation(1);

    tft.setBrightness(255);

    tft.fillScreen(TFT_BLACK);
}

void loop()
{
    static int speed = 0;

    int w = tft.width();
    int h = tft.height();

    // background
    tft.fillScreen(TFT_DARKBLUE);

    // ===== HEADER =====
    tft.fillRoundRect(0, 0, w, 34, 8, TFT_BLUE);

    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextSize(1);

    tft.drawCentreString("RC DASHBOARD", w / 2, 10, 2);

    // ===== SPEED =====
    String spd = String(speed);

    tft.setFreeFont(&FreeSansBold24pt7b);
    tft.setTextColor(TFT_CYAN, TFT_DARKBLUE);

    tft.drawCentreString(spd, w / 2, 70, 1);

    // ===== KM/H =====
    tft.setFreeFont(NULL);

    tft.setTextColor(TFT_WHITE, TFT_DARKBLUE);
    tft.setTextSize(2);

    tft.drawCentreString("KM/H", w / 2, 120, 2);

    // ===== PROGRESS BAR =====
    int barWidth = map(speed, 0, 120, 0, w - 40);

    tft.drawRoundRect(
        20,
        h - 30,
        w - 40,
        16,
        8,
        TFT_WHITE
    );

    tft.fillRoundRect(
        20,
        h - 30,
        barWidth,
        16,
        8,
        TFT_GREEN
    );

    // ===== ANIMASI =====
    speed++;

    if(speed > 120)
    {
        speed = 0;
    }

    delay(50);
}