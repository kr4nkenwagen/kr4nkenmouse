#include "ble_mouse.h"
#include <Arduino.h>
#include <BleMouse.h>
#include <cstdint>

void setup() { ble_mouse_setup(); }

void loop() { ble_mouse_loop(); }
