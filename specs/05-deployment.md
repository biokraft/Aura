# Deployment Specification

## 1. Purpose

This document outlines the procedure for compiling and deploying the Aura firmware to a compatible ESP32 device, as well as the initial on-device configuration.

## 2. Prerequisites

-   A fully configured development environment as specified in `specs/04-build-and-environment.md`.
-   A compatible ESP32-2432S028R device.
-   A micro-USB cable for connecting the device to a computer.

## 3. Flashing Procedure (Arduino IDE)

1.  **Connect Device:** Connect the ESP32 device to the computer using the micro-USB cable.
2.  **Select Port:** In the Arduino IDE, navigate to `Tools` > `Port` and select the correct serial port for the ESP32.
3.  **Compile and Upload:**
    -   Open the `weather.ino` sketch.
    -   Click the "Upload" button (the arrow icon) in the Arduino IDE toolbar.
    -   The IDE will compile the sketch and upload the binary to the device.
    -   Monitor the output console for progress and confirmation of a successful upload. The device may need to be manually put into bootloader mode by holding the `BOOT` button if the upload fails.

## 4. First-Time Setup (On-Device)

Once the firmware is flashed, the device will boot up and require initial configuration. This process is handled by the `WiFiManager` library.

1.  **Captive Portal:** The device will start a Wi-Fi Access Point (AP) with the SSID **`Aura`**. The screen will display instructions to connect to this network.
2.  **Connect to AP:** Using a smartphone or laptop, connect to the "Aura" Wi-Fi network.
3.  **Configure Wi-Fi:**
    -   After connecting, a captive portal page should automatically open in a web browser.
    -   If it does not, manually navigate to `http://192.168.4.1`.
    -   On this page, select your local Wi-Fi network (SSID) and enter its password.
    -   Save the credentials.
4.  **Reboot and Connect:** The ESP32 will save the credentials to its non-volatile storage, reboot, and automatically connect to the configured Wi-Fi network.
5.  **Initial Operation:** Once connected to the internet, the device will fetch weather data for the default location (London) and display the main weather screen. From here, the user can configure their specific location and other preferences via the on-screen settings menu. 