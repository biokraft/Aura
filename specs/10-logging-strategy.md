# Logging Strategy Specification

## 1. Purpose

This document defines the logging strategy for the Aura project to provide developers with clear, actionable insights into the application's runtime behavior, enhancing debugging and development workflows, particularly with `make monitor`.

## 2. Guiding Principles

The logging implementation will adhere to the best practices for embedded systems:

- **Performance First**: Logging should have minimal impact on real-time operations.
- **Clarity and Context**: Logs must be easy to understand and provide sufficient context.
- **Configurability**: Log verbosity must be controllable at both compile-time and run-time.
- **Resource Efficiency**: The logging framework should be optimized for the resource-constrained environment of the ESP32.

## 3. Chosen Framework: ESP-IDF `esp_log`

The project will use the **ESP-IDF's built-in `esp_log` library**, which is the standard and most efficient logging solution for the ESP32 platform.

### Key Features and Configuration:

- **Log Version**: We will use **Log V2**, which offers improved flexibility, reduced binary size, and centralized log formatting over the legacy V1.
- **Log Levels**: The standard `esp_log` levels will be used to control verbosity:
  - `ESP_LOGE` - Error
  - `ESP_LOGW` - Warning
  - `ESP_LOGI` - Info
  - `ESP_LOGD` - Debug
  - `ESP_LOGV` - Verbose
- **Compile-Time Verbosity**: The global maximum log level will be set via `CONFIG_LOG_MAXIMUM_LEVEL` in the `sdkconfig`.
- **Runtime Verbosity**: The default runtime level will be set by `CONFIG_LOG_DEFAULT_LEVEL`. We will leverage `esp_log_level_set()` to allow dynamic, per-component log level changes at runtime, which can be triggered via a serial command for debugging.
- **Performance**: For time-sensitive operations, we will use the standard `ESP_LOGx` macros. In contexts where interrupts are disabled or flash cache is inaccessible, the `ESP_DRAM_LOGx` macros will be used sparingly.

## 4. Log Format

The standard `esp_log` format will be used, which includes:

- `L (T) TAG: message`
- `L`: A single character representing the log level (e.g., `E`, `W`, `I`).
- `T`: Milliseconds since boot.
- `TAG`: A short string identifying the source of the log message (e.g., `WiFi`, `Weather`).
- `message`: The log message itself.

Example:
`I (123) WiFi: Connecting to SSID...`

Color-coding will be enabled in `idf.py monitor` for improved readability during development.

## 5. Implementation Notes

- **Component Tagging**: Each component (e.g., `display`, `weather`) will use a unique `TAG` for its log messages to allow for targeted filtering.
- **Log Output**: All logs will be directed to the default UART, making them visible during `make monitor`.
- **No Deferred Logging**: The built-in `esp_log` handles buffering and thread safety internally. A custom deferred logging architecture is not necessary.
- **Central Component**: A central logging component is not required, as `esp_log` is globally available. Components will include `"esp_log.h"` and define their own `TAG`. 