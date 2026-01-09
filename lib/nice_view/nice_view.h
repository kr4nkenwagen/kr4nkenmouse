#ifndef OLED_DISPLAY
#define OLED_DISPLAY

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "vector2.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
typedef struct nice_view {
  uint8_t sck_pin;
  uint8_t cs_pin;
  uint8_t mosi_pin;
  spi_bus_config_t bus;
  spi_device_interface_config_t interface_config;
  spi_device_handle_t handler;
} nice_view_t;

void init_oled_display(nice_view_t *nv, uint8_t sck_pin, uint8_t cs_pin,
                       uint8_t mosi_pin);
void clear_oled_display(nice_view_t *nv);
void write_first_line(nice_view_t *nv);
void draw_char(nice_view_t *nv, uvector2_t pos, char c, uint8_t color);
void draw_string(nice_view_t *nv, uvector2_t pos, const char *str);
#ifdef __cplusplus
}
#endif
#endif
