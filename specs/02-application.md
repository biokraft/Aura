# Application Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document details the software architecture, dependencies, and core logic of the Aura weather widget firmware.

## 2. Platform and Frameworks

-   **Platform:** Arduino
-   **Core Framework:** ESP32 Arduino Core

## 3. Libraries and Dependencies

| Library               | Version      | Purpose                               |
| --------------------- | ------------ | ------------------------------------- |
| `ArduinoJson`         | 7.4.1        | JSON parsing for API responses        |
| `HttpClient`          | 2.2.0        | HTTP requests to weather APIs         |
| `TFT_eSPI`            | 2.5.43       | TFT display driver                    |
| `WifiManager`         | 2.0.17       | Wi-Fi connection management           |
| `XPT2046_Touchscreen` | 1.4          | Touchscreen input handling            |
| `lvgl`                | 9.2.2        | Graphics and UI                       |
| `Preferences`         | (built-in)   | Storing user settings in NVS          |

## 4. Component-Based Architecture

The application is designed around a component-based architecture, promoting modularity and separation of concerns. The core components are defined in `aura/src/components/`:

-   **`display`**: Manages the low-level interactions with the TFT screen and the LVGL graphics library.
-   **`ui`**: Responsible for building and managing all user interface elements.
-   **`weather`**: Handles all logic related to fetching, parsing, and managing weather data.

These components are orchestrated by the main `aura.ino` sketch.

## 5. Core Application Logic

### 5.1. Initialization (`setup()` in `aura.ino`)

1.  Initialize Serial and the `Preferences` library.
2.  Call the `display` component to initialize the TFT and LVGL.
3.  Load user preferences from NVS.
4.  Configure and connect to Wi-Fi using `WiFiManager`. A captive portal with SSID `Aura` is launched if credentials are not stored.
5.  Call the `ui` component to create the main user interface.
6.  Perform an initial weather data fetch via the `weather` component.

### 5.2. Main Loop (`loop()` in `aura.ino`)

1.  Call the `display` component's loop handler for LVGL task processing.
2.  Periodically trigger the `weather` component to fetch new data every 10 minutes.

### 5.3. Weather Component (`weather.cpp`/`.h`)

-   **Geocoding:**
    -   Uses `geocoding-api.open-meteo.com` to search for locations by name.
    -   Provides a function to send a GET request and parse the JSON response.
-   **Weather Forecast:**
    -   Uses `api.open-meteo.com` to fetch weather data.
    -   Constructs the API request URL with coordinates and required data fields.
    -   Provides a function to handle the API request, parse the JSON payload, and notify the `ui` component to update its elements.
    -   Sets the system time using the UTC offset from the API response.

### 5.4. Localization

-   Supports multiple languages (English, Spanish, German, French) via string data structures.
-   The `ui` component is responsible for dynamically updating on-screen text based on the selected language.
-   The `display` component manages font loading to support different character sets.

## 6. Configuration and Preferences

-   User settings are stored in NVS using the `Preferences` library under the "weather" namespace.
-   Settings include:
    -   `latitude`, `longitude`, `location` (string)
    -   `useFahrenheit` (boolean)
    -   `use24Hour` (boolean)
    -   `brightness` (uint)
    -   `language` (uint, enum `Language`)
-   Default values are defined for initial setup. 