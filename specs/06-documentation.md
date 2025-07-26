# Documentation Specification

## 1. Purpose

This document specifies the requirements for user-facing documentation for the Aura project. The primary goal is to provide clear, comprehensive, and accessible information for end-users and developers.

## 2. Target Audience

-   **End-Users/Makers:** Individuals who want to build and use the Aura weather widget.
-   **Developers:** Individuals who want to contribute to the firmware development or customize it.

## 3. Core Documentation: README.md

The `README.md` file is the central piece of documentation. It must be structured to serve both audiences effectively.

### 3.1. Required Sections

1.  **Project Title:** "Aura: Smart Weather Forecast Display".
2.  **Badges:**
    -   Must include shields for License (GPL-3.0), Platform (ESP32), Framework (Arduino), and UI Library (LVGL).
3.  **Introduction:**
    -   A brief, engaging summary of the project.
    -   Mention of the target hardware (ESP32-2432S028R / CYD).
    -   A link to the 3D-printable case and full project on MakerWorld.
4.  **Features:** A bulleted list outlining key capabilities (e.g., Current/7-Day/Hourly Forecast, Location Search, etc.).
5.  **Table of Contents:** For easy navigation.
6.  **Hardware Requirements:** A list of all necessary hardware components.
7.  **Installation Guide:** A step-by-step guide covering:
    -   Arduino IDE setup and ESP32 board support installation.
    -   A table of required libraries and their versions.
    -   Instructions for copying the custom `User_Setup.h` and `lv_conf.h` configuration files.
    -   Instructions for placing the `aura` sketch folder.
    -   Board configuration in the IDE (Device Type, Partition Scheme).
    -   The final compile and upload process.
8.  **First-Time Device Setup:**
    -   A clear guide on the Wi-Fi captive portal process (`Aura` SSID, fallback IP `http://192.168.4.1`).
9.  **Development Section:**
    -   Must include a subsection on "Font Asset Management" explaining the purpose of the `extract_unicode_chars.py` script and the workflow for updating fonts.
10. **License:**
    -   Clear statement of the firmware's license (GPL-3.0).
    -   A note on the license for the graphical assets.
11. **Credits and Acknowledgements:**
    -   A section thanking the creators of key libraries, frameworks, and resources.

## 4. Other Documentation

-   **Source Code Comments:** Code should be reasonably commented to explain complex logic, especially within `weather.ino`.
-   **Specification Library:** The `specs/` directory and its `SPECS.md` index serve as the authoritative technical reference for the project's architecture and design. 