#include <LovyanGFX.hpp>
#include <lvgl.h>

// ===========================
// Ukuran LCD Waveshare
// ESP32-C6 LCD 1.47"
// ===========================
#define SCREEN_WIDTH   172
#define SCREEN_HEIGHT  320

// ===========================
// TFT
// ===========================
TFT_eSPI lcd = TFT_eSPI();

// ===========================
// LVGL Display
// ===========================
static lv_display_t *disp;

// Buffer render sebagian layar
static lv_color_t buf1[SCREEN_WIDTH * 40];

// ===========================
// Flush Callback
// ===========================
void my_disp_flush(lv_display_t *disp,
                   const lv_area_t *area,
                   uint8_t *px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    lcd.startWrite();

    lcd.setAddrWindow(
        area->x1,
        area->y1,
        w,
        h
    );

    lcd.pushColors(
        (uint16_t *)px_map,
        w * h,
        true
    );

    lcd.endWrite();

    lv_display_flush_ready(disp);
}

// ===========================
// UI Dashboard
// ===========================
void create_dashboard()
{
    // Background
    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_color_hex(0x101010),
        0
    );

    // Title
    lv_obj_t *title = lv_label_create(lv_screen_active());

    lv_label_set_text(title, "RC DASHBOARD");

    lv_obj_set_style_text_color(
        title,
        lv_color_hex(0x00FFCC),
        0
    );

    lv_obj_set_style_text_font(
        title,
        &lv_font_montserrat_20,
        0
    );

    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Speed Label
    lv_obj_t *speed = lv_label_create(lv_screen_active());

    lv_label_set_text(speed, "SPEED\n72");

    lv_obj_set_style_text_color(
        speed,
        lv_color_hex(0xFFFFFF),
        0
    );

    lv_obj_set_style_text_font(
        speed,
        &lv_font_montserrat_32,
        0
    );

    lv_obj_align(speed, LV_ALIGN_CENTER, 0, -20);

    // Battery Bar
    lv_obj_t *bar = lv_bar_create(lv_screen_active());

    lv_obj_set_size(bar, 120, 18);

    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -30);

    lv_bar_set_range(bar, 0, 100);

    lv_bar_set_value(bar, 85, LV_ANIM_OFF);

    lv_obj_set_style_bg_color(
        bar,
        lv_color_hex(0x303030),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        bar,
        lv_color_hex(0x00FF00),
        LV_PART_INDICATOR
    );

    // Battery Text
    lv_obj_t *bat = lv_label_create(lv_screen_active());

    lv_label_set_text(bat, "BATTERY 85%");

    lv_obj_set_style_text_color(
        bat,
        lv_color_hex(0xFFFFFF),
        0
    );

    lv_obj_align_to(
        bat,
        bar,
        LV_ALIGN_OUT_TOP_MID,
        0,
        -8
    );
}

// ===========================
// Setup
// ===========================
void setup()
{
    Serial.begin(115200);

    // TFT Init
    lcd.init();

    lcd.setRotation(0);

    lcd.fillScreen(TFT_BLACK);

    // LVGL Init
    lv_init();

    // Create display
    disp = lv_display_create(
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    // Flush callback
    lv_display_set_flush_cb(
        disp,
        my_disp_flush
    );

    // Display buffer
    lv_display_set_buffers(
        disp,
        buf1,
        NULL,
        sizeof(buf1),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    // Create UI
    create_dashboard();
}

// ===========================
// Loop
// ===========================
void loop()
{
    lv_timer_handler();

    delay(5);
}