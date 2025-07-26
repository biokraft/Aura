# Aura Weather Widget Specifications

## 1. Project Overview

Aura is a firmware project for a smart weather forecast display running on ESP32-based "Cheap Yellow Display" (CYD) devices. It provides a touch-enabled graphical interface to display current weather, a 7-day forecast, and an hourly forecast, with support for location search and multiple languages.

The application is built on the Arduino framework and utilizes the LVGL library for the user interface. It fetches data from the Open-Meteo public APIs for both weather forecasts and geocoding. User preferences are stored in the device's non-volatile storage.

## 2. Specification Library

This table provides a central index for all detailed specification documents.

| Filename                          | Description                                                                  |
| --------------------------------- | ---------------------------------------------------------------------------- |
| `specs/01-hardware.md`            | Defines the target hardware, including the MCU, display, and pinout.         |
| `specs/02-application.md`         | Details the software architecture, libraries, data flow, and core logic.     |
| `specs/03-ui-ux.md`               | Describes the user interface, screen layouts, interaction flows, and assets. |
| `specs/04-build-and-environment.md` | Specifies the development environment, build process, and required tooling.  |
| `specs/05-deployment.md`          | Outlines the firmware flashing process and first-time device setup.        |

## 3. Open Questions / Future Work

-   **Asset Management:** The workflow for updating fonts is manual and error-prone. The project would benefit from an automated build process that integrates the `extract_unicode_chars.py` script and the LVGL font converter. Storing assets on SPIFFS instead of compiling them into the binary should also be evaluated for better scalability.
-   **Configuration Management:** The custom `User_Setup.h` and `lv_conf.h` files must be manually copied into library folders, which is fragile. Using a more robust build system like PlatformIO would allow for project-specific library configurations without modifying the original library source.
-   **Error Handling:** The application's error handling for API failures or network issues could be improved to provide more user-friendly feedback on the display instead of just logging to the serial console.
-   **Code Duplication:** The `choose_icon` and `choose_image` functions contain identical logic. This could be consolidated into a single function to reduce redundancy and improve maintainability.
-   **Extensibility:** Adding new features, such as air quality data or calendar integration, would require significant refactoring. A more modular, object-oriented architecture could facilitate future enhancements. 