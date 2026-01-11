#include "logger.h"
#include "nice_view.h"
#include <string.h>
#include <sys/unistd.h>

static logger_t logger;

void init_logger(nice_view_t *disp) {
  logger.disp = disp;
  logger.size = (uvector2_t){.x = 20, .y = 8};
  logger.head = 0;
  logger.tail = 0;
  logger.current_row = 0;
  logger.current_col = 0;
  memset(logger.buf, 0, LOG_BUF_MAX);
  deep_clean_display(disp);
}

void logmsg(const char *msg) {
  if (!msg)
    return;
  while (*msg != '\0') {
    uint16_t next = (logger.head + 1) % LOG_BUF_MAX;
    if (next == logger.tail) {
      break;
    }
    logger.buf[logger.head] = *msg;
    logger.head = next;
    msg++;
  }
  uint16_t next = (logger.head + 1) % LOG_BUF_MAX;
  if (next != logger.tail) {
    logger.buf[logger.head] = '\n';
    logger.head = next;
  }
}

void update_logger() {
  if (logger.head == logger.tail) {
    return;
  }
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
  uvector2_t print_pos = {.x = 10 + (logger.current_row * 7),
                          .y = 10 + (logger.current_col * 8)};
  draw_char(logger.disp, print_pos, c, 1);
  logger.current_col++;
  if (logger.current_col >= logger.size.x) {
    logger.current_row++;
    logger.current_col = 0;
  }
  logger.tail = (logger.tail + 1) % LOG_BUF_MAX;
}
