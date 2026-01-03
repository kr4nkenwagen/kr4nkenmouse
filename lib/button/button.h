#ifndef SWITCH
#define SWITCH

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum button_state { PRESSED, DOWN, RELEASED, IDLE } button_state_t;

typedef struct button {
  uint8_t pin;
  uint8_t value;
  button_state_t state;
  uint32_t last_debounce_time;
} button_t;

#define DEBOUNCE_TIME 20

button_t *init_button(uint8_t pin);
void delete_button(button_t *btn);
void update_button(button_t *btn);
#ifdef __cplusplus
}
#endif
#endif
