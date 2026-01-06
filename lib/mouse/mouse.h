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
  POINTER_MOVED,
  BACK_PRESSED,
  BACK_DOWN,
  BACK_RELEASED,
  FORWARD_PRESSED,
  FORWARD_DOWN,
  FORWARD_RELEASED
} mouse_event_t;

typedef struct mouse {
  uint8_t sensitivity;
  button_t m1;
  button_t m2;
  button_t back;
  button_t forward;
  joystick_t pointer;
  event_t m1_event_pressed;
  event_t m1_event_down;
  event_t m1_event_released;
  event_t m2_event_pressed;
  event_t m2_event_down;
  event_t m2_event_released;
  event_t back_event_pressed;
  event_t back_event_down;
  event_t back_event_released;
  event_t forward_event_pressed;
  event_t forward_event_down;
  event_t forward_event_released;
  event_t ptr_event;
} mouse_t;

void init_mouse(mouse_t *ms, uint8_t m1, uint8_t m2, uint8_t ptr_x,
                uint8_t ptr_y, uint8_t back, uint8_t forward, uint8_t sens);
void update_mouse(mouse_t *ms);
void hook_mouse_event(mouse_t *ms, mouse_event_t event, void *fn);
#ifdef __cplusplus
}
#endif
#endif
