#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 15

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    8,   // DC
    14,  // CS
    6,   // SCK
    7,   // MOSI
    -1   // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    0,    // rotation
    true, // IPS
    172,  // width
    320,  // height
    34,   // col offset
    0,    // row offset
    35    // col offset2
);

void setup()
{
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    gfx->begin();

    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 80);
    gfx->println("LCD TEST");

    gfx->drawRect(10, 10, 150, 100, RED);
}

void loop()
{
}