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