#include <Arduino.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX() {
    {
      auto cfg = _bus.config();

      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;

      cfg.pin_sclk = 6;
      cfg.pin_mosi = 7;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 14;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();

      cfg.pin_cs           = 15;
      cfg.pin_rst          = 21;
      cfg.pin_busy         = -1;

      cfg.panel_width      = 172;
      cfg.panel_height     = 320;

      cfg.offset_x         = 34;
      cfg.offset_y         = 0;

      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;

      cfg.readable         = false;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       = true;

      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};

LGFX tft;

void drawDashboard(int speed)
{
  tft.fillScreen(TFT_DARKBLUE);

  tft.fillRect(0, 0, 320, 80, TFT_BLUE);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);

  tft.setCursor(80, 20);
  tft.printf("%d KM/H", speed);

  tft.drawCircle(160, 220, 80, TFT_CYAN);
  tft.drawCircle(160, 220, 81, TFT_CYAN);
  tft.drawCircle(160, 220, 82, TFT_CYAN);

  int angle = map(speed, 0, 100, -140, -40);

  float rad = angle * 0.0174533;

  int x = 160 + cos(rad) * 60;
  int y = 220 + sin(rad) * 60;

  tft.drawLine(160, 220, x, y, TFT_RED);
  tft.fillCircle(160, 220, 8, TFT_WHITE);
}

void setup()
{
  tft.init();

  tft.setRotation(1);

  drawDashboard(0);
}

void loop()
{
  static int speed = 0;
  static int dir = 1;

  drawDashboard(speed);

  speed += dir;

  if(speed >= 100) dir = -1;
  if(speed <= 0) dir = 1;

  delay(30);
}