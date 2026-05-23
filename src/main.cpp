#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// ======================
// Pin Waveshare ESP32-C6 LCD 1.47
// ======================

#define TFT_BL   15

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_DC   8
#define TFT_RST  9
#define TFT_CS   14

// ======================
// Warna RGB565
// ======================

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define YELLOW  0xFFE0

// ======================
// LCD Driver
// ======================

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
    320
);

void setup()
{
    Serial.begin(115200);

    // Backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Init Display
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

    // Tampilan utama
    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(3);
    gfx->setCursor(20, 40);
    gfx->println("ESP32-C6");

    gfx->setTextColor(YELLOW);
    gfx->setTextSize(2);
    gfx->setCursor(20, 100);
    gfx->println("Waveshare LCD");

    gfx->setTextColor(GREEN);
    gfx->setCursor(20, 140);
    gfx->println("BUILD SUCCESS");
}

void loop()
{
}