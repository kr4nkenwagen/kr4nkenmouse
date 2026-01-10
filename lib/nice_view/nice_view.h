#ifndef NICE_VIEW
#define NICE_VIEW

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

void init_nice_view_display(nice_view_t *nv, uint8_t sck_pin, uint8_t cs_pin,
                            uint8_t mosi_pin);
void clear_nice_view_display(nice_view_t *nv);
void draw_char(nice_view_t *nv, uvector2_t pos, char c, uint8_t color);
void draw_string(nice_view_t *nv, uvector2_t pos, const char *str);
void deep_clean_display(nice_view_t *nw);
void flush_line_buffer(nice_view_t *nw, uint8_t line_addr,
                       const char *line_text);
#ifdef __cplusplus
}
#endif
#endif
