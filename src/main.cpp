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
            cfg.freq_read  = 16000000;

            cfg.spi_3wire = false;
            cfg.use_lock  = true;

            cfg.dma_channel = SPI_DMA_CH_AUTO;

            cfg.pin_sclk = 7;
            cfg.pin_mosi = 6;
            cfg.pin_miso = -1;
            cfg.pin_dc   = 15;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _panel.config();

            cfg.pin_cs   = 14;
            cfg.pin_rst  = 21;
            cfg.pin_busy = -1;

            cfg.memory_width  = 172;
            cfg.memory_height = 320;

            cfg.panel_width  = 172;
            cfg.panel_height = 320;

            cfg.offset_x = 34;
            cfg.offset_y = 0;

            cfg.offset_rotation = 0;

            cfg.readable   = false;
            cfg.invert     = true;
            cfg.rgb_order  = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel.config(cfg);
        }

        {
            auto cfg = _light.config();

            cfg.pin_bl = 22;
            cfg.invert = false;

            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light.config(cfg);
            _panel.setLight(&_light);
        }

        setPanel(&_panel);
    }
};

LGFX tft;

void setup()
{
    tft.init();
    tft.setRotation(1);
    tft.setBrightness(255);
}

void loop()
{
    static int speed = 0;

    // background
    tft.fillScreen(TFT_BLACK);

    // ukuran asli LCD
    int w = tft.width();
    int h = tft.height();

    // header
    tft.fillRoundRect(0, 0, w, 36, 8, TFT_BLUE);

    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextSize(2);
    tft.drawCentreString("RC DASHBOARD", w / 2, 10, 2);

    // angka speed
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(6);

    String spd = String(speed);

    tft.drawCentreString(spd, w / 2, 70, 4);

    // kmh
    tft.setTextSize(2);
    tft.drawCentreString("KM/H", w / 2, 135, 2);

    // progress bar
    int barWidth = map(speed, 0, 120, 0, w - 40);

    tft.drawRoundRect(20, h - 40, w - 40, 20, 10, TFT_WHITE);

    tft.fillRoundRect(20, h - 40, barWidth, 20, 10, TFT_GREEN);

    // animasi
    speed++;

    if(speed > 120)
    {
        speed = 0;
    }

    delay(50);
}
   