#include "joystick.h"
#include "esp32-hal-adc.h"
#include <sys/_intsup.h>

static int16_t update_joystick_axis(uint8_t pin, int16_t value) {
  int16_t v = analogRead(pin) - MOUSE_CENTER;
  float smoothed = (MOUSE_ALPHA * v) + ((1.0f - MOUSE_ALPHA) * value);
  float normalized = smoothed / (float)MOUSE_CENTER;
  float power = (normalized * normalized) * (normalized > 0 ? 1.0 : -1.0);
  return (int16_t)(power * MOUSE_CENTER);
}

void init_joystick(joystick_t *js, uint8_t x, uint8_t y) {
  js->pins.x = x;
  js->pins.y = y;
  js->position.x = 0;
  js->position.y = 0;
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
}

void update_joystick(joystick_t *js) {
  if (!js) {
    return;
  }
  js->position.x = update_joystick_axis(js->pins.x, js->position.x) * -1;
  js->position.y = update_joystick_axis(js->pins.y, js->position.y);
}
