#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;

public:
    LGFX(void)
    {
        // SPI
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

        // PANEL
        {
            auto cfg = _panel.config();

            cfg.pin_cs   = 14;
            cfg.pin_rst  = 21;
            cfg.pin_busy = -1;

            // ASLI WAVESHARE
            cfg.memory_width  = 172;
            cfg.memory_height = 320;

            cfg.panel_width  = 172;
            cfg.panel_height = 320;

            cfg.offset_x = 34;
            cfg.offset_y = 0;

            cfg.offset_rotation = 0;

            cfg.readable  = false;
            cfg.invert    = true;
            cfg.rgb_order = false;

            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel.config(cfg);
        }

        // BACKLIGHT
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

// LVGL
static lv_display_t *display;
static lv_color_t draw_buf[320 * 20];

lv_obj_t *speedLabel;
lv_obj_t *arc;

// FLUSH
static void flush_cb(lv_display_t *disp,
                     const lv_area_t *area,
                     uint8_t *px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();

    tft.setAddrWindow(
        area->x1,
        area->y1,
        w,
        h
    );

    tft.writePixels(
        (lgfx::rgb565_t*)px_map,
        w * h
    );

    tft.endWrite();

    lv_display_flush_ready(disp);
}

void create_dashboard()
{
    // Background
    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_color_hex(0x101820),
        0
    );

    // SPEED LABEL
    speedLabel = lv_label_create(
        lv_screen_active()
    );

    lv_label_set_text(
        speedLabel,
        "0 KM/H"
    );

    lv_obj_set_style_text_color(
        speedLabel,
        lv_color_white(),
        0
    );

    lv_obj_set_style_text_font(
        speedLabel,
        &lv_font_montserrat_20,
        0
    );

    lv_obj_align(
        speedLabel,
        LV_ALIGN_TOP_MID,
        0,
        15
    );

    // ARC
    arc = lv_arc_create(
        lv_screen_active()
    );

    lv_obj_set_size(
        arc,
        110,
        110
    );

    lv_obj_align(
        arc,
        LV_ALIGN_CENTER,
        0,
        20
    );

    lv_arc_set_rotation(
        arc,
        135
    );

    lv_arc_set_bg_angles(
        arc,
        0,
        270
    );

    lv_arc_set_range(
        arc,
        0,
        120
    );

    lv_arc_set_value(
        arc,
        0
    );

    lv_obj_remove_style(
        arc,
        NULL,
        LV_PART_KNOB
    );

    lv_obj_set_style_arc_width(
        arc,
        10,
        LV_PART_MAIN
    );

    lv_obj_set_style_arc_width(
        arc,
        10,
        LV_PART_INDICATOR
    );

    lv_obj_set_style_arc_color(
        arc,
        lv_color_hex(0x303030),
        LV_PART_MAIN
    );

    lv_obj_set_style_arc_color(
        arc,
        lv_color_hex(0x00E5FF),
        LV_PART_INDICATOR
    );
}

void setup()
{
    Serial.begin(115200);

    tft.init();

    // LANDSCAPE
    tft.setRotation(1);

    tft.setBrightness(255);

    lv_init();

    // LANDSCAPE LVGL
    display = lv_display_create(
        320,
        172
    );

    lv_display_set_flush_cb(
        display,
        flush_cb
    );

    lv_display_set_buffers(
        display,
        draw_buf,
        NULL,
        sizeof(draw_buf),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    create_dashboard();
}

void loop()
{
    lv_timer_handler();

    static uint32_t last = 0;
    static int speed = 0;
    static int dir = 1;

    if (millis() - last > 40)
    {
        last = millis();

        speed += dir;

        if(speed >= 120)
            dir = -1;

        if(speed <= 0)
            dir = 1;

        char txt[32];

        sprintf(
            txt,
            "%d KM/H",
            speed
        );

        lv_label_set_text(
            speedLabel,
            txt
        );

        lv_arc_set_value(
            arc,
            speed
        );
    }

    delay(5);
}