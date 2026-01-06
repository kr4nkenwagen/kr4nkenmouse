#include "button.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include <stdint.h>

void init_button(button_t *btn, uint8_t pin) {
  btn->pin = pin;
  btn->value = 0;
  btn->state = IDLE;
  btn->last_debounce_time = millis();
  pinMode(btn->pin, INPUT_PULLUP);
}

void update_button(button_t *btn) {
  uint32_t current_time = millis();
  uint8_t current_state = !digitalRead(btn->pin);
  if (current_state != btn->value &&
      (current_time - btn->last_debounce_time > DEBOUNCE_TIME)) {
    if (current_state && !btn->value) {
      btn->state = PRESSED;
    } else if (!current_state && btn->value) {
      btn->state = RELEASED;
    }
    btn->value = current_state;
    btn->last_debounce_time = current_time;
  } else {
    if (btn->value && btn->state == PRESSED) {
      btn->state = DOWN;
    } else if (!btn->value && btn->state == RELEASED) {
      btn->state = IDLE;
    }
  }
}
