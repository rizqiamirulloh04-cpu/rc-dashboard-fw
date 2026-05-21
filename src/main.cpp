#include <Arduino.h>
#include <lvgl.h>
#include "LGFX_Config.h"

LGFX lcd;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[172 * 20];

void my_flush_cb(lv_disp_drv_t *disp,
                 const lv_area_t *area,
                 lv_color_t *color_p)
{
    int w = area->x2 - area->x1 + 1;
    int h = area->y2 - area->y1 + 1;

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

    lcd.setBrightness(255);

    lcd.setRotation(1);

    lcd.fillScreen(TFT_BLACK);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf,
                          buf,
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
                              lv_color_hex(0x000000),
                              0);

    lv_obj_t *label = lv_label_create(lv_scr_act());

    lv_label_set_text(label,
                      "RC DASHBOARD");

    lv_obj_set_style_text_color(label,
                                lv_color_hex(0x00FFCC),
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