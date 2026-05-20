#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX() {
    {
      auto cfg = _bus.config();

      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;

      cfg.pin_sclk = 6;
      cfg.pin_mosi = 7;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 4;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();

      cfg.pin_cs           = 5;
      cfg.pin_rst          = 8;
      cfg.pin_busy         = -1;

      cfg.memory_width     = 172;
      cfg.memory_height    = 320;

      cfg.panel_width      = 172;
      cfg.panel_height     = 320;

      cfg.offset_x         = 34;
      cfg.offset_y         = 0;

      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;

      cfg.readable         = false;
      cfg.invert           = true;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       = true;

      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};

LGFX tft;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[172 * 20];

lv_obj_t *speedLabel;
lv_obj_t *arc;

void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p)
{
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.pushImage(area->x1, area->y1, w, h,
                (uint16_t *)&color_p->full);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void create_dashboard()
{
  lv_obj_set_style_bg_color(
      lv_scr_act(),
      lv_color_hex(0x101828),
      0);

  speedLabel = lv_label_create(lv_scr_act());

  lv_obj_set_style_text_color(
      speedLabel,
      lv_color_white(),
      0);

  lv_obj_set_style_text_font(
      speedLabel,
      &lv_font_montserrat_20,
      0);

  lv_label_set_text(speedLabel, "0 KM/H");

  lv_obj_align(speedLabel,
               LV_ALIGN_TOP_MID,
               0,
               12);

  arc = lv_arc_create(lv_scr_act());

  lv_obj_set_size(arc, 100, 100);

  lv_arc_set_rotation(arc, 135);
  lv_arc_set_bg_angles(arc, 0, 270);
  lv_arc_set_value(arc, 0);

  lv_obj_set_style_arc_width(arc, 10, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 10, LV_PART_INDICATOR);

  lv_obj_set_style_arc_color(
      arc,
      lv_color_hex(0x00E5FF),
      LV_PART_INDICATOR);

  lv_obj_set_style_arc_color(
      arc,
      lv_color_hex(0x303030),
      LV_PART_MAIN);

  lv_obj_center(arc);
}

void setup()
{
  tft.init();

  tft.setRotation(1);

  lv_init();

  lv_disp_draw_buf_init(
      &draw_buf,
      buf,
      NULL,
      172 * 20);

  static lv_disp_drv_t disp_drv;

  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = 320;
  disp_drv.ver_res = 172;

  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

  create_dashboard();
}

void loop()
{
  lv_timer_handler();
  delay(5);
}