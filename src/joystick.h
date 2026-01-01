#ifndef JOYSTICK
#define JOYSTICK

#ifdef __cplusplus
extern "C" {
#endif

#include "vector2.h"

typedef struct joystick {
  vector2_t *pins;
  uint8_t sw_pin;
  uint8_t sw_val;
  vector2_t *value;
} joystick_t;

#define MOUSE_DEADZONE 1.0
#define MOUSE_CENTER 512
#define MOUSE_ALPHA 0.2

joystick_t *init_joystick(uint8_t x, uint8_t y);
void delete_joystick(joystick_t *js);
void update_joystick(joystick_t *js);
#ifdef __cplusplus
}
#endif
#endif
