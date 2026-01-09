#ifndef JOYSTICK
#define JOYSTICK

#ifdef __cplusplus
extern "C" {
#endif

#include "vector2.h"

typedef struct joystick {
  vector2_t pins;
  volatile vector2_t position;
} joystick_t;

#define MOUSE_DEADZONE 1.0
#define MOUSE_CENTER 512
#define MOUSE_ALPHA 0.1

void init_joystick(joystick_t *js, uint8_t x, uint8_t y);
void update_joystick(joystick_t *js);
#ifdef __cplusplus
}
#endif
#endif
