#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <math.h>

// ==========================
// DISPLAY CONFIG
// ==========================
#define TFT_BL 22

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    15, // DC
    14, // CS
    7,  // SCK
    6,  // MOSI
    -1  // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    1,    // rotation
    true, // IPS
    172,
    320
);

// ==========================
// PWM INPUT
// ==========================
#define CH1_PIN 4
#define CH2_PIN 5

// ==========================
// SCREEN
// ==========================
#define SCREEN_WIDTH 172
#define SCREEN_HEIGHT 320

// ==========================
// LVGL BUFFER
// ==========================
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 20];

// ==========================
// UI OBJECTS
// ==========================
lv_obj_t *speedArc;
lv_obj_t *speedLabel;
lv_obj_t *rpmBar;
lv_obj_t *throttleBar;
lv_obj_t *steeringLabel;

// ==========================
// DISPLAY FLUSH
// ==========================
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

// ==========================
// READ PWM
// ==========================
int readPWM(int pin)
{
    uint32_t pulse = pulseIn(pin, HIGH, 25000);

    if (pulse < 900 || pulse > 2100)
        return 1500;

    return pulse;
}

// ==========================
// CREATE UI
// ==========================
void createUI()
{
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        0);

    speedArc = lv_arc_create(lv_scr_act());

    lv_obj_set_size(speedArc, 180, 180);

    lv_obj_center(speedArc);

    lv_arc_set_rotation(speedArc, 135);

    lv_arc_set_bg_angles(speedArc, 0, 270);

    lv_arc_set_range(speedArc, 0, 240);

    lv_arc_set_value(speedArc, 0);

    lv_obj_remove_style(
        speedArc,
        NULL,
        LV_PART_KNOB);

    speedLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        speedLabel,
        &lv_font_montserrat_48,
        0);

    lv_obj_set_style_text_color(
        speedLabel,
        lv_color_white(),
        0);

    lv_label_set_text(speedLabel, "0");

    lv_obj_align(
        speedLabel,
        LV_ALIGN_CENTER,
        0,
        -10);

    rpmBar = lv_bar_create(lv_scr_act());

    lv_obj_set_size(rpmBar, 220, 10);

    lv_obj_align(
        rpmBar,
        LV_ALIGN_BOTTOM_MID,
        0,
        -15);

    lv_bar_set_range(rpmBar, 0, 8000);

    throttleBar = lv_bar_create(lv_scr_act());

    lv_obj_set_size(throttleBar, 20, 100);

    lv_obj_align(
        throttleBar,
        LV_ALIGN_LEFT_MID,
        15,
        0);

    steeringLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_color(
        steeringLabel,
        lv_color_white(),
        0);

    lv_label_set_text(
        steeringLabel,
        "0 DEG");

    lv_obj_align(
        steeringLabel,
        LV_ALIGN_RIGHT_MID,
        -15,
        0);
}

// ==========================
// UPDATE DASHBOARD
// ==========================
void updateDashboard()
{
    int ch1 = readPWM(CH1_PIN);
    int ch2 = readPWM(CH2_PIN);

    int steeringValue = map(
        ch1,
        1000,
        2000,
        -90,
        90);

    int throttleValue = map(
        ch2,
        1000,
        2000,
        0,
        100);

    int speedValue = map(
        ch2,
        1000,
        2000,
        0,
        240);

    int rpmValue = map(
        ch2,
        1000,
        2000,
        0,
        8000);

    lv_arc_set_value(
        speedArc,
        speedValue);

    lv_label_set_text_fmt(
        speedLabel,
        "%d",
        speedValue);

    lv_bar_set_value(
        rpmBar,
        rpmValue,
        LV_ANIM_ON);

    lv_bar_set_value(
        throttleBar,
        throttleValue,
        LV_ANIM_ON);

    lv_label_set_text_fmt(
        steeringLabel,
        "%d DEG",
        steeringValue);
}

// ==========================
// SETUP
// ==========================
void setup()
{
    Serial.begin(115200);

    pinMode(CH1_PIN, INPUT);
    pinMode(CH2_PIN, INPUT);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();
    gfx->fillScreen(BLACK);

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

    createUI();
}

// ==========================
// LOOP
// ==========================
void loop()
{
    updateDashboard();

    lv_timer_handler();

    delay(5);
}