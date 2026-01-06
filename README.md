# ESP32 BLE Mouse (Joystick + Buttons)

An ESP32-based Bluetooth Low Energy (BLE) mouse implementation using an analog joystick for pointer movement and physical buttons for mouse actions.
The project is designed with a clean, modular C architecture, an event-driven input system, and persistent configuration storage using ESP32 NVS.

---

## Features

- BLE HID mouse using the BleMouse library
- Analog joystick for pointer movement
- Multiple buttons (Left, Right, Back, Forward)
- Event-driven input handling
- Software debouncing for buttons
- Persistent configuration using ESP32 NVS
- Runtime configuration via serial commands
- Modular and reusable C components

---

## Project Structure

.
├── button.c / button.h # Button handling with debouncing
├── joystick.c / joystick.h # Analog joystick processing and smoothing
├── mouse.c / mouse.h # Mouse abstraction and event mapping
├── event.c / event.h # Lightweight event system
├── storage.c / storage.h # NVS-backed persistent storage
├── main.cpp # Application entry point

---

## Input Mapping

| Input        | Function          |
|-------------|-------------------|
| Joystick X/Y | Mouse movement    |
| Button 1     | Left click        |
| Button 2     | Right click       |
| Back button  | Browser back      |
| Forward btn  | Browser forward   |

---

## Architecture Overview

### Event System
- Each input action triggers a typed event
- Multiple handlers can be registered per event
- Decouples hardware input from application logic

### Button Handling
- Software debouncing based on `millis()`
- State machine: `PRESSED`, `DOWN`, `RELEASED`, `IDLE`

### Joystick Processing
- Exponential smoothing filter
- Deadzone handling and non-linear scaling for better control

### Storage (NVS)
- Persistent configuration using ESP32 NVS
- Sensitivity stored across reboots
- Runtime configuration through serial commands

---

## Configuration

### Default Sensitivity

Defined at build time:
```c
#define SENSITIVITY 100
```

Change Sensitivity at Runtime
Using a serial terminal:

set_sensitivity=80
The value is written to NVS and restored on boot.

Main Loop Behavior
Non-blocking serial input handling

Button and joystick state updates

Event dispatching

BLE mouse reports sent at a fixed refresh rate

Dependencies
Arduino framework for ESP32

BleMouse library

ESP-IDF NVS (nvs.h, nvs_flash.h)
