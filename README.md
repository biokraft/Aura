# Aura: Smart Weather Forecast Display

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/seanbaufeld/Aura)](https://github.com/seanbaufeld/Aura/releases/latest)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-ESP32-brightgreen.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![UI Library](https://img.shields.io/badge/UI-LVGL-orange.svg)](https://lvgl.io/)

Aura is a firmware for a DIY smart weather widget that runs on ESP32-2432S028R devices, commonly known as "Cheap Yellow Displays" (CYDs). It features a 2.8" touchscreen display, providing a rich, graphical interface for viewing current weather conditions, 7-day forecasts, and hourly outlooks.

This repository contains the complete source code and a detailed guide for building and installing the Aura firmware. For the 3D-printable case design and assembly instructions, please visit the complete project page on [MakerWorld](https://makerworld.com/en/models/1382304-aura-smart-weather-forecast-display).

## Features

-   **Current Weather:** Displays temperature, "feels like" temperature, and a large, dynamic weather image.
-   **7-Day Forecast:** Shows the upcoming week's weather with high/low temperatures and icons.
-   **Hourly Forecast:** Provides a 7-hour outlook with temperature, precipitation probability, and icons.
-   **Location Search:** Find and set any location worldwide.
-   **Multi-Lingual Support:** UI available in English, Spanish, German, and French.
-   **Touch-Enabled Interface:** Easily navigate between views and settings.
-   **Configurable Settings:** Adjust brightness, temperature units (°C/°F), and time format (12/24hr).

## Table of Contents

-   [Hardware Requirements](#hardware-requirements)
-   [Installation Guide](#installation-guide)
    -   [Step 1: Set Up the Arduino IDE](#step-1-set-up-the-arduino-ide)
    -   [Step 2: Install Libraries](#step-2-install-libraries)
    -   [Step 3: Configure Libraries](#step-3-configure-libraries)
    -   [Step 4: Compile and Upload](#step-4-compile-and-upload)
    -   [Step 5: First-Time Device Setup](#step-5-first-time-device-setup)
-   [Development](#development)
    -   [Font Asset Management](#font-asset-management)
-   [License](#license)
-   [Credits and Acknowledgements](#credits-and-acknowledgements)

## Hardware Requirements

-   **Device:** ESP32-2432S028R (also known as a "CYD" or Cheap Yellow Display)
-   **Display:** 2.8" 240x320 ILI9341 TFT Display
-   **Touch Controller:** XPT2046
-   **Cable:** Micro-USB cable

## Installation Guide

This guide walks through the process of setting up your environment and flashing the Aura firmware to your device.

### Step 1: Set Up the Arduino IDE

1.  **Install Arduino IDE:** Download and install the latest version from the [official Arduino website](https://www.arduino.cc/en/software).
2.  **Install ESP32 Board Support:**
    -   In the IDE, go to `File` > `Preferences` (`Arduino IDE` > `Settings...` on macOS).
    -   In the "Additional boards manager URLs" field, enter:
        ```
        https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
        ```
    -   Go to `Tools` > `Board` > `Boards Manager...`.
    -   Search for "esp32" and install the package by Espressif Systems.

### Step 2: Install Libraries

The project requires several libraries. Install them using the Arduino Library Manager (`Tools` > `Manage Libraries...`).

| Library               | Version |
| --------------------- | ------- |
| `ArduinoJson`         | 7.4.1   |
| `HttpClient`          | 2.2.0   |
| `TFT_eSPI`            | 2.5.43  |
| `WifiManager`         | 2.0.17  |
| `XPT2046_Touchscreen` | 1.4     |
| `lvgl`                | 9.2.2   |

### Step 3: Configure Libraries

Aura requires specific configurations for `TFT_eSPI` and `lvgl`.

1.  **Download Source Code:** Clone or download this repository as a ZIP file and extract it.
2.  **Locate Arduino Libraries Folder:** Find your Arduino libraries directory (usually in `~/Documents/Arduino/libraries/`).
3.  **Copy Configuration Files:**
    -   Copy the `User_Setup.h` file from this repository's `TFT_eSPI/` folder into your installed `TFT_eSPI` library folder, overwriting the existing file.
    -   Copy the `lv_conf.h` file from this repository's `lvgl/src/` folder into your installed `lvgl/src/` library folder, overwriting the existing file.
4.  **Place Sketch Folder:** Copy the `aura/` folder from this repository into your main Arduino sketch folder (e.g., `~/Documents/Arduino/`).

### Step 4: Compile and Upload

1.  **Open Sketch:** Open the `weather.ino` file from the `aura` folder you just copied.
2.  **Configure Board Settings:**
    -   `Tools` > `Board`: Select "ESP32 Dev Module".
    -   `Tools` > `Partition Scheme`: Select "Huge App (3MB No OTA/1MB SPIFFS)".
3.  **Connect Device:** Connect your ESP32 CYD to your computer.
4.  **Select Port:** Go to `Tools` > `Port` and choose the correct serial port for your device.
5.  **Upload:** Click the "Upload" button (right arrow icon). The IDE will compile and flash the firmware.

### Step 5: First-Time Device Setup

After flashing, the device needs to be connected to your Wi-Fi.

1.  **Connect to AP:** The device will start a Wi-Fi access point named **`Aura`**. Use a phone or computer to connect to this network.
2.  **Configure Wi-Fi:** A captive portal should open automatically. If not, open a browser and go to `http://192.168.4.1`. Select your home Wi-Fi network, enter the password, and save.
3.  **Done:** The device will restart and connect to your network, then begin displaying the weather. You can now configure your location and other preferences using the on-screen touch menu.

## Development

### Font Asset Management

The UI fonts are pre-compiled into the firmware. If you modify string literals with new special characters (e.g., for translations), you must regenerate the font files.

-   **Tool:** `aura/extract_unicode_chars.py`
-   **Usage:** Run this script to get a list of all non-ASCII characters needed for the font.
    ```bash
    python3 aura/extract_unicode_chars.py aura/weather.ino
    ```
-   **Conversion:** Copy the output character list into the [LVGL Font Converter](https://lvgl.io/tools/fontconverter) to generate new C files. Replace the old font files in the `aura/` directory with the new ones.

## License

The `weather.ino` application source code is licensed under the [GPL-3.0 License](./LICENSE).

The weather icons and images are derived from the [Google Weather Icons project by @mrdarrengriffin](https://github.com/mrdarrengriffin/google-weather-icons/tree/main/v2) and are subject to their original license terms.

## Credits and Acknowledgements

This project would not have been possible without the incredible work of the open-source community.

-   **UI Framework:** [LVGL](https://lvgl.io/)
-   **Libraries:** `ArduinoJson`, `HttpClient`, `TFT_eSPI`, `WifiManager`, `XPT2046_Touchscreen`
-   **Inspiration & Resources:**
    -   [witnessmenow's CYD GitHub Repo](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
    -   [Random Nerd Tutorials](https://randomnerdtutorials.com/)
