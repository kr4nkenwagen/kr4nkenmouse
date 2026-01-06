#include "event.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void init_event(event_t *e, uint8_t id) {
  e->count = 0;
  e->id = id;
  for (uint8_t i = 0; i < MAX_EVENT_HANDLERS; i++) {
    e->handlers[i] = NULL;
  }
}

void hook_event(event_t *e, void *fn) {
  if (!e || !fn || e->count >= MAX_EVENT_HANDLERS) {
    return;
  }
  e->handlers[e->count++] = fn;
}
void trigger_event(event_t *e, void **arg, uint8_t argc) {
  if (!e) {
    return;
  }
  event_data_t data = {.arg = arg, .argc = argc, .sender = e, .id = e->id};
  typedef void (*func_t)(event_data_t data);
  for (uint8_t i = 0; i < e->count; i++) {
    if (e->handlers[i]) {
      e->handlers[i](data);
    }
  }
}
