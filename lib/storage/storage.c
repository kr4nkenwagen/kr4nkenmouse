#include "storage.h"
#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "serial_handler.h"
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 32
static const char *TAG = "NVS_STORAGE";

static int parse_int8(const char *s, int8_t *out) {
  char *end;
  int16_t v;
  if (!s || !*s || !out) {
    return 0;
  }
  while (*s && isspace((unsigned char)*s)) {
    s++;
  }
  v = strtol(s, &end, 10);
  if (s == end) {
    return 0;
  }
  while (*end &&
         (isspace((unsigned char)*end) || *end == '\r' || *end == '\n')) {
    end++;
  }
  if (*end != '\0') {
    return 0;
  }
  if (v < INT8_MIN || v > INT8_MAX) {
    return 0;
  }
  *out = (int8_t)v;
  return 1;
}

esp_err_t set(const char *key, uint8_t value) {
  nvs_handle_t handle;
  esp_err_t err;
  err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    return err;
  }
  err = nvs_set_i8(handle, key, value);
  if (err != ESP_OK) {
    nvs_close(handle);
    return err;
  }
  err = nvs_commit(handle);
  nvs_close(handle);
  return err;
}

int8_t get(const char *key) {
  nvs_handle_t handle;
  esp_err_t err;
  int8_t value = 0;
  err = nvs_open("storage", NVS_READONLY, &handle);
  if (err != ESP_OK) {
    return -1;
  }
  err = nvs_get_i8(handle, key, &value);
  if (err != ESP_OK) {
    return -1;
  }
  nvs_close(handle);
  return value;
}

void init_storage() {
  nvs_flash_init();
  if (get("sensitivity") == -1) {
    set("sensitivity", SENSITIVITY);
  }
}

void parse_function(const char *key, int8_t value) {
  if (strcmp(key, "set_sensitivity") == 0) {
    set("sensitivity", value);
  }
}

void update_storage(void) {
  static char line[BUFFER_SIZE];
  static uint8_t count = 0;
  int byte;
  while ((byte = serial_read_byte()) != -1) {
    if (byte == '\r') {
      continue;
    }
    if (byte == '\n') {
      line[count < BUFFER_SIZE ? count : BUFFER_SIZE - 1] = '\0';
      count = 0;
      char *eq = strchr(line, '=');
      if (!eq) {
        continue;
      }
      *eq = '\0';
      const char *key = line;
      const char *value_str = eq + 1;
      if (!*key || !*value_str) {
        continue;
      }
      int8_t value;
      if (parse_int8(value_str, &value)) {
        parse_function(key, value);
      }
    } else {
      if (count < BUFFER_SIZE - 1) {
        line[count++] = (char)byte;
      } else {
        count = 0;
      }
    }
  }
}
