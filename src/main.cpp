#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 15

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    6,   // DC
    7,   // CS
    5,   // SCK
    4,   // MOSI
    GFX_NOT_DEFINED
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    3,    // RST
    3,    // rotation
    true, // IPS
    172,
    320,
    34,
    0,
    0,
    0
);

void setup()
{
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    Serial.begin(115200);

    delay(200);

    gfx->begin();

    gfx->fillScreen(0x0000);

    gfx->setCursor(20, 40);
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(2);
    gfx->println("Waveshare ESP32-C6");

    gfx->setCursor(20, 80);
    gfx->println("LCD OK");
}

void loop()
{
}