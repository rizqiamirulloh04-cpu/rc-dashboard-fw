#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// Waveshare ESP32-C6 LCD 1.47
#define TFT_BL 15
#define TFT_CS 14
#define TFT_DC 7
#define TFT_RST 21
#define TFT_SCK 6
#define TFT_MOSI 5

Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,
  TFT_CS,
  TFT_SCK,
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
  34,
  0
);

int speedValue = 0;
int throttle = 0;

void drawUI()
{
  gfx->fillScreen(BLACK);

  gfx->setTextColor(CYAN);
  gfx->setTextSize(3);
  gfx->setCursor(40, 20);
  gfx->println("RC DASH");

  gfx->drawRect(20, 80, 280, 30, WHITE);
  gfx->fillRect(22, 82, speedValue * 2, 26, GREEN);

  gfx->setCursor(20, 130);
  gfx->setTextColor(YELLOW);
  gfx->print("Speed: ");
  gfx->print(speedValue);

  gfx->drawRect(20, 180, 280, 30, WHITE);
  gfx->fillRect(22, 182, throttle * 2, 26, RED);

  gfx->setCursor(20, 230);
  gfx->setTextColor(WHITE);
  gfx->print("Throttle: ");
  gfx->print(throttle);
}

void setup()
{
  Serial.begin(115200);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  gfx->begin();
  gfx->fillScreen(BLACK);

  drawUI();
}

void loop()
{
  speedValue++;
  throttle++;

  if(speedValue > 100) speedValue = 0;
  if(throttle > 100) throttle = 0;

  drawUI();

  delay(100);
}