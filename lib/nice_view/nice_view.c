#include "nice_view.h"
#include "driver/spi_common.h"
#include "esp32-hal.h"
#include "font_5x7.h"
#include "hal/spi_types.h"
#include "vector2.h"
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <stdint.h>
#include <string.h>

void deep_clean_display(nice_view_t *nw) {
  uint8_t mode = 0x01;
  static uint8_t clean_buf[3522];
  memset(clean_buf, 0, sizeof(clean_buf));
  clean_buf[0] = mode;
  for (int i = 0; i < 160; i++) {
    int line_start = 1 + (i * 22);
    clean_buf[line_start] = (uint8_t)(i + 1);
  }
  clean_buf[3521] = 0x00;
  spi_transaction_t t = {
      .length = sizeof(clean_buf) * 8,
      .tx_buffer = clean_buf,
  };
  spi_device_transmit(nw->handler, &t);
}

void init_nice_view_display(nice_view_t *nw, uint8_t sck_pin, uint8_t cs_pin,
                            uint8_t mosi_pin) {
  if (!nw) {
    return;
  }
  nw->sck_pin = sck_pin;
  nw->cs_pin = cs_pin;
  nw->mosi_pin = mosi_pin;
  gpio_reset_pin(nw->cs_pin);
  gpio_set_direction(nw->cs_pin, GPIO_MODE_OUTPUT);
  gpio_set_level(nw->cs_pin, 0);
  vTaskDelay(pdMS_TO_TICKS(100));
  gpio_set_level(nw->cs_pin, 1);
  vTaskDelay(pdMS_TO_TICKS(10));
  gpio_set_level(nw->cs_pin, 0);
  nw->bus = (spi_bus_config_t){.miso_io_num = -1,
                               .mosi_io_num = nw->mosi_pin,
                               .sclk_io_num = nw->sck_pin,
                               .quadwp_io_num = -1,
                               .quadhd_io_num = -1,
                               .max_transfer_sz = 4092};
  nw->interface_config = (spi_device_interface_config_t){
      .clock_speed_hz = 1 * 1000 * 1000,
      .mode = 0,
      .spics_io_num = nw->cs_pin,
      .queue_size = 7,
      .flags = SPI_DEVICE_POSITIVE_CS | SPI_DEVICE_BIT_LSBFIRST,
  };
  spi_bus_initialize(SPI2_HOST, &nw->bus, SPI_DMA_CH_AUTO);
  spi_bus_add_device(SPI2_HOST, &nw->interface_config, &nw->handler);
  deep_clean_display(nw);
}

void clear_nice_view_display(nice_view_t *nw) {
  if (!nw) {
    return;
  }
  static uint8_t full_buffer[3362];
  full_buffer[0] = 0x01;
  for (int i = 0; i < 160; i++) {
    int offset = 1 + (i * 21);
    full_buffer[offset] = i + 1;
    memset(&full_buffer[offset + 1], 0x00, 20);
  }
  full_buffer[3361] = 0x00;
  spi_transaction_t t = {
      .length = 3362 * 8,
      .tx_buffer = full_buffer,
  };
  spi_device_transmit(nw->handler, &t);
}

void draw_char(nice_view_t *nw, uvector2_t pos, char c, uint8_t color) {
  if (c < 32 || c > 126)
    c = ' ';
  uint8_t font_idx = c - 32;

  for (int col = 0; col < 5; col++) {
    uint8_t line_data[20] = {0};
    uint8_t current_line = pos.y + col + 1;

    for (int row = 0; row < 7; row++) {
      if ((font5x7[font_idx][col] >> row) & 0x01) {
        // This is the "Top-to-Bottom" math we had
        int px = 159 - (pos.x + row);

        int byte_idx = px / 8;
        int bit_idx = px % 8;

        if (byte_idx < 20) {
          if (color)
            line_data[byte_idx] |= (1 << bit_idx);
        }
      }
    }

    uint8_t frame[23];
    frame[0] = 0x01;
    frame[1] = current_line;
    memcpy(&frame[2], line_data, 20);
    frame[22] = 0x00;

    spi_transaction_t t = {.length = 23 * 8, .tx_buffer = frame};
    spi_device_transmit(nw->handler, &t);
  }
}

void flush_line_buffer(nice_view_t *nw, uint8_t line_addr,
                       const char *line_text) {
  uint8_t line_data[20] = {0};

  // Iterate through every character in this text line (max ~20 chars)
  for (int char_pos = 0; char_pos < 20; char_pos++) {
    char c = line_text[char_pos];
    if (c == '\0')
      break; // End of string
    if (c < 32 || c > 126)
      c = ' ';

    uint8_t font_idx = c - 32;

    // Find which 'column' of the font matches this physical scanline
    // We calculate this based on the line_addr % char_height
    int font_col = (line_addr - 10) % 8; // Adjust 10 to your Y-offset

    if (font_col >= 0 && font_col < 5) {
      uint8_t char_slice = font5x7[font_idx][font_col];

      for (int bit = 0; bit < 7; bit++) {
        if ((char_slice >> bit) & 0x01) {
          // Map the bit to the correct physical X position
          // pos.x is determined by the char_pos * line_spacing
          int px = 159 - ((char_pos * 16) + bit);

          int byte_idx = px / 8;
          int bit_idx = px % 8;
          if (byte_idx < 20) {
            line_data[byte_idx] |= (1 << bit_idx);
          }
        }
      }
    }
  }

  // Now send the FULL line to the display—this preserves all characters!
  uint8_t frame[23];
  frame[0] = 0x01;
  frame[1] = line_addr;
  memcpy(&frame[2], line_data, 20);
  frame[22] = 0x00;

  spi_transaction_t t = {.length = 23 * 8, .tx_buffer = frame};
  spi_device_transmit(nw->handler, &t);
}
