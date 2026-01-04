#ifndef EVENT
#define EVENT
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct event_data {
  uint8_t id;
  void *sender;
  void **arg;
  uint8_t argc;
} event_data_t;

typedef struct event {
  uint8_t id;
  uint8_t count;
  void **fn_ptr;
} event_t;

event_t *init_event(uint8_t id);
void hook_event(event_t *e, void *fn);
void trigger_event(event_t *e, void **arg, uint8_t argc);
void delete_event(event_t *e);
#ifdef __cplusplus
}
#endif
#endif
