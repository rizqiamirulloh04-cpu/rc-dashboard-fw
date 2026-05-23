#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 22

// SPI LCD
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    15, // DC
    14, // CS
    6,  // SCK
    7,  // MOSI
    -1  // MISO
);

// Driver LCD
Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    0,    // rotation
    true, // IPS
    172,  // width
    320,  // height
    34,   // col offset
    0,    // row offset
    0,
    0
);

void setup()
{
    Serial.begin(115200);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    gfx->begin(40000000);

    gfx->setRotation(1);

    // test
    gfx->fillScreen(0xF800);
    delay(1000);

    gfx->fillScreen(0x07E0);
    delay(1000);

    gfx->fillScreen(0x001F);
    delay(1000);

    gfx->fillScreen(0x0000);

    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);

    gfx->setCursor(20, 80);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 130);
    gfx->println("WAVESHARE");

    gfx->setCursor(20, 180);
    gfx->println("LCD TEST");
}

void loop()
{
}