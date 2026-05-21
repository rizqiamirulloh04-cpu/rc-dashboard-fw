#include <Arduino.h>
#include <lvgl.h>

void setup()
{
    Serial.begin(115200);

    lv_init();

    // Background hitam
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        LV_PART_MAIN
    );

    // Label
    lv_obj_t *label = lv_label_create(lv_scr_act());

    lv_label_set_text(label, "RC DASHBOARD");

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0x00FFCC),
        LV_PART_MAIN
    );

    lv_obj_align(
        label,
        LV_ALIGN_CENTER,
        0,
        0
    );
}

void loop()
{
    lv_timer_handler();
    delay(5);
}