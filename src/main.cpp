#include "button.h"
#include "esp32-hal-adc.h"
#include "esp32-hal.h"
#include "joystick.h"
#include <Arduino.h>
#include <BleMouse.h>
#include <cstdint>

BleMouse bleMouse("kr4nkenmouse");

#define SENSITIVITY 100
#define REFRESH_RATE 1000 / 500
#define BT_REFRESH_RATE 0.01

static joystick_t *js = NULL;
static button_t *btn_m1 = NULL;
static float delta_time = 0;
volatile uint32_t last_frame_time = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  analogSetAttenuation(ADC_11db);
  js = init_joystick(3, 4);
  btn_m1 = init_button(6);
  bleMouse.begin();
}

void loop() {
  if (bleMouse.isConnected()) {
    uint32_t current_time = millis();
    delta_time = (current_time - last_frame_time) / 1000.0;
    update_joystick(js);
    if (delta_time >= BT_REFRESH_RATE) {
      update_button(btn_m1);
      switch (btn_m1->state) {
      case PRESSED:
        bleMouse.press(MOUSE_LEFT);
        break;
      case RELEASED:
        bleMouse.release(MOUSE_LEFT);
        break;
      case DOWN:
      case IDLE:
        break;
      }
      int16_t x = (int16_t)((js->value->x * SENSITIVITY * -1) * delta_time);
      int16_t y = (int16_t)((js->value->y * SENSITIVITY) * delta_time);

      if (x != 0 || y != 0) {
        bleMouse.move(y, x);
      }
      last_frame_time = current_time;
    }
  }
}
