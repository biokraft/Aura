# Build and Environment Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document specifies the development environment, compilation process, and tooling required to build the Aura firmware.

## 2. Development Environment

-   **IDE:** Arduino IDE (or a compatible environment like PlatformIO)
-   **Board Support:** `esp32` board package must be installed.
    -   **Device Type:** `ESP32 Dev Module`
    -   **Partition Scheme:** `Huge App (3MB No OTA/1MB SPIFFS)`

## 3. Library Dependencies

The following libraries must be installed in the Arduino environment with the specified versions:

| Library               | Version      |
| --------------------- | ------------ |
| `ArduinoJson`         | 7.4.1        |
| `HttpClient`          | 2.2.0        |
| `TFT_eSPI`            | 2.5.43       |
| `WifiManager`         | 2.0.17       |
| `XPT2046_Touchscreen` | 1.4          |
| `lvgl`                | 9.2.2        |

## 4. Custom Library Configurations

The project requires specific configuration files for the `TFT_eSPI` and `lvgl` libraries. These files are included in the repository and must be placed in their respective library installation folders, overwriting the default files.

### 4.1. `TFT_eSPI` Configuration (`TFT_eSPI/User_Setup.h`)

This file contains critical hardware-specific settings for the display controller.

-   **Driver:** `ILI9341_2_DRIVER` is enabled for the specific display model.
-   **Display Dimensions:** `TFT_WIDTH` is 240, `TFT_HEIGHT` is 320.
-   **Pin Definitions:** Defines the GPIO pins for MOSI, SCLK, CS, DC, RST, and the backlight.
-   **SPI Settings:** Sets the SPI frequency to `55000000` and enables `USE_HSPI_PORT`.
-   **Touchscreen:** Defines the touch chip select pin (`TOUCH_CS`) and sets the touch SPI frequency.

### 4.2. `lvgl` Configuration (`lvgl/src/lv_conf.h`)

This file configures the LVGL graphics library.

-   **Color Depth:** Set to `16`-bit.
-   **Memory:** Configured to use standard C library `malloc`.
-   **Tick Source:** Uses a custom tick source based on Arduino's `millis()`.
-   **Integration:** `LV_USE_TFT_ESPI` is enabled to integrate LVGL with the `TFT_eSPI` driver.
-   **Widgets & Fonts:** Enables all necessary widgets and Montserrat fonts used by the application.

## 5. Development Workflow & Tooling

### 5.1. Source Code placement

The source code folders (`aura/`, `lvgl/`, `TFT_eSPI/`) must be placed in the user's Arduino sketch directory (e.g., `~/Documents/Arduino/`).

### 5.2. Font Asset Management

The application uses pre-rendered fonts compiled into `.c` files. The multilingual support requires including specific Unicode characters in these fonts.

-   **Tool:** `aura/extract_unicode_chars.py`
-   **Purpose:** This Python script scans `weather.ino` to find all non-ASCII characters used in the string literals for different languages.
-   **Workflow:**
    1.  When adding or modifying text with special characters, run the script:
        ```bash
        python3 aura/extract_unicode_chars.py aura/weather.ino
        ```
    2.  The script outputs a list of unique characters.
    3.  This list should be used as input for the [LVGL Font Converter tool](https://lvgl.io/tools/fontconverter) to generate new C files for the required fonts.
    4.  The newly generated font files must replace the existing ones in the `aura/` directory. 