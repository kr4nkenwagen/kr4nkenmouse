#include "joystick.h"
#include "esp32-hal-adc.h"
#include <stdlib.h>
#include <sys/_intsup.h>

static int16_t update_joystick_axis(uint8_t pin, int16_t value) {
  int16_t v = analogRead(pin) - MOUSE_CENTER;
  float smoothed = (MOUSE_ALPHA * v) + ((1.0f - MOUSE_ALPHA) * value);
  float normalized = smoothed / (float)MOUSE_CENTER;
  float power = (normalized * normalized) * (normalized > 0 ? 1.0 : -1.0);
  return (int16_t)(power * MOUSE_CENTER);
}

joystick_t *init_joystick(uint8_t x, uint8_t y) {
  joystick_t *js = malloc(sizeof(joystick_t));
  if (!js) {
    return NULL;
  }
  js->pins.x = x;
  js->pins.y = y;
  js->position.x = 0;
  js->position.y = 0;
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
  return js;
}

void delete_joystick(joystick_t *js) { free(js); }

void update_joystick(joystick_t *js) {
  if (!js) {
    return;
  }
  js->position.x = update_joystick_axis(js->pins.x, js->position.x) * -1;
  js->position.y = update_joystick_axis(js->pins.y, js->position.y);
}
