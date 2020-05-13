# esp32-homekit

# What does this do?
Brings Apple Homekit Accessory Protocol(HAP) to ESP32.
Please see https://github.com/younghyunjo/esp32-homekit.git for original repo.

This fork has significantly diverged in order to extend functionality:
- Adds support for min and max range (such as temperature min and max)
- Adds support for stepped increment (such as thermostat value increment)
- `esp_http_server` replaces Mongoose to serve requests
- Uses WIFI event hooks to handle network up/down events and ensure HAP reliably stays up
- Simplified memory management where possible to mitigate memory leaks
- CMake used to build as an IDF component

## Requirements
 - https://github.com/lerebel103/esp-wolfssl
 - ESP-IDF 4.x