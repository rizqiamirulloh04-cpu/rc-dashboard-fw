#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL   22

#define TFT_SCLK 6
#define TFT_MOSI 7
#define TFT_DC   15
#define TFT_CS   14
#define TFT_RST  21

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    -1);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,
    true,
    172,
    320);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

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

    gfx->fillScreen(BLACK);

    gfx->setCursor(20, 120);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(2);
    gfx->println("ESP32-C6 OK");
}

void loop()
{
}