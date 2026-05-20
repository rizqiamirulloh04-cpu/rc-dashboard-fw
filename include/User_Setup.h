#pragma once

#define USER_SETUP_INFO "Waveshare ESP32-C6 1.47 LCD"

// =====================
// Driver LCD
// =====================
#define ST7789_DRIVER

// =====================
// Resolusi LCD
// =====================
#define TFT_WIDTH  320
#define TFT_HEIGHT 172

// =====================
// Pin SPI
// =====================
#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21
#define TFT_BL   22

// =====================
// SPI Speed
// =====================
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000

// =====================
// Font
// =====================
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT