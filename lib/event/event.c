#include "event.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void init_event(event_t *e, uint8_t id) {
  e->count = 0;
  e->fn_ptr = NULL;
  e->id = id;
}

void hook_event(event_t *e, void *fn) {
  if (!e || e->count >= 254) {
    return;
  }
  if (!e->fn_ptr) {
    e->fn_ptr = malloc(sizeof(void *));
  } else {
    e->fn_ptr = realloc(e->fn_ptr, sizeof(void *) * e->count + 1);
  }
  if (!e->fn_ptr) {
    return;
  }
  e->count++;
  e->fn_ptr[e->count - 1] = fn;
}

void trigger_event(event_t *e, void **arg, uint8_t argc) {
  if (!e) {
    return;
  }
  event_data_t data = {.arg = arg, .argc = argc, .sender = e, .id = e->id};
  typedef void (*func_t)(event_data_t data);
  for (uint8_t i = 0; i < e->count; i++) {
    func_t func = (func_t)e->fn_ptr[i];
    if (func) {
      func(data);
    }
  }
}
