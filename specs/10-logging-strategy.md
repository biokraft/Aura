# Logging Strategy Specification

## 1. Purpose

This document defines the logging strategy for the Aura project to provide developers with clear, actionable insights into the application's runtime behavior, enhancing debugging and development workflows, particularly with `make watch`.

## 2. Guiding Principles

The logging implementation will adhere to the best practices for embedded systems:

- **Performance First**: Logging should have minimal impact on real-time operations.
- **Clarity and Context**: Logs must be easy to understand and provide sufficient context.
- **Configurability**: Log verbosity must be controllable at both compile-time and run-time.

## 3. Architecture: Deferred Logging

To meet performance requirements, a **deferred logging** architecture will be implemented.

- **Capture**: In high-frequency or time-sensitive code (like the main loop or event handlers), log calls will only capture essential data (e.g., a message ID, timestamp, and parameters) and place it into a lightweight, lock-free ring buffer. This operation will be extremely fast and non-blocking.
- **Processing**: A low-priority background task will be responsible for retrieving log data from the buffer, formatting it into human-readable strings, and writing it to the serial output.

## 4. Chosen Framework

Given the ESP32 platform and the need for a balance between performance and features, **`spdlog`** will be integrated and configured for asynchronous (deferred) logging.

- It provides a type-safe API using `{fmt}`.
- It has built-in support for asynchronous logging via a thread pool and queue.
- It is highly configurable and well-supported.

If `spdlog` proves to be too resource-intensive, a custom C++ deferred logger based on the principles in the `.cursor/rules/embedded-logging/` will be implemented as a fallback.

## 5. Logging Levels

The following standard log levels will be used:

| Level   | Description                               | Use Case Example                    |
|---------|-------------------------------------------|-------------------------------------|
| `ERROR` | Critical system failures                  | Wi-Fi connection failed after retries |
| `WARN`  | Unexpected but recoverable events         | API request timeout, will retry     |
| `INFO`  | High-level application flow               | System initialized, Entering new state |
| `DEBUG` | Detailed information for developers       | API response payload, function entry  |

The default compile-time level will be `INFO`. The default runtime level will also be `INFO`, but it will be configurable via the serial console.

## 6. Log Format

The standard log format will be:
`[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v`

- `[%Y-%m-%d %H:%M:%S.%e]`: Timestamp with milliseconds
- `[%l]`: Log level (e.g., `info`, `error`)
- `[%n]`: Logger name (e.g., `WiFi`, `WeatherAPI`)
- `%v`: The log message

Example:
`[2023-10-27 10:30:05.123] [info] [WiFi] Connecting to SSID...`

## 7. Implementation Notes

- A central logging component will be created in `src/components/logging/`.
- Individual components (e.g., `display`, `weather`) will be provided with their own named logger instances (e.g., `display_logger`).
- Log output will be directed to `Serial`, making it visible during `make watch`. 