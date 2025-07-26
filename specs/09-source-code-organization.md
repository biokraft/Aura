# Source Code Organization Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document defines a new, modular source code structure for the `aura` sketch. The goal is to improve code organization, scalability, and maintainability by separating concerns into distinct modules and assets. This structure is designed to be compatible with the Arduino IDE build system.

## 2. Directory Structure

The `aura/` directory will be reorganized as follows:

```
aura/
├── aura.ino
├── src/
│   ├── assets/
│   │   ├── fonts/
│   │   │   └── *.c         # Font files
│   │   └── images/
│   │       ├── backgrounds/
│   │       │   └── *.c     # Full-screen weather images
│   │       └── icons/
│   │           └── *.c     # Weather condition icons
│   ├── components/
│   │   ├── display/
│   │   │   ├── display.cpp
│   │   │   └── display.h
│   │   ├── ui/
│   │   │   ├── ui.cpp
│   │   │   └── ui.h
│   │   └── weather/
│   │       ├── weather.cpp
│   │       └── weather.h
│   └── config.h
└── extract_unicode_chars.py
```

## 3. Module Descriptions

-   **`aura.ino`**: The main sketch file and entry point of the application. Its role will be reduced to initializing and coordinating the different components.
-   **`src/`**: This directory will contain all source code (`.cpp`, `.c`, `.h`) for the project, which is automatically compiled by the Arduino IDE.
-   **`src/assets/`**: Contains all static assets converted to C arrays.
    -   `fonts/`: Stores all LVGL font files.
    -   `images/backgrounds/`: Stores the large background weather condition images.
    -   `images/icons/`: Stores the smaller weather condition icons.
-   **`src/components/`**: Contains the core functional modules of the application. Each component should have a header (`.h`) and implementation (`.cpp`) file.
    -   `display/`: Manages the TFT screen, LVGL library initialization, and rendering.
    -   `ui/`: Responsible for creating and managing all UI elements (screens, labels, images).
    -   `weather/`: Handles weather data acquisition from the API, parsing, and state management.
-   **`src/config.h`**: A central header for application-wide constants, configuration flags, and global settings.
-   **`extract_unicode_chars.py`**: The existing Python script for extracting non-ASCII characters for font generation. Its location remains unchanged for now.

## 4. Refactoring Guidelines

1.  **Asset Migration**:
    -   All `image_*.c` files shall be moved to `src/assets/images/backgrounds/`.
    -   All `icon_*.c` files shall be moved to `src/assets/images/icons/`.
    -   All `lv_font_*.c` files shall be moved to `src/assets/fonts/`.
2.  **Component Refactoring**:
    -   The logic currently in `aura.ino` should be broken down and moved into the respective components (`display`, `ui`, `weather`).
    -   `aura.ino` should primarily contain the `setup()` and `loop()` functions, which will delegate calls to the various components.
    -   Global variables and constants should be moved into `config.h` or scoped within their relevant components.

## 5. Build System Impact

This new structure is fully compatible with the standard Arduino IDE build process. The IDE will automatically discover and compile all `.c` and `.cpp` files within the `src/` directory and its subdirectories. No changes are required to the build configuration defined in `04-build-and-environment.md`. 