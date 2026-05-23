#include <Arduino.h>
#include <SPI.h>

#define LCD_SCK  6
#define LCD_MOSI 7
#define LCD_CS   14
#define LCD_DC   15
#define LCD_RST  21
#define LCD_BL   22

SPIClass spi(FSPI);

void writeCmd(uint8_t cmd)
{
    digitalWrite(LCD_DC, LOW);
    digitalWrite(LCD_CS, LOW);
    spi.transfer(cmd);
    digitalWrite(LCD_CS, HIGH);
}

void writeData(uint8_t data)
{
    digitalWrite(LCD_DC, HIGH);
    digitalWrite(LCD_CS, LOW);
    spi.transfer(data);
    digitalWrite(LCD_CS, HIGH);
}

void lcdInit()
{
    digitalWrite(LCD_RST, LOW);
    delay(100);

    digitalWrite(LCD_RST, HIGH);
    delay(100);

    writeCmd(0x11);
    delay(120);

    writeCmd(0x36);
    writeData(0x00);

    writeCmd(0x3A);
    writeData(0x05);

    writeCmd(0x21);

    writeCmd(0x29);
    delay(20);
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    writeCmd(0x2A);

    writeData(x0 >> 8);
    writeData(x0 & 0xFF);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);

    writeCmd(0x2B);

    writeData(y0 >> 8);
    writeData(y0 & 0xFF);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);

    writeCmd(0x2C);
}

void fillColor(uint16_t color)
{
    setAddrWindow(0, 0, 171, 319);

    digitalWrite(LCD_DC, HIGH);
    digitalWrite(LCD_CS, LOW);

    for (uint32_t i = 0; i < 172UL * 320UL; i++)
    {
        spi.transfer(color >> 8);
        spi.transfer(color & 0xFF);
    }

    digitalWrite(LCD_CS, HIGH);
}

void setup()
{
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_RST, OUTPUT);
    pinMode(LCD_BL, OUTPUT);

    digitalWrite(LCD_BL, HIGH);

    spi.begin(LCD_SCK, -1, LCD_MOSI, LCD_CS);

    lcdInit();

    fillColor(0xF800);
    delay(1000);

    fillColor(0x07E0);
    delay(1000);

    fillColor(0x001F);
    delay(1000);

    fillColor(0xFFFF);
    delay(1000);

    fillColor(0x0000);
}

void loop()
{
}