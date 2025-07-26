# Hardware Specification

## 1. Purpose

This document specifies the hardware components and their configurations for the Aura weather widget.

## 2. Target Device

-   **Device:** ESP32-2432S028R
-   **Common Names:** "CYD" (Cheap Yellow Display)
-   **Screen:** 2.8" ILI9341 TFT Display
-   **Resolution:** 240x320 pixels

## 3. Microcontroller

-   **MCU:** ESP32 Dev Module
-   **Partition Scheme:** Huge App (3MB No OTA/1MB SPIFFS)

## 4. Peripherals and Pinout

| Peripheral          | Pin Name | GPIO |
| ------------------- | -------- | ---- |
| Touchscreen IRQ     | `T_IRQ`  | 36   |
| Touchscreen MOSI    | `T_DIN`  | 32   |
| Touchscreen MISO    | `T_OUT`  | 39   |
| Touchscreen Clock   | `T_CLK`  | 25   |
| Touchscreen CS      | `T_CS`   | 33   |
| LCD Backlight       | -        | 21   |

## 5. Components

-   **Display Controller:** ILI9341
-   **Touch Controller:** XPT2046

## 6. Wiring

The components are integrated into the ESP32-2432S028R development board. No external wiring is required for the core functionality. 