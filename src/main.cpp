#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "waveshare_lcd.h"

#define SCREEN_WIDTH 172
#define SCREEN_HEIGHT 320

#define BLACK 0x0000

Arduino_DataBus *bus = new Arduino_HWSPI(
    TFT_DC,
    TFT_CS,
    SCK,
    MOSI,
    MISO);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,
    true,
    SCREEN_WIDTH,
    SCREEN_HEIGHT);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 20];

lv_obj_t *speedLabel;
lv_obj_t *statusLabel;
lv_obj_t *barThrottle;
lv_obj_t *barSteering;

void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    gfx->draw16bitRGBBitmap(
        area->x1,
        area->y1,
        (uint16_t *)&color_p->full,
        w,
        h);

    lv_disp_flush_ready(disp);
}

void create_ui()
{
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        LV_PART_MAIN);

    speedLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_color(
        speedLabel,
        lv_color_hex(0x00FFAA),
        LV_PART_MAIN);

    lv_obj_set_style_text_font(
        speedLabel,
        &lv_font_montserrat_28,
        LV_PART_MAIN);

    lv_label_set_text(speedLabel, "125");

    lv_obj_align(
        speedLabel,
        LV_ALIGN_TOP_MID,
        0,
        25);

    statusLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_color(
        statusLabel,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN);

    lv_obj_set_style_text_font(
        statusLabel,
        &lv_font_montserrat_16,
        LV_PART_MAIN);

    lv_label_set_text(statusLabel, "RC CONNECTED");

    lv_obj_align(
        statusLabel,
        LV_ALIGN_TOP_MID,
        0,
        80);

    barThrottle = lv_bar_create(lv_scr_act());

    lv_obj_set_size(
        barThrottle,
        30,
        140);

    lv_obj_align(
        barThrottle,
        LV_ALIGN_BOTTOM_LEFT,
        20,
        -20);

    lv_bar_set_range(
        barThrottle,
        0,
        100);

    lv_bar_set_value(
        barThrottle,
        80,
        LV_ANIM_OFF);

    barSteering = lv_bar_create(lv_scr_act());

    lv_obj_set_size(
        barSteering,
        140,
        20);

    lv_obj_align(
        barSteering,
        LV_ALIGN_BOTTOM_MID,
        20,
        -30);

    lv_bar_set_range(
        barSteering,
        -100,
        100);

    lv_bar_set_value(
        barSteering,
        35,
        LV_ANIM_OFF);
}

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();
    gfx->fillScreen(0x0000);

    lv_init();

    lv_disp_draw_buf_init(
        &draw_buf,
        buf,
        NULL,
        SCREEN_WIDTH * 20);

    static lv_disp_drv_t disp_drv;

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    create_ui();

    Serial.println("Dashboard Started");
}

void loop()
{
    lv_timer_handler();
    delay(5);
}