#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[240 * 20];

lv_obj_t *speedLabel;
lv_obj_t *arc;

void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void create_dashboard()
{
    lv_obj_set_style_bg_color(lv_scr_act(),
                              lv_color_hex(0x101820),
                              LV_PART_MAIN);

    // SPEED TEXT
    speedLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_color(speedLabel,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(speedLabel,
                               &lv_font_montserrat_20,
                               LV_PART_MAIN);

    lv_label_set_text(speedLabel, "0 KM/H");

    lv_obj_align(speedLabel,
                 LV_ALIGN_TOP_MID,
                 0,
                 15);

    // SPEED ARC
    arc = lv_arc_create(lv_scr_act());

    lv_obj_set_size(arc, 110, 110);

    lv_obj_align(arc,
                 LV_ALIGN_CENTER,
                 0,
                 15);

    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 70);

    // REMOVE KNOB
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);

    // ARC BACKGROUND
    lv_obj_set_style_arc_width(arc,
                               10,
                               LV_PART_MAIN);

    lv_obj_set_style_arc_color(arc,
                               lv_color_hex(0x303030),
                               LV_PART_MAIN);

    // ARC INDICATOR
    lv_obj_set_style_arc_width(arc,
                               10,
                               LV_PART_INDICATOR);

    lv_obj_set_style_arc_color(arc,
                               lv_color_hex(0x00E5FF),
                               LV_PART_INDICATOR);

    // REMOVE CENTER
    lv_obj_set_style_bg_opa(arc,
                            LV_OPA_TRANSP,
                            LV_PART_MAIN);
}

void setup()
{
    Serial.begin(115200);

    tft.init();

    // LANDSCAPE
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf,
                          buf,
                          NULL,
                          240 * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 172;
    disp_drv.ver_res = 320;

    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    create_dashboard();
}

void loop()
{
    lv_timer_handler();
    delay(5);
}