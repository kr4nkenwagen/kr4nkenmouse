#include <Arduino.h>

extern "C" int serial_read_byte(void) {
  if (Serial.available()) {
    return Serial.read();
  }
  return -1;
}
