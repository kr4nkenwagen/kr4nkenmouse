#include "led.h"
#include "esp32-hal-gpio.h"
#include <stdint.h>

void init_led(led_t *led, uint8_t pin) {
  pinMode(pin, OUTPUT);
  led->pin = pin;
  led->value = LOW;
  digitalWrite(led->pin, led->value);
}
void switch_led(led_t *led) {
  if (!led) {
    return;
  }
  if (led->value == HIGH) {
    led->value = LOW;
  } else {
    led->value = HIGH;
  }
  digitalWrite(led->pin, led->value);
}
void led_on(led_t *led) {
  if (!led) {
    return;
  }
  led->value = HIGH;
  digitalWrite(led->pin, led->value);
}
void led_off(led_t *led) {
  if (!led) {
    return;
  }
  led->value = LOW;
  digitalWrite(led->pin, led->value);
}

uint8_t is_led_on(led_t *led) {
  if (!led) {
    return 0;
  }
  return led->value == HIGH;
}
