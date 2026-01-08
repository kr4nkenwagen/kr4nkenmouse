#ifndef MOUSE
#define MOUSE
#include "button.h"
#include "event.h"
#include "joystick.h"
#include "led.h"
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
  M4_PRESSED,
  M4_DOWN,
  M4_RELEASED,
  M5_PRESSED,
  M5_DOWN,
  M5_RELEASED
} mouse_event_t;

typedef struct mouse {
  uint8_t sensitivity;
  button_t m1;
  button_t m2;
  button_t m4;
  button_t m5;
  joystick_t pointer;
  event_t m1_event_pressed;
  event_t m1_event_down;
  event_t m1_event_released;
  event_t m2_event_pressed;
  event_t m2_event_down;
  event_t m2_event_released;
  event_t m4_event_pressed;
  event_t m4_event_down;
  event_t m4_event_released;
  event_t m5_event_pressed;
  event_t m5_event_down;
  event_t m5_event_released;
  event_t ptr_event;
  led_t on_led;
} mouse_t;

void init_mouse(mouse_t *ms, uint8_t m1, uint8_t m2, uint8_t ptr_x,
                uint8_t ptr_y, uint8_t m4, uint8_t m5, uint8_t sens,
                uint8_t on_led);
void update_mouse(mouse_t *ms);
void hook_mouse_event(mouse_t *ms, mouse_event_t event, void *fn);
#ifdef __cplusplus
}
#endif
#endif
