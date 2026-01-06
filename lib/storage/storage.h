#ifndef STORAGE
#define STORAGE

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include <stdint.h>

#define SENSITIVITY 100

void init_storage();
void update_storage();
esp_err_t get(const char *key, int8_t *out);
esp_err_t set(const char *key, int8_t value);

#ifdef __cplusplus
}
#endif
#endif
