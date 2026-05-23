#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 15

// Waveshare ESP32-C6 LCD 1.47
#define TFT_DC   8
#define TFT_CS   14
#define TFT_SCLK 7
#define TFT_MOSI 6
#define TFT_RST  9

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    GFX_NOT_DEFINED
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
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

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    gfx->begin();

    gfx->setRotation(1);

    gfx->fillScreen(BLACK);
    delay(1000);

    gfx->fillScreen(RED);
    delay(1000);

    gfx->fillScreen(GREEN);
    delay(1000);

    gfx->fillScreen(BLUE);
    delay(1000);

    gfx->fillScreen(WHITE);
}

void loop()
{
}