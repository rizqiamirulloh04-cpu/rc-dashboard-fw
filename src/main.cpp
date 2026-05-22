#include <Arduino.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>

#define TFT_BL 15

// =======================
// LCD CONFIG
// =======================

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    8,    // DC
    14,   // CS
    6,    // SCK
    7,    // MOSI
    -1    // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    1,    // rotation
    true, // IPS
    172,  // width
    320,  // height
    34,   // col offset
    0,    // row offset
    0     // col offset2
);

// =======================
// LVGL BUFFER
// =======================

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[172 * 20];

lv_disp_drv_t disp_drv;

// =======================
// FLUSH CALLBACK
// =======================

void my_disp_flush(
    lv_disp_drv_t *disp,
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

// =======================
// UI
// =======================

lv_obj_t *speedLabel;
lv_obj_t *arc;

void create_ui()
{
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        0);

    // SPEED ARC
    arc = lv_arc_create(lv_scr_act());

    lv_obj_set_size(arc, 150, 150);

    lv_arc_set_rotation(arc, 135);

    lv_arc_set_bg_angles(arc, 0, 270);

    lv_arc_set_range(arc, 0, 180);

    lv_arc_set_value(arc, 108);

    lv_obj_center(arc);

    lv_obj_set_style_arc_width(
        arc,
        10,
        LV_PART_INDICATOR);

    lv_obj_set_style_arc_color(
        arc,
        lv_palette_main(LV_PALETTE_BLUE),
        LV_PART_INDICATOR);

    // SPEED TEXT
    speedLabel = lv_label_create(lv_scr_act());

    lv_label_set_text(speedLabel, "108");

    lv_obj_set_style_text_color(
        speedLabel,
        lv_color_white(),
        0);

    lv_obj_set_style_text_font(
        speedLabel,
        &lv_font_montserrat_48,
        0);

    lv_obj_center(speedLabel);

    // KMH
    lv_obj_t *kmh = lv_label_create(lv_scr_act());

    lv_label_set_text(kmh, "KM/H");

    lv_obj_set_style_text_color(
        kmh,
        lv_palette_main(LV_PALETTE_CYAN),
        0);

    lv_obj_align(
        kmh,
        LV_ALIGN_CENTER,
        0,
        45);

    // BATTERY
    lv_obj_t *bat = lv_bar_create(lv_scr_act());

    lv_obj_set_size(bat, 80, 10);

    lv_bar_set_value(bat, 80, LV_ANIM_OFF);

    lv_obj_align(
        bat,
        LV_ALIGN_TOP_RIGHT,
        -10,
        15);

    // TITLE
    lv_obj_t *title = lv_label_create(lv_scr_act());

    lv_label_set_text(title, "ESP32-C6 RC");

    lv_obj_set_style_text_color(
        title,
        lv_palette_main(LV_PALETTE_BLUE),
        0);

    lv_obj_align(
        title,
        LV_ALIGN_TOP_MID,
        0,
        10);
}

// =======================
// SETUP
// =======================

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    delay(200);

    gfx->begin();

    gfx->fillScreen(BLACK);

    lv_init();

    lv_disp_draw_buf_init(
        &draw_buf,
        buf,
        NULL,
        172 * 20);

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 172;
    disp_drv.ver_res = 320;

    disp_drv.flush_cb = my_disp_flush;

    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    create_ui();
}

// =======================
// LOOP
// =======================

void loop()
{
    lv_timer_handler();
    delay(5);
}