#ifndef OLED_DISPLAY
#define OLED_DISPLAY

#include "vector2.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "nice_view.h"

#define LOG_BUF_MAX 256

typedef struct {
  nice_view_t *disp;
  char buf[LOG_BUF_MAX];
  uint16_t head; // Where we write new chars
  uint16_t tail; // Where the display task is currently reading
  uvector2_t size;
  uint8_t current_row;
  uint8_t current_col;
} logger_t;

void init_logger(nice_view_t *disp);
void logmsg(const char *msg);
void update_logger();

#ifdef __cplusplus
}
#endif
#endif
