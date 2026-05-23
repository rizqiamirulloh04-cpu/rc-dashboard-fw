#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>

#define TFT_BL   15

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_DC   8
#define TFT_RST  9
#define TFT_CS   14

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    GFX_NOT_DEFINED
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,
    true,
    172,
    320,
    34,
    0,
    0,
    0
);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[172 * 20];

void my_flush(lv_disp_drv_t *disp,
              const lv_area_t *area,
              lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    gfx->draw16bitRGBBitmap(
        area->x1,
        area->y1,
        (uint16_t *)color_p,
        w,
        h
    );

    lv_disp_flush_ready(disp);
}

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->setRotation(1);

    gfx->fillScreen(BLACK);

    delay(1000);

    lv_init();

    lv_disp_draw_buf_init(
        &draw_buf,
        buf,
        NULL,
        172 * 20
    );

    static lv_disp_drv_t disp_drv;

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 320;
    disp_drv.ver_res = 172;
    disp_drv.flush_cb = my_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        0
    );

    lv_obj_t *label = lv_label_create(lv_scr_act());

    lv_label_set_text(
        label,
        "RC DASHBOARD"
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0x00FFFF),
        0
    );

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_22,
        0
    );

    lv_obj_center(label);
}

void loop()
{
    lv_timer_handler();
    delay(5);
}