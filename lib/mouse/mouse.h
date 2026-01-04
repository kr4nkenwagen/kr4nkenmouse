#ifndef MOUSE
#define MOUSE
#include "button.h"
#include "event.h"
#include "joystick.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum mouse_event {
  M1_PRESSED,
  M1_DOWN,
  M1_RELEASED,
  M2_PRESSED,
  M2_DOWN,
  M2_RELEASED,
  POINTER_MOVED
} mouse_event_t;

typedef struct mouse {
  uint8_t sensitivity;
  button_t *m1;
  button_t *m2;
  joystick_t *pointer;
  event_t *m1_event_pressed;
  event_t *m1_event_down;
  event_t *m1_event_released;
  event_t *m2_event_pressed;
  event_t *m2_event_down;
  event_t *m2_event_released;
  event_t *ptr_event;
} mouse_t;

mouse_t *init_mouse(uint8_t m1, uint8_t m2, uint8_t ptr_x, uint8_t ptr_y,
                    uint8_t sens);
void update_mouse(mouse_t *ms);
void hook_mouse_event(mouse_t *ms, mouse_event_t event, void *fn);
void delete_mouse(mouse_t *ms);
#ifdef __cplusplus
}
#endif
#endif
