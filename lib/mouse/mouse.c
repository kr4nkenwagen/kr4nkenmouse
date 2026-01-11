#include "mouse.h"
#include "button.h"
#include "event.h"
#include "led.h"
#include "logger.h"
#include <Arduino.h>
#include <stdint.h>

void init_mouse(mouse_t *ms, uint8_t m1, uint8_t m2, uint8_t ptr_x,
                uint8_t ptr_y, uint8_t m4, uint8_t m5, uint8_t sens,
                uint8_t power_led) {
  init_joystick(&ms->pointer, ptr_x, ptr_y);
  init_button(&ms->m1, m1);
  init_button(&ms->m2, m2);
  init_button(&ms->m4, m4);
  init_button(&ms->m5, m5);
  init_event(&ms->m1_event_pressed, M1_PRESSED);
  init_event(&ms->m1_event_down, M1_DOWN);
  init_event(&ms->m1_event_released, M1_RELEASED);
  init_event(&ms->m2_event_pressed, M2_PRESSED);
  init_event(&ms->m2_event_down, M2_DOWN);
  init_event(&ms->m2_event_released, M2_RELEASED);
  init_event(&ms->m4_event_pressed, M4_PRESSED);
  init_event(&ms->m4_event_down, M4_DOWN);
  init_event(&ms->m4_event_released, M4_RELEASED);
  init_event(&ms->m5_event_pressed, M5_PRESSED);
  init_event(&ms->m5_event_down, M5_DOWN);
  init_event(&ms->m5_event_released, M5_RELEASED);
  init_event(&ms->ptr_event, POINTER_MOVED);
  init_led(&ms->power_led, power_led);
  ms->sensitivity = sens;
}

void hook_mouse_event(mouse_t *ms, mouse_event_t event, void *fn) {
  switch (event) {
  case M1_PRESSED:
    hook_event(&ms->m1_event_pressed, fn);
    break;
  case M1_DOWN:
    hook_event(&ms->m1_event_down, fn);
    break;
  case M1_RELEASED:
    hook_event(&ms->m1_event_released, fn);
    break;
  case POINTER_MOVED:
    hook_event(&ms->ptr_event, fn);
    break;
  case M2_PRESSED:
    hook_event(&ms->m2_event_pressed, fn);
    break;
  case M2_DOWN:
    hook_event(&ms->m2_event_down, fn);
    break;
  case M2_RELEASED:
    hook_event(&ms->m2_event_released, fn);
    break;
  case M4_PRESSED:
    hook_event(&ms->m4_event_pressed, fn);
    break;
  case M4_DOWN:
    hook_event(&ms->m4_event_down, fn);
    break;
  case M4_RELEASED:
    hook_event(&ms->m4_event_released, fn);
    break;
  case M5_PRESSED:
    hook_event(&ms->m5_event_pressed, fn);
    break;
  case M5_DOWN:
    hook_event(&ms->m5_event_down, fn);
    break;
  case M5_RELEASED:
    hook_event(&ms->m5_event_released, fn);
    break;
  }
}

static void update_mouse_button(button_t *btn, event_t *e_pressed,
                                event_t *e_down, event_t *e_released) {
  update_button(btn);
  switch (btn->state) {
  case PRESSED:
    trigger_event(e_pressed, NULL, 0);
    break;
  case RELEASED:
    trigger_event(e_released, NULL, 0);
    break;
  case DOWN:
    trigger_event(e_down, NULL, 0);
    break;
  case IDLE:
    break;
  }
}

void update_mouse(mouse_t *ms) {
  update_joystick(&ms->pointer);
  update_mouse_button(&ms->m1, &ms->m1_event_pressed, &ms->m1_event_down,
                      &ms->m1_event_released);
  update_mouse_button(&ms->m2, &ms->m2_event_pressed, &ms->m2_event_down,
                      &ms->m2_event_released);
  update_mouse_button(&ms->m4, &ms->m4_event_pressed, &ms->m4_event_down,
                      &ms->m4_event_released);
  update_mouse_button(&ms->m5, &ms->m5_event_pressed, &ms->m5_event_down,
                      &ms->m5_event_released);
  int16_t x = (int16_t)((ms->pointer.position.x * ms->sensitivity));
  int16_t y = (int16_t)((ms->pointer.position.y * ms->sensitivity));
  if (x != 0 || y != 0) {
    void *args[] = {&x, &y};
    trigger_event(&ms->ptr_event, args, 2);
  }
}
