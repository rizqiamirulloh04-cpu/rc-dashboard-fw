#include <Arduino.h>

#include <lvgl.h>

#include "LGFX_Config.h"

LGFX tft;

static lv_disp_draw_buf_t draw_buf;

static lv_color_t buf1[172 * 20];

void my_flush_cb(lv_disp_drv_t *disp,
                 const lv_area_t *area,
                 lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();

    tft.setAddrWindow(area->x1,
                      area->y1,
                      w,
                      h);

    tft.writePixels((lgfx::rgb565_t *)&color_p->full,
                    w * h);

    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setup()
{
    Serial.begin(115200);

    lv_init();

    tft.init();

    tft.setRotation(1);

    tft.setBrightness(255);

    lv_disp_draw_buf_init(&draw_buf,
                          buf1,
                          NULL,
                          172 * 20);

    static lv_disp_drv_t disp_drv;

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 320;
    disp_drv.ver_res = 172;

    disp_drv.flush_cb = my_flush_cb;

    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_bg_color(lv_scr_act(),
                              lv_color_hex(0x101820),
                              0);

    lv_obj_t *label = lv_label_create(lv_scr_act());

    lv_label_set_text(label,
                      "RC DASHBOARD");

    lv_obj_set_style_text_color(label,
                                lv_color_white(),
                                0);

    lv_obj_set_style_text_font(label,
                               &lv_font_montserrat_14,
                               0);

    lv_obj_center(label);
}

void loop()
{
    lv_timer_handler();

    delay(5);
}