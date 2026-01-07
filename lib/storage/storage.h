#ifndef STORAGE
#define STORAGE

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include <stdint.h>

#define DEFAULT_SENSITIVITY 100
#define DEFAULT_M1_PIN 8
#define DEFAULT_M2_PIN 7
#define DEFAULT_M4_PIN 9
#define DEFAULT_M5_PIN 10

void init_storage();
void update_storage();
esp_err_t get(const char *key, int8_t *out);
esp_err_t set(const char *key, int8_t value);

#ifdef __cplusplus
}
#endif
#endif
