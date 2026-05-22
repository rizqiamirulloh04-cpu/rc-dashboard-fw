#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>

// ======================
// Waveshare ESP32-C6 LCD 1.47
// ======================

// Pin SPI
#define TFT_SCLK 6
#define TFT_MOSI 7
#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21
#define TFT_BL   22

// Warna RGB565
#define BLACK 0x0000
#define BLUE  0x001F
#define RED   0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    -1
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,
    true,
    172,
    320
);

void setup()
{
    Serial.begin(115200);

    // Backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    // Rotation
    gfx->setRotation(1);

    // Test warna
    gfx->fillScreen(BLACK);
    delay(1000);

    gfx->fillScreen(RED);
    delay(1000);

    gfx->fillScreen(GREEN);
    delay(1000);

    gfx->fillScreen(BLUE);
    delay(1000);

    // Teks
    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(3);

    gfx->setCursor(20, 80);
    gfx->println("WAVESHARE");

    gfx->setCursor(20, 130);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 180);
    gfx->println("LCD OK!");
}

void loop()
{
}