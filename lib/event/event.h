#ifndef EVENT
#define EVENT
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_EVENT_HANDLERS 8

typedef struct event_data {
  uint8_t id;
  void *sender;
  void **arg;
  uint8_t argc;
} event_data_t;

typedef struct event {
  uint8_t id;
  uint8_t count;
  void (*handlers[MAX_EVENT_HANDLERS])(event_data_t);
} event_t;

void init_event(event_t *e, uint8_t id);
void hook_event(event_t *e, void *fn);
void trigger_event(event_t *e, void **arg, uint8_t argc);
#ifdef __cplusplus
}
#endif
#endif
