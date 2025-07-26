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

## 4. Core Application Logic (`weather.ino`)

### 4.1. Initialization (`setup()`)

1.  Initialize Serial, TFT display, and LVGL.
2.  Initialize the XPT2046 touchscreen controller.
3.  Load saved user preferences from Non-Volatile Storage (NVS) using the `Preferences` library:
    -   Latitude, Longitude
    -   Temperature unit (°C/°F)
    -   Time format (12/24hr)
    -   UI Language
    -   Screen brightness
4.  Configure and connect to Wi-Fi using `WiFiManager`. It starts a captive portal with SSID `Aura` if no credentials are stored.
5.  Create the main user interface.
6.  Start a timer to update the on-screen clock every second.
7.  Perform an initial fetch and display of weather data.

### 4.2. Main Loop (`loop()`)

1.  Handle LVGL timer and task processing.
2.  Periodically fetch new weather data every 10 minutes (`UPDATE_INTERVAL`).
3.  Increment the LVGL tick.

### 4.3. Weather Data Handling

-   **Geocoding:**
    -   Uses `geocoding-api.open-meteo.com` to search for locations by name.
    -   The `do_geocode_query()` function sends a GET request and parses the JSON response to populate a results list.
-   **Weather Forecast:**
    -   Uses `api.open-meteo.com` to fetch weather data.
    -   Constructs a URL with latitude, longitude, and requested data fields (current, daily, hourly).
    -   The `fetch_and_update_weather()` function handles the API request, parses the JSON payload, and updates all relevant UI elements.
    -   Sets the system time using the UTC offset provided by the weather API.

### 4.4. Localization

-   Supports multiple languages (English, Spanish, German, French) via `LocalizedStrings` structs.
-   The UI dynamically updates text based on the selected language.
-   Font selection is managed to support different character sets if needed (though currently uses a common Latin font).

## 5. Configuration and Preferences

-   User settings are stored in NVS using the `Preferences` library under the "weather" namespace.
-   Settings include:
    -   `latitude`, `longitude`, `location` (string)
    -   `useFahrenheit` (boolean)
    -   `use24Hour` (boolean)
    -   `brightness` (uint)
    -   `language` (uint, enum `Language`)
-   Default values are defined for initial setup. 