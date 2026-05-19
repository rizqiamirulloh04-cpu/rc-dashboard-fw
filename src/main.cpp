#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREEN 0x07E0

// SPI bus
Arduino_DataBus *bus = new Arduino_ESP32SPI(
15, // DC
14, // CS
7,  // SCK
6,  // MOSI
-1  // MISO
);

// Display ST7789
Arduino_GFX *gfx = new Arduino_ST7789(
bus,
21,   // RST
1,    // rotation
true, // IPS
172,  // width
320   // height
);

void setup()
{
pinMode(22, OUTPUT);
digitalWrite(22, HIGH);

gfx->begin();  

gfx->fillScreen(BLACK);  

gfx->setTextColor(GREEN);  
gfx->setTextSize(2);  

gfx->setCursor(25, 40);  
gfx->println("RC DASHBOARD");  

gfx->setCursor(50, 80);  
gfx->println("ESP32-C6");  

// Bar outline  
gfx->drawRect(20, 140, 280, 30, WHITE);  

// Fill bar  
gfx->fillRect(20, 140, 180, 30, GREEN);  

gfx->setCursor(90, 190);  
gfx->println("Throttle");

}

void loop()
{
}