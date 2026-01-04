#include "mouse.h"
#include "button.h"
#include "event.h"
#include <Arduino.h>
#include <stdint.h>

mouse_t *init_mouse(uint8_t m1, uint8_t m2, uint8_t ptr_x, uint8_t ptr_y,
                    uint8_t sens) {
  mouse_t *ms = (mouse_t *)malloc(sizeof(mouse_t));
  ms->pointer = init_joystick(ptr_x, ptr_y);
  ms->m1 = init_button(m1);
  ms->m2 = init_button(m2);
  ms->m1_event_pressed = init_event(M1_PRESSED);
  ms->m1_event_down = init_event(M1_DOWN);
  ms->m1_event_released = init_event(M1_RELEASED);
  ms->m2_event_pressed = init_event(M2_PRESSED);
  ms->m2_event_down = init_event(M2_DOWN);
  ms->m2_event_released = init_event(M2_RELEASED);
  ms->ptr_event = init_event(POINTER_MOVED);
  ms->sensitivity = sens;
  return ms;
}

void hook_mouse_event(mouse_t *ms, mouse_event_t event, void *fn) {
  switch (event) {
  case M1_PRESSED:
    hook_event(ms->m1_event_pressed, fn);
    break;
  case M1_DOWN:
    hook_event(ms->m1_event_down, fn);
    break;
  case M1_RELEASED:
    hook_event(ms->m1_event_released, fn);
    break;
  case POINTER_MOVED:
    hook_event(ms->ptr_event, fn);
    break;
  case M2_PRESSED:
    hook_event(ms->m2_event_pressed, fn);
    break;
  case M2_DOWN:
    break;
  case M2_RELEASED:
    hook_event(ms->m2_event_released, fn);
    break;
  }
}

void update_mouse_button(button_t *btn, event_t *e_pressed, event_t *e_down,
                         event_t *e_released) {
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

  printf("ddd\n");
  update_joystick(ms->pointer);
  update_mouse_button(ms->m1, ms->m1_event_pressed, ms->m1_event_down,
                      ms->m1_event_released);
  update_mouse_button(ms->m2, ms->m2_event_pressed, ms->m2_event_down,
                      ms->m2_event_released);
  int16_t x = (int16_t)((ms->pointer->value->x * ms->sensitivity));
  int16_t y = (int16_t)((ms->pointer->value->y * ms->sensitivity));
  if (x != 0 || y != 0) {
    void *args[] = {&x, &y};
    trigger_event(ms->ptr_event, args, 2);
  }
}
