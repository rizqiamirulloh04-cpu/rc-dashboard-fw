#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL 15

Arduino_DataBus *bus = new Arduino_HWSPI(
    15 /* DC */,
    14 /* CS */,
    7  /* SCK */,
    6  /* MOSI */,
    GFX_NOT_DEFINED /* MISO */
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21 /* RST */,
    1  /* rotation */,
    true /* IPS */,
    172 /* width */,
    320 /* height */
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->fillScreen(BLACK);

    gfx->setTextColor(CYAN);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 80);
    gfx->println("LCD OK");
}

void loop()
{
}