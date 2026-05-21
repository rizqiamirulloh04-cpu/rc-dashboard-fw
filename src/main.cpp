void setup()
{
    Serial.begin(115200);

    lcd.init();

    lcd.setBrightness(255);

    lcd.setRotation(3);

    lcd.fillScreen(TFT_RED);
    delay(1000);

    lcd.fillScreen(TFT_GREEN);
    delay(1000);

    lcd.fillScreen(TFT_BLUE);
    delay(1000);

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