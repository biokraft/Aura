# UI/UX Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document describes the user interface (UI), user experience (UX), and graphical assets for the Aura weather widget.

## 2. UI Library

-   **Library:** LVGL (Light and Versatile Graphics Library)
-   **Version:** 9.2.2

## 3. Screen Layout and Flow

The application consists of three main screens/views:
1.  **Main Weather Screen:** The default view displaying current and forecast weather.
2.  **Settings Window:** An overlay for configuring user preferences.
3.  **Location Search Window:** An overlay for finding and setting a new location.

### 3.1. Main Weather Screen

-   **Interaction:** A single click/tap anywhere on the screen opens the **Settings Window**.
-   **Layout:**
    -   **Top Left:** Large weather image (`image_*`) and current temperature.
    -   **Top Right:** "Feels like" temperature, current location name, and digital clock.
    -   **Mid-Section:** A toggleable view between the 7-day forecast and the hourly forecast.
        -   Initially shows the **7-Day Forecast**.
        -   Clicking the 7-day forecast box switches to the **Hourly Forecast**.
        -   Clicking the hourly forecast box switches back to the 7-day forecast.
-   **Components:**
    -   **Current Weather:**
        -   Large background image representing the current weather (`image_*`).
        -   Large font label for the current temperature.
        -   Smaller label for the "feels like" temperature.
    -   **7-Day Forecast View:**
        -   A list showing the next 7 days.
        -   Each row displays: Day of the week, weather icon (`icon_*`), high temperature, and low temperature.
    -   **Hourly Forecast View:**
        -   A list showing the next 7 hours.
        -   Each row displays: Hour, weather icon (`icon_*`), precipitation probability, and temperature.
    -   **Clock:** A label in the top-right corner, updated every second.

### 3.2. Settings Window (`create_settings_window()`)

-   **Activation:** Opened by tapping the main weather screen.
-   **Layout:** A modal window with various controls.
-   **Controls:**
    -   **Brightness:** A slider to control the LCD backlight brightness.
    -   **Temperature Unit:** A switch to toggle between Celsius and Fahrenheit.
    -   **Time Format:** A switch to toggle between 12-hour and 24-hour clock formats.
    -   **Language:** A dropdown menu to select the UI language (English, Spanish, German, French).
    -   **Location:** Displays the current location name.
    -   **Change Location Button:** Opens the **Location Search Window**.
    -   **Reset Wi-Fi Button:** Shows a confirmation dialog and then clears stored Wi-Fi credentials, forcing the captive portal on next boot.
    -   **Close Button:** Saves all settings (except language, which saves on change) to NVS and closes the window.

### 3.3. Location Search Window (`create_location_dialog()`)

-   **Activation:** Opened from the **Settings Window**.
-   **Layout:** A modal window for searching for a new city.
-   **Controls:**
    -   **City Text Input:** A text area to type a city name. Tapping it reveals an on-screen keyboard.
    -   **On-Screen Keyboard:** Appears when the text input is focused.
    -   **Search Results Dropdown:** Populated with location results from the geocoding API after typing in the text input.
    -   **Save Button:** Saves the selected location's coordinates and name to NVS, closes the window, and triggers a weather update.
    -   **Cancel Button:** Closes the window without saving.

## 4. Visual Assets

### 4.1. Fonts

-   **Family:** Montserrat (Latin)
-   **Sizes:** 12, 14, 16, 20, 42
-   **Source:** Pre-compiled into C files (`lv_font_*.c`).

### 4.2. Icons and Images

-   **Source:** Based on Google Weather Icons.
-   **Format:** Pre-compiled into C arrays (`icon_*.c`, `image_*.c`).
-   **Icon Set (`icon_*`):** Smaller, monochrome-style icons used for daily and hourly forecasts.
-   **Image Set (`image_*`):** Larger, detailed background images for the main current weather display.
-   **Mapping:** The `choose_icon()` and `choose_image()` functions map WMO (World Meteorological Organization) weather codes from the API response to the corresponding graphical assets. 