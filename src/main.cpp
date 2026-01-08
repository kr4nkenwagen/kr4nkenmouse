#include "esp32-hal.h"
#include "event.h"
#include "mouse.h"
#include "storage.h"
#include <Arduino.h>
#include <BleMouse.h>
#include <cstdint>

BleMouse bleMouse("kr4nkenmouse");

#define BT_REFRESH_RATE 0.01
#define X_PIN 3
#define Y_PIN 4
#define ON_LED_PIN 21

float delta_time = 0;
uint32_t last_frame_time = 0;

static mouse_t mouse;

void on_m1_pressed(event_data_t e) { bleMouse.press(MOUSE_LEFT); }
void on_m1_released(event_data_t e) { bleMouse.release(MOUSE_LEFT); }
void on_m2_pressed(event_data_t e) { bleMouse.press(MOUSE_RIGHT); }
void on_m2_released(event_data_t e) { bleMouse.release(MOUSE_RIGHT); }
void on_m4_pressed(event_data_t e) { bleMouse.press(MOUSE_BACK); }
void on_m4_released(event_data_t e) { bleMouse.release(MOUSE_BACK); }
void on_m5_pressed(event_data_t e) { bleMouse.press(MOUSE_FORWARD); }
void on_m5_released(event_data_t e) { bleMouse.release(MOUSE_FORWARD); }

void on_mouse_move(event_data_t e) {
  if (e.argc != 2) {
    return;
  }
  int16_t x = *(int16_t *)e.arg[0];
  int16_t y = *(int16_t *)e.arg[1];
  bleMouse.move(y * delta_time, x * delta_time);
}

void setup() {
  Serial.begin(115200);
  init_storage();
  int8_t sens = DEFAULT_SENSITIVITY;
  int8_t m1_pin = DEFAULT_M1_PIN;
  int8_t m2_pin = DEFAULT_M2_PIN;
  int8_t m4_pin = DEFAULT_M4_PIN;
  int8_t m5_pin = DEFAULT_M5_PIN;
  get("sensitivity", &sens);
  get("m1_pin", &m1_pin);
  get("m2_pin", &m2_pin);
  get("m4_pin", &m4_pin);
  get("m5_pin", &m5_pin);
  init_mouse(&mouse, (uint8_t)m1_pin, (uint8_t)m2_pin, X_PIN, Y_PIN,
             (uint8_t)m4_pin, (uint8_t)m5_pin, (uint8_t)sens, ON_LED_PIN);
  hook_mouse_event(&mouse, M1_PRESSED, (void *)on_m1_pressed);
  hook_mouse_event(&mouse, M1_RELEASED, (void *)on_m1_released);
  hook_mouse_event(&mouse, M2_PRESSED, (void *)on_m2_pressed);
  hook_mouse_event(&mouse, M2_RELEASED, (void *)on_m2_released);
  hook_mouse_event(&mouse, POINTER_MOVED, (void *)on_mouse_move);
  hook_mouse_event(&mouse, M4_PRESSED, (void *)on_m4_pressed);
  hook_mouse_event(&mouse, M4_RELEASED, (void *)on_m4_released);
  hook_mouse_event(&mouse, M5_PRESSED, (void *)on_m5_pressed);
  hook_mouse_event(&mouse, M5_RELEASED, (void *)on_m5_released);
  bleMouse.begin();
  last_frame_time = millis();
}

void loop() {
  update_storage();
  if (!bleMouse.isConnected()) {
    return;
  }
  if (!is_led_on(&mouse.on_led)) {
    led_on(&mouse.on_led);
  }
  uint32_t current_time = millis();
  delta_time = (current_time - last_frame_time) / 1000.0;
  if (delta_time < BT_REFRESH_RATE) {
    return;
  }
  update_mouse(&mouse);
  last_frame_time = current_time;
}
