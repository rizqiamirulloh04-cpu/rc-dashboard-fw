#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL 15

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F

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
    35,
    0
);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->setRotation(1);
    gfx->invertDisplay(true);

    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(3);

    gfx->setCursor(20, 40);
    gfx->println("WAVESHARE");

    gfx->setTextColor(GREEN);

    gfx->setCursor(20, 90);
    gfx->println("ESP32-C6");

    gfx->setTextColor(BLUE);

    gfx->setCursor(20, 140);
    gfx->println("LCD OK");
}

void loop()
{
}