#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789 _panel;
    lgfx::Bus_SPI _bus;

public:
    LGFX(void) {
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

            cfg.pin_cs           = 5;
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

        setPanel(&_panel);
    }
};

LGFX lcd;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[172 * 20];

void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    lcd.endWrite();

    lv_disp_flush_ready(disp);
}

void setup()
{
    Serial.begin(115200);

    lcd.init();
    lcd.setRotation(1);
    lcd.fillScreen(TFT_BLACK);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 172 * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 320;
    disp_drv.ver_res = 172;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_bg_color(lv_scr_act(),
                              lv_color_hex(0x000000),
                              0);

    lv_obj_t *title = lv_label_create(lv_scr_act());

    lv_label_set_text(title, "RC DASHBOARD");

    lv_obj_set_style_text_color(title,
                                lv_color_hex(0x00FFCC),
                                0);

    lv_obj_set_style_text_font(title,
                               &lv_font_montserrat_14,
                               0);

    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t *speed = lv_label_create(lv_scr_act());

    lv_label_set_text(speed, "SPEED 72");

    lv_obj_set_style_text_color(speed,
                                lv_color_hex(0xFFFFFF),
                                0);

    lv_obj_set_style_text_font(speed,
                               &lv_font_montserrat_14,
                               0);

    lv_obj_align(speed, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *status = lv_label_create(lv_scr_act());

    lv_label_set_text(status, "UART CONNECTED");

    lv_obj_set_style_text_color(status,
                                lv_color_hex(0x00FF00),
                                0);

    lv_obj_set_style_text_font(status,
                               &lv_font_montserrat_14,
                               0);

    lv_obj_align(status, LV_ALIGN_BOTTOM_MID, 0, -20);
}

void loop()
{
    lv_timer_handler();
    delay(5);
}