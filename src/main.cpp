#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Pin Waveshare ESP32-C6 LCD 1.47
#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21
#define TFT_BL   22

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

    tft.init(172, 320);

    // Coba orientasi berbeda
    tft.setRotation(1);

    // Inversi warna kadang wajib di board waveshare
    tft.invertDisplay(true);

    // Test layar
    tft.fillScreen(ST77XX_RED);
    delay(1000);

    tft.fillScreen(ST77XX_GREEN);
    delay(1000);

    tft.fillScreen(ST77XX_BLUE);
    delay(1000);

    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(20, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(3);
    tft.println("ESP32-C6");

    tft.setCursor(20, 130);
    tft.setTextSize(2);
    tft.println("LCD OK");
}

void loop() {
}