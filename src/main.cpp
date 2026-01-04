#include "esp32-hal.h"
#include "event.h"
#include "mouse.h"
#include <Arduino.h>
#include <BleMouse.h>
#include <cstdint>

BleMouse bleMouse("kr4nkenmouse");

#define SENSITIVITY 100
#define BT_REFRESH_RATE 0.01

volatile float delta_time = 0;
volatile uint32_t last_frame_time = 0;

static mouse_t *mouse;

void on_m1_pressed(event_data_t *e) { bleMouse.press(MOUSE_LEFT); }
void on_m1_released(event_data_t *e) { bleMouse.release(MOUSE_LEFT); }
void on_m2_pressed(event_data_t *e) { bleMouse.press(MOUSE_RIGHT); }
void on_m2_released(event_data_t *e) { bleMouse.release(MOUSE_RIGHT); }

void on_mouse_move(event_data_t *e) {
  if (e->argc != 2) {
    return;
  }
  int16_t x = *(int16_t *)e->arg[0];
  int16_t y = *(int16_t *)e->arg[1];
  bleMouse.move(y * delta_time, x * delta_time);
}

void setup() {
  Serial.begin(115200);
  mouse = init_mouse(6, 7, 3, 4, SENSITIVITY);
  hook_mouse_event(mouse, M1_PRESSED, (void *)on_m1_pressed);
  hook_mouse_event(mouse, M1_RELEASED, (void *)on_m1_released);
  hook_mouse_event(mouse, M2_PRESSED, (void *)on_m2_pressed);
  hook_mouse_event(mouse, M2_RELEASED, (void *)on_m2_released);
  hook_mouse_event(mouse, POINTER_MOVED, (void *)on_mouse_move);
  bleMouse.begin();
}

void loop() {
  if (bleMouse.isConnected()) {
    uint32_t current_time = millis();
    delta_time = (current_time - last_frame_time) / 1000.0;
    if (delta_time >= BT_REFRESH_RATE) {
      update_mouse(mouse);
      last_frame_time = current_time;
    }
  }
}
