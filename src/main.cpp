#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <math.h>

// ============================================
// DISPLAY CONFIG
// ============================================

#define SCREEN_WIDTH   172
#define SCREEN_HEIGHT  320

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    15,   // DC
    14,   // CS
    7,    // SCK
    6,    // MOSI
    -1    // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    21,   // RST
    1,    // rotation
    true, // IPS
    SCREEN_WIDTH,
    SCREEN_HEIGHT
);

// ============================================
// PWM INPUT
// ============================================

#define CH1_PIN 4
#define CH2_PIN 5
#define CH3_PIN 8

// ============================================
// LVGL BUFFER
// ============================================

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 20];

// ============================================
// UI OBJECTS
// ============================================

lv_obj_t *speedArc;
lv_obj_t *speedLabel;
lv_obj_t *rpmBar;
lv_obj_t *throttleBar;
lv_obj_t *steeringLabel;

lv_obj_t *leftSignal;
lv_obj_t *rightSignal;

lv_obj_t *needleLine;

// ============================================
// NEEDLE POINTS
// ============================================

static lv_point_precise_t needle_points[] =
{
    {0, 0},
    {0, -60}
};

// ============================================
// VALUES
// ============================================

int speedValue = 0;
int rpmValue = 0;
int throttleValue = 0;
int steeringValue = 0;

// ============================================
// SIGNALS
// ============================================

bool leftBlink = false;
bool rightBlink = false;
bool blinkState = false;

unsigned long blinkTimer = 0;

// ============================================
// DISPLAY FLUSH
// ============================================

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
        h
    );

    lv_disp_flush_ready(disp);
}

// ============================================
// READ PWM
// ============================================

int readPWM(int pin)
{
    uint32_t pulse = pulseIn(pin, HIGH, 25000);

    if (pulse < 900 || pulse > 2100)
        return 1500;

    return pulse;
}

// ============================================
// BOOT ANIMATION
// ============================================

void bootAnimation()
{
    lv_obj_t *title = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        title,
        &lv_font_montserrat_32,
        0);

    lv_obj_set_style_text_color(
        title,
        lv_color_white(),
        0);

    lv_label_set_text(
        title,
        "RC DASH");

    lv_obj_center(title);

    lv_timer_handler();

    delay(1500);

    lv_obj_clean(lv_scr_act());
}

// ============================================
// CREATE NEEDLE
// ============================================

void createNeedle()
{
    needleLine = lv_line_create(lv_scr_act());

    lv_line_set_points(
        needleLine,
        needle_points,
        2);

    lv_obj_set_style_line_width(
        needleLine,
        4,
        0);

    lv_obj_set_style_line_color(
        needleLine,
        lv_color_hex(0xFF2200),
        0);
}

// ============================================
// UPDATE NEEDLE
// ============================================

void updateNeedle(int speed)
{
    float angle = map(speed,
                      0,
                      240,
                      -135,
                      135);

    float rad = angle * 0.0174533;

    int radius = 70;

    int cx = 86;
    int cy = 100;

    int x = cx + radius * cos(rad);
    int y = cy + radius * sin(rad);

    needle_points[0].x = cx;
    needle_points[0].y = cy;

    needle_points[1].x = x;
    needle_points[1].y = y;

    lv_line_set_points(
        needleLine,
        needle_points,
        2);
}

// ============================================
// CREATE UI
// ============================================

