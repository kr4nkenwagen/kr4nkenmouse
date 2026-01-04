#include "storage.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <string.h>

#define BUFFER_SIZE 64
static const char *TAG = "NVS_STORAGE";

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

uint8_t get(const char *key) {
  nvs_handle_t handle;
  esp_err_t err;
  int8_t value = 0;
  err = nvs_open("storage", NVS_READONLY, &handle);
  if (err != ESP_OK) {
    return NULL;
  }
  err = nvs_get_i8(handle, key, &value);
  if (err != ESP_OK) {
    return NULL;
  }
  nvs_close(handle);
  return value;
}

void init_storage() {
  nvs_flash_init();
  if (!get("sensitivity")) {
    set("sensitivity", SENSITIVITY);
  }
}

void update_storage() {
  char line[BUFFER_SIZE];
  if (fgets(line, sizeof(line), stdin) != NULL) {
    if (strncmp(line, "set_senstivity=", 15) == 0) {
      int new_val = atoi(line + 9);
      set("sensitivity", new_val);
      esp_restart();
    }
  }
}
