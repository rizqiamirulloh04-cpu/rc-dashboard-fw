#pragma once

#define LGFX_USE_V1

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
            cfg.freq_write = 80000000;
            cfg.freq_read  = 20000000;

            cfg.spi_3wire = true;

            cfg.use_lock = true;

            cfg.dma_channel = SPI_DMA_CH_AUTO;

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

            cfg.freq = 44100;

            cfg.pwm_channel = 7;

            _light.config(cfg);
            _panel.setLight(&_light);
        }

        setPanel(&_panel);
    }
};