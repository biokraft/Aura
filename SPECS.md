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
| `specs/06-documentation.md`       | Defines the requirements for user-facing documentation, including the README. |
| `specs/07-framework-architecture.md` | Specifies the architecture for the general-purpose Display Agent framework. |
| `specs/08-app-bus-schedule.md`    | Defines the functional and technical requirements for the Bus Schedule Agent. |

## 3. Implementation Plan

This plan outlines the key phases for transforming Aura into a general-purpose display framework and implementing the first new application.

| Phase | Focus Area                    | Key Deliverables                                                                                                                              | Related Specs                                                              | Status |
| :---- | :---------------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------- | :----- |
| 1     | **Core Framework Refactoring**  | - Refactor `weather.ino` into a modular, C++ class-based structure.<br>- Create a `DisplayAgent` abstract base class.<br>- Isolate all weather-specific logic into a `WeatherAgent` class. | `specs/07-framework-architecture.md`                                       | TBD    |
| 2     | **Remote Configuration**      | - Implement the Agent Manager to fetch and parse the remote JSON config.<br>- Add a "Config URL" field to the WiFiManager setup.<br>- Implement default fallback to the Weather Agent. | `specs/07-framework-architecture.md`                                       | TBD    |
| 3     | **App Lifecycle Management**  | - Implement the on-device agent switching logic (long-press gesture).<br>- Implement the `setup()`, `loop()`, and `teardown()` lifecycle methods in the Agent Manager and Weather Agent. | `specs/07-framework-architecture.md`                                       | TBD    |
| 4     | **Bus Schedule App: UI & Data** | - Implement the data fetching logic for the MVG API.<br>- Develop the LVGL user interface for the bus schedule as specified.<br>- Encapsulate all logic into a `BusScheduleAgent` class. | `specs/08-app-bus-schedule.md`                                             | TBD    |
| 5     | **Integration and Testing**   | - Test fetching a remote config that includes both Weather and Bus agents.<br>- Test switching between the two agents on the device.<br>- Verify all parameters are passed and used correctly. | `specs/07-framework-architecture.md`, `specs/08-app-bus-schedule.md`         | TBD    |
| 6     | **Documentation Update**      | - Update `README.md` and all relevant specs to reflect the new framework architecture, agent configuration, and the new Bus Schedule app.       | `specs/06-documentation.md`                                                | TBD    |

## 4. Open Questions / Future Work

-   **Asset Management:** The workflow for updating fonts is manual and error-prone. The project would benefit from an automated build process that integrates the `extract_unicode_chars.py` script and the LVGL font converter. Storing assets on SPIFFS instead of compiling them into the binary should also be evaluated for better scalability.
-   **Configuration Management:** The custom `User_Setup.h` and `lv_conf.h` files must be manually copied into library folders, which is fragile. Using a more robust build system like PlatformIO would allow for project-specific library configurations without modifying the original library source.
-   **Error Handling:** The application's error handling for API failures or network issues could be improved to provide more user-friendly feedback on the display instead of just logging to the serial console.
-   **Code Duplication:** The `choose_icon` and `choose_image` functions contain identical logic. This could be consolidated into a single function to reduce redundancy and improve maintainability.
-   **Extensibility:** Adding new features, such as air quality data or calendar integration, would require significant refactoring. A more modular, object-oriented architecture could facilitate future enhancements. 