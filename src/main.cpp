#include <Arduino.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;

public:

    LGFX(void)
    {
        // ================= SPI =================
        {
            auto cfg = _bus.config();

            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;

            cfg.freq_write = 40000000;
            cfg.freq_read  = 16000000;

            cfg.spi_3wire = false;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;

            cfg.pin_sclk = 6;
            cfg.pin_mosi = 7;
            cfg.pin_miso = -1;
            cfg.pin_dc   = 15;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        // ================= PANEL =================
        {
            auto cfg = _panel.config();

            cfg.pin_cs   = 14;
            cfg.pin_rst  = 21;
            cfg.pin_busy = -1;

            cfg.memory_width  = 128;
            cfg.memory_height = 160;

            cfg.panel_width  = 128;
            cfg.panel_height = 160;

            cfg.offset_x = 0;
            cfg.offset_y = 0;

            cfg.offset_rotation = 0;

            cfg.readable = false;
            cfg.invert   = false;
            cfg.rgb_order = false;

            cfg.dlen_16bit = false;
            cfg.bus_shared = false;

            _panel.config(cfg);
        }

        // ================= BACKLIGHT =================
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

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(2);

    tft.drawString("WAVESHARE", 20, 20);

    tft.setTextColor(TFT_WHITE);

    tft.drawString("ESP32-C6", 25, 50);

    tft.fillRect(10, 100, 140, 12, TFT_GREEN);
}

void loop()
{
}