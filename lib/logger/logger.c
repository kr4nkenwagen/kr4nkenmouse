#include "logger.h"
#include "nice_view.h"
#include <string.h>
#include <sys/unistd.h>

static logger_t logger;

void init_logger(nice_view_t *disp) {
  // 1. Assign the display handler
  logger.disp = disp;

  // 2. Initialize the logical character grid
  // 160 pixels / 8 pixels per char = 20 columns
  // 160 pixels / 12 pixels per line = 13 rows
  logger.size = (uvector2_t){.x = 20, .y = 8};

  // 3. Reset Queue Pointers
  logger.head = 0;
  logger.tail = 0;

  // 4. Reset Screen Position
  logger.current_row = 0;
  logger.current_col = 0;

  // 5. Clear the memory buffer
  memset(logger.buf, 0, LOG_BUF_MAX);

  // 6. Start with a fresh screen
  deep_clean_display(disp);
}

void logmsg(const char *msg) {
  if (!msg)
    return;

  while (*msg != '\0') {
    uint16_t next = (logger.head + 1) % LOG_BUF_MAX;
    if (next == logger.tail)
      break;

    logger.buf[logger.head] = *msg;
    logger.head = next;
    msg++;
  }

  // Explicitly add a newline character after the message
  uint16_t next = (logger.head + 1) % LOG_BUF_MAX;
  if (next != logger.tail) {
    logger.buf[logger.head] = '\n';
    logger.head = next;
  }
}

void update_logger() {
  if (logger.head == logger.tail)
    return;

  char c = logger.buf[logger.tail];

  if (c == '\n') {
    logger.current_row++;
    logger.current_col = 0;
    logger.tail = (logger.tail + 1) % LOG_BUF_MAX;

    if (logger.current_row >= logger.size.y) {
      deep_clean_display(logger.disp);
      logger.current_row = 0;
    }
    return;
  }

  uvector2_t print_pos = {// Increase 16 to 20 to ensure NO scanline overlap
                          .x = 10 + (logger.current_row * 20),
                          .y = 10 + (logger.current_col * 8)};

  draw_char(logger.disp, print_pos, c, 1);
  logger.current_col++;

  if (logger.current_col >= logger.size.x) {
    logger.current_row++;
    logger.current_col = 0;
  }

  logger.tail = (logger.tail + 1) % LOG_BUF_MAX;
}
