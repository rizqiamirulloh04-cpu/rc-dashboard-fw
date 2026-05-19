#ifndef WAVESHARE_LCD_H
#define WAVESHARE_LCD_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#define LCD_WIDTH 172
#define LCD_HEIGHT 320

#define LCD_BL 22

TFT_eSPI tft = TFT_eSPI();

void lcd_init()
{
    pinMode(LCD_BL, OUTPUT);

    digitalWrite(LCD_BL, HIGH);

    tft.begin();

    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);
}

void lcd_brightness(uint8_t brightness)
{
    ledcAttachPin(LCD_BL, 0);

    ledcSetup(0, 5000, 8);

    ledcWrite(0, brightness);
}

#endif