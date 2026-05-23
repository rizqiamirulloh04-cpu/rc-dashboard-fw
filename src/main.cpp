#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL 8

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    4,   // DC
    5,   // CS
    6,   // SCK
    7,   // MOSI
    -1
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    0,
    true,
    172,
    320,
    34,
    0,
    0,
    0
);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->setRotation(1);

    gfx->fillScreen(0xF800);
    delay(1000);

    gfx->fillScreen(0x07E0);
    delay(1000);

    gfx->fillScreen(0x001F);
    delay(1000);

    gfx->fillScreen(0x0000);

    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);

    gfx->setCursor(20, 100);
    gfx->println("LCD OK");
}

void loop()
{
}