void createUI()
{
    lv_obj_set_style_bg_color(
        lv_scr_act(),
        lv_color_hex(0x000000),
        0);

    // SPEED ARC
    speedArc = lv_arc_create(lv_scr_act());

    lv_obj_set_size(speedArc,
                    160,
                    160);

    lv_obj_align(speedArc,
                 LV_ALIGN_TOP_MID,
                 0,
                 20);

    lv_arc_set_rotation(speedArc,
                        135);

    lv_arc_set_bg_angles(speedArc,
                         0,
                         270);

    lv_arc_set_range(speedArc,
                     0,
                     240);

    lv_arc_set_value(speedArc,
                     0);

    lv_obj_remove_style(
        speedArc,
        NULL,
        LV_PART_KNOB);

    // SPEED LABEL
    speedLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        speedLabel,
        &lv_font_montserrat_48,
        0);

    lv_obj_set_style_text_color(
        speedLabel,
        lv_color_white(),
        0);

    lv_label_set_text(speedLabel,
                      "0");

    lv_obj_align(speedLabel,
                 LV_ALIGN_TOP_MID,
                 0,
                 70);

    // RPM BAR
    rpmBar = lv_bar_create(lv_scr_act());

    lv_obj_set_size(rpmBar,
                    140,
                    10);

    lv_obj_align(rpmBar,
                 LV_ALIGN_BOTTOM_MID,
                 0,
                 -20);

    lv_bar_set_range(rpmBar,
                     0,
                     8000);

    // THROTTLE
    throttleBar = lv_bar_create(lv_scr_act());

    lv_obj_set_size(throttleBar,
                    18,
                    100);

    lv_obj_align(throttleBar,
                 LV_ALIGN_LEFT_MID,
                 10,
                 20);

    lv_bar_set_range(throttleBar,
                     0,
                     100);

    // STEERING
    steeringLabel = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_color(
        steeringLabel,
        lv_color_white(),
        0);

    lv_label_set_text(
        steeringLabel,
        "0 DEG");

    lv_obj_align(steeringLabel,
                 LV_ALIGN_RIGHT_MID,
                 -10,
                 20);

    // LEFT SIGNAL
    leftSignal = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        leftSignal,
        &lv_font_montserrat_32,
        0);

    lv_obj_set_style_text_color(
        leftSignal,
        lv_color_hex(0xFFD000),
        0);

    lv_label_set_text(
        leftSignal,
        LV_SYMBOL_LEFT);

    lv_obj_align(leftSignal,
                 LV_ALIGN_TOP_LEFT,
                 10,
                 10);

    // RIGHT SIGNAL
    rightSignal = lv_label_create(lv_scr_act());

    lv_obj_set_style_text_font(
        rightSignal,
        &lv_font_montserrat_32,
        0);

    lv_obj_set_style_text_color(
        rightSignal,
        lv_color_hex(0xFFD000),
        0);

    lv_label_set_text(
        rightSignal,
        LV_SYMBOL_RIGHT);

    lv_obj_align(rightSignal,
                 LV_ALIGN_TOP_RIGHT,
                 -10,
                 10);

    createNeedle();
}

// ============================================
// UPDATE SIGNAL
// ============================================

void updateSignal()
{
    if (steeringValue < -20)
    {
        leftBlink = true;
        rightBlink = false;
    }
    else if (steeringValue > 20)
    {
        leftBlink = false;
        rightBlink = true;
    }
    else
    {
        leftBlink = false;
        rightBlink = false;
    }

    if (millis() - blinkTimer > 500)
    {
        blinkState = !blinkState;
        blinkTimer = millis();
    }

    if (leftBlink && blinkState)
        lv_obj_clear_flag(leftSignal, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(leftSignal, LV_OBJ_FLAG_HIDDEN);

    if (rightBlink && blinkState)
        lv_obj_clear_flag(rightSignal, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(rightSignal, LV_OBJ_FLAG_HIDDEN);
}

// ============================================
// UPDATE DASHBOARD
// ============================================

void updateDashboard()
{
    int ch1 = readPWM(CH1_PIN);
    int ch2 = readPWM(CH2_PIN);

    steeringValue = map(ch1,
                        1000,
                        2000,
                        -90,
                        90);

    throttleValue = map(ch2,
                        1000,
                        2000,
                        0,
                        100);

    speedValue = map(ch2,
                     1000,
                     2000,
                     0,
                     240);

    rpmValue = map(ch2,
                   1000,
                   2000,
                   0,
                   8000);

    lv_arc_set_value(speedArc,
                     speedValue);

    lv_label_set_text_fmt(speedLabel,
                          "%d",
                          speedValue);

    lv_bar_set_value(rpmBar,
                     rpmValue,
                     LV_ANIM_ON);

    lv_bar_set_value(throttleBar,
                     throttleValue,
                     LV_ANIM_ON);

    lv_label_set_text_fmt(steeringLabel,
                          "%d DEG",
                          steeringValue);

    updateNeedle(speedValue);

    updateSignal();
}

// ============================================
// SETUP
// ============================================

void setup()
{
    Serial.begin(115200);

    pinMode(CH1_PIN, INPUT);
    pinMode(CH2_PIN, INPUT);
    pinMode(CH3_PIN, INPUT);

    gfx->begin();

    gfx->fillScreen(BLACK);

    lv_init();

    lv_disp_draw_buf_init(
        &draw_buf,
        buf,
        NULL,
        SCREEN_WIDTH * 20
    );

    static lv_disp_drv_t disp_drv;

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;

    disp_drv.flush_cb = my_disp_flush;

    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);

    bootAnimation();

    createUI();
}

// ============================================
// LOOP
// ============================================

void loop()
{
    updateDashboard();

    lv_timer_handler();

    delay(5);
}