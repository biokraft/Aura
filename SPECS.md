# Aura Weather Widget Specifications

## 1. Project Overview

Aura is a firmware project for a smart weather forecast display running on ESP32-based "Cheap Yellow Display" (CYD) devices. It provides a touch-enabled graphical interface to display current weather, a 7-day forecast, and an hourly forecast, with support for location search and multiple languages.

The application is built on the Arduino framework and utilizes the LVGL library for the user interface. It fetches data from the Open-Meteo public APIs for both weather forecasts and geocoding. User preferences are stored in the device's non-volatile storage.

## 2. Specification Library

This table provides a central index for all detailed specification documents.

| Filename                  | Description                                                  |
| ------------------------- | ------------------------------------------------------------ |
| `specs/01-hardware.md`    | Defines the target hardware, including the MCU, display, and pinout configurations. |
| `specs/02-application.md` | Details the software architecture, libraries, data flow, and core application logic. |
| `specs/03-ui-ux.md`       | Describes the user interface, screen layouts, interaction flows, and visual assets. |

## 3. Open Questions / Future Work

-   **Asset Management:** The current method of embedding fonts and images directly as C arrays is inefficient and makes updates difficult. A more scalable approach, such as storing assets on SPIFFS, should be considered.
-   **Error Handling:** The application's error handling for API failures or network issues could be improved to provide more user-friendly feedback.
-   **Extensibility:** Adding new features, such as air quality data or calendar integration, would require significant refactoring. A more modular architecture could facilitate future enhancements.
-   **Code Duplication:** The `choose_icon` and `choose_image` functions contain identical logic. This could be consolidated to reduce redundancy. 