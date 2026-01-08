#ifndef LED
#define LED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct led {
  uint8_t pin;
  volatile uint8_t value;
} led_t;

void init_led(led_t *led, uint8_t pin);
void switch_led(led_t *led);
void led_on(led_t *led);
void led_off(led_t *led);
uint8_t is_led_on(led_t *led);

#ifdef __cplusplus
}
#endif
#endif
