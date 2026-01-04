#include "event.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

event_t *init_event(uint8_t id) {
  event_t *e = (event_t *)malloc(sizeof(event_t));
  if (!e) {
    return NULL;
  }
  e->count = 0;
  e->fn_ptr = NULL;
  e->id = id;
  return e;
}

void hook_event(event_t *e, void *fn) {
  if (!e || e->count >= 254) {
    return;
  }
  e->count++;
  if (!e->fn_ptr) {
    e->fn_ptr = malloc(sizeof(void *) * e->count);
  } else {
    e->fn_ptr = realloc(e->fn_ptr, sizeof(void *) * e->count);
  }
  if (!e->fn_ptr) {
    return;
  }
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

void delete_event(event_t *e) {
  if (!e) {
    return;
  }
  if (!e->fn_ptr) {
    free(e->fn_ptr);
  }
  free(e);
}
