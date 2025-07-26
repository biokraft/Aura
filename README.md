# Aura: Smart Weather Forecast Display

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/biokraft/Aura)](https://github.com/biokraft/Aura/releases/latest)
[![License: GPL v3](https://img.shields.io/badge/License-GPL_v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-ESP32-brightgreen.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![UI Library](https://img.shields.io/badge/UI-LVGL-orange.svg)](https://lvgl.io/)

> **Note:** This is a private fork for personal development. The original project is from [Surrey-Homeware/Aura](https://github.com/Surrey-Homeware/Aura).

Aura is a firmware for a DIY smart weather widget that runs on ESP32-2432S028R devices, commonly known as "Cheap Yellow Displays" (CYDs). It features a 2.8" touchscreen display, providing a rich, graphical interface for viewing current weather conditions, 7-day forecasts, and hourly outlooks.

This repository contains the complete source code and a detailed guide for building and installing the Aura firmware. The beautiful 3D-printable case was designed by @biokraft, and you can find the design and assembly instructions on [MakerWorld](https://makerworld.com/en/models/1382304-aura-smart-weather-forecast-display).

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
-   [Quick Start Installation](#quick-start-installation)
-   [Device Setup](#device-setup)
-   [Development](#development)
-   [Troubleshooting](#troubleshooting)
-   [License](#license)
-   [Credits and Acknowledgements](#credits-and-acknowledgements)

## Hardware Requirements

-   **Device:** ESP32-2432S028R (also known as a "CYD" or Cheap Yellow Display)
-   **Display:** 2.8" 240x320 ILI9341 TFT Display
-   **Touch Controller:** XPT2046
-   **Cable:** Micro-USB data cable
-   **Computer:** Windows, macOS, or Linux computer for flashing

## Quick Start Installation

This method uses a `Makefile` to automate the entire setup and installation process.

**Prerequisites:**
- **Git:** [Install Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- **Make:**
    - **macOS:** Install Xcode Command Line Tools: `xcode-select --install`
    - **Linux (Debian/Ubuntu):** `sudo apt-get install build-essential`
    - **Windows:** Use [Chocolatey](https://chocolatey.org/) (`choco install make`) or install it with Git Bash.

### 1. Clone the Repository

```bash
git clone https://github.com/seanbaufeld/Aura.git
cd Aura
```

### 2. Install and Flash

Connect your ESP32 device to your computer via USB and run:

```bash
make install
```

This single command will:
- ✅ Install Arduino CLI
- ✅ Install ESP32 board support
- ✅ Install all required libraries
- ✅ Configure library settings automatically
- ✅ Compile and flash the firmware
- ✅ Provide detailed progress updates

**Supported Platforms:** macOS, Linux, Windows (with WSL or Git Bash).

If you encounter any issues, please see the [Troubleshooting Guide](./docs/TROUBLESHOOTING.md). For a fully manual installation process, refer to the [Manual Installation Guide](./docs/MANUAL_INSTALLATION.md).

## Device Setup

After successfully flashing the firmware, your device needs initial WiFi configuration:

1.  **Power On:** Connect the device to a power source.
2.  **Connect to "Aura" Network:** On your phone or computer, connect to the WiFi network named **"Aura"**. No password is required.
3.  **Configure WiFi:** A configuration portal should open automatically. If not, open a browser and go to `http://192.168.4.1`.
4.  **Save Credentials:** Select your home WiFi network, enter the password, and click **Save**.
5.  **Restart:** The device will restart and connect to your network.

Once connected, you can set your location and customize settings by long-pressing the screen.

## Development

For developers looking to contribute to the project, a detailed guide covering environment setup, code quality tools, and the complete list of `Makefile` commands is available in the [**Developer Guide**](./docs/DEVELOPMENT.md).

## Troubleshooting

For solutions to common problems, please see the detailed [**Troubleshooting Guide**](./docs/TROUBLESHOOTING.md).

## License

The Aura firmware source code is licensed under the [GPL-3.0 License](./LICENSE).

The weather icons and images are derived from the [Google Weather Icons project by @mrdarrengriffin](https://github.com/mrdarrengriffin/google-weather-icons/tree/main/v2) and are subject to their original license terms.

## Credits and Acknowledgements

This project is based on the fantastic work of others in the open-source community.

-   **Original Project:** The foundational code comes from [Surrey-Homeware/Aura](https://github.com/Surrey-Homeware/Aura).
-   **3D Model & Fork:** The 3D-printable case and inspiration for this fork are from [@biokraft's version](https://github.com/biokraft/Aura).
-   **UI Framework:** [LVGL](https://lvgl.io/)
-   **Libraries:** `ArduinoJson`, `HttpClient`, `TFT_eSPI`, `WifiManager`, `XPT2046_Touchscreen`
-   **Inspiration & Resources:**
    -   [witnessmenow's CYD GitHub Repo](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
    -   [Random Nerd Tutorials](https://randomnerdtutorials.com/)
