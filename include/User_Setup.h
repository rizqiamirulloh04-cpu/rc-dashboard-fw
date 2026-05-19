#ifndef USER_SETUP_H
#define USER_SETUP_H

// =====================================================
// DRIVER
// =====================================================
#define ST7789_DRIVER

// =====================================================
// DISPLAY SIZE
// =====================================================
#define TFT_WIDTH  172
#define TFT_HEIGHT 320

// =====================================================
// ESP32-C6 WAVESHARE PINS
// =====================================================
#define TFT_MOSI 6
#define TFT_SCLK 7

#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21

#define TFT_BL   22

// =====================================================
// SPI SPEED
// =====================================================
#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000

// =====================================================
// COLORS
// =====================================================
#define TFT_RGB_ORDER TFT_RGB

// =====================================================
// FONT
// =====================================================
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8

#define LOAD_GFXFF

#define SMOOTH_FONT

#endif