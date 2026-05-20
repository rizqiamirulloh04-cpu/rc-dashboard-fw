#include <Arduino.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;

public:

    LGFX(void)
    {
        {
            auto cfg = _bus.config();

            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 16000000;

            cfg.pin_sclk = 6;
            cfg.pin_mosi = 7;
            cfg.pin_miso = -1;
            cfg.pin_dc   = 15;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _panel.config();

            cfg.pin_cs           = 14;
            cfg.pin_rst          = 21;
            cfg.pin_busy         = -1;

            cfg.memory_width     = 172;
            cfg.memory_height    = 320;

            cfg.panel_width      = 172;
            cfg.panel_height     = 320;

            cfg.offset_x         = 34;
            cfg.offset_y         = 0;
            cfg.offset_rotation  = 0;

            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;

            cfg.readable         = false;
            cfg.invert           = true;
            cfg.rgb_order        = false;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       = true;

            _panel.config(cfg);
        }

        {
            auto cfg = _light.config();

            cfg.pin_bl = 22;
            cfg.invert = false;
            cfg.freq   = 44100;
            cfg.pwm_channel = 7;

            _light.config(cfg);
            _panel.setLight(&_light);
        }

        setPanel(&_panel);
    }
};

LGFX tft;

void drawBar(int value)
{
    int barWidth = map(value, 0, 120, 0, 240);

    tft.drawRoundRect(40, 110, 240, 20, 10, TFT_WHITE);
    tft.fillRoundRect(40, 110, barWidth, 20, 10, TFT_GREEN);
}

void setup()
{
    tft.init();

    tft.setRotation(1);

    tft.setBrightness(255);

    tft.fillScreen(TFT_BLACK);
}

void loop()
{
    static int speed = 0;

    tft.fillScreen(TFT_BLACK);

    // Header
    tft.fillRoundRect(0, 0, 320, 35, 0, TFT_DARKCYAN);

    tft.setTextColor(TFT_WHITE, TFT_DARKCYAN);
    tft.setTextSize(2);

    tft.drawCentreString("RC DASHBOARD", 160, 10, 2);

    // Speed angka besar
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(6);

    tft.drawCentreString(String(speed), 120, 45, 4);

    // KM/H
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    tft.drawString("KM/H", 220, 80, 2);

    // Speed bar
    drawBar(speed);

    // Status
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawCentreString("READY", 160, 145, 2);

    speed++;

    if(speed > 120)
    {
        speed = 0;
    }

    delay(40);
}