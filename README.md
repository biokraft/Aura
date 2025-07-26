# Aura: Smart Weather Forecast Display

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/seanbaufeld/Aura)](https://github.com/seanbaufeld/Aura/releases/latest)
[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-ESP32-brightgreen.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![UI Library](https://img.shields.io/badge/UI-LVGL-orange.svg)](https://lvgl.io/)

Aura is a firmware for a DIY smart weather widget that runs on ESP32-2432S028R devices, commonly known as "Cheap Yellow Displays" (CYDs). It features a 2.8" touchscreen display, providing a rich, graphical interface for viewing current weather conditions, 7-day forecasts, and hourly outlooks.

This repository contains the complete source code and a detailed guide for building and installing the Aura firmware. For the 3D-printable case design and assembly instructions, please visit the complete project page on [MakerWorld](https://makerworld.com/en/models/1382304-aura-smart-weather-forecast-display).

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
-   [Quick Start (Automated)](#quick-start-automated)
-   [Manual Installation Guide](#manual-installation-guide)
    -   [Prerequisites](#prerequisites)
    -   [Option A: Arduino CLI (Recommended)](#option-a-arduino-cli-recommended)
    -   [Option B: Arduino IDE](#option-b-arduino-ide)
-   [Device Setup](#device-setup)
-   [Troubleshooting](#troubleshooting)
-   [Development](#development)
-   [License](#license)
-   [Credits and Acknowledgements](#credits-and-acknowledgements)

## Hardware Requirements

-   **Device:** ESP32-2432S028R (also known as a "CYD" or Cheap Yellow Display)
-   **Display:** 2.8" 240x320 ILI9341 TFT Display
-   **Touch Controller:** XPT2046
-   **Cable:** Micro-USB cable
-   **Computer:** Windows, macOS, or Linux computer for flashing

## Quick Start (Automated)

**Prerequisites:** Git and Make installed on your system.

The fastest way to get Aura running is using our automated Makefile:

```bash
# Clone the repository
git clone https://github.com/seanbaufeld/Aura.git
cd Aura

# Install everything and flash to device (connect your ESP32 first!)
make install

# Or just compile without flashing
make compile
```

The automated installation will:
- ✅ Install Arduino CLI
- ✅ Install ESP32 board support
- ✅ Install all required libraries
- ✅ Configure library settings automatically
- ✅ Compile and flash the firmware
- ✅ Provide detailed progress updates

**Supported Platforms:** macOS, Linux, Windows (with WSL or Git Bash)

If the automated installation doesn't work for your setup, continue with the manual installation guide below.

## Manual Installation Guide

### Prerequisites

Choose **one** of the following development environments:

#### System Requirements

- **Operating System:** Windows 10+, macOS 10.15+, or Linux
- **USB Drivers:** ESP32 drivers (usually auto-installed)
- **Available Space:** ~2GB for Arduino environment and libraries

### Option A: Arduino CLI (Recommended)

Arduino CLI provides a faster, more reliable installation process.

#### Step 1: Install Arduino CLI

**Windows:**
```bash
# Using Chocolatey
choco install arduino-cli

# Or using Scoop
scoop install arduino-cli

# Or download from: https://arduino.github.io/arduino-cli/installation/
```

**macOS:**
```bash
# Using Homebrew
brew install arduino-cli

# Or using MacPorts
sudo port install arduino-cli
```

**Linux:**
```bash
# Using snap
sudo snap install arduino-cli

# Or download binary from: https://arduino.github.io/arduino-cli/installation/
```

#### Step 2: Configure Arduino CLI

```bash
# Initialize configuration
arduino-cli config init

# Add ESP32 board index
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Update package index
arduino-cli core update-index

# Install ESP32 platform
arduino-cli core install esp32:esp32
```

#### Step 3: Install Required Libraries

```bash
# Install all required libraries with specific versions
arduino-cli lib install "ArduinoJson@7.4.1"
arduino-cli lib install "HttpClient@2.2.0"
arduino-cli lib install "TFT_eSPI@2.5.43"
arduino-cli lib install "WiFiManager@2.0.17"
arduino-cli lib install "XPT2046_Touchscreen@1.4"
arduino-cli lib install "lvgl@9.2.2"
```

#### Step 4: Configure Libraries

```bash
# Clone or download this repository
git clone https://github.com/seanbaufeld/Aura.git
cd Aura

# Find your Arduino libraries directory
# Linux/macOS: ~/Arduino/libraries/
# Windows: Documents/Arduino/libraries/

# Copy configuration files (replace paths as needed)
cp TFT_eSPI/User_Setup.h ~/Arduino/libraries/TFT_eSPI/
cp lvgl/src/lv_conf.h ~/Arduino/libraries/lvgl/src/
```

#### Step 5: Compile and Flash

```bash
# Connect your ESP32 device via USB

# Find your device port
arduino-cli board list

# Compile (replace /dev/ttyUSB0 with your port)
arduino-cli compile --fqbn esp32:esp32:esp32:PartitionScheme=huge_app --port /dev/ttyUSB0 aura

# Flash to device
arduino-cli upload --fqbn esp32:esp32:esp32:PartitionScheme=huge_app --port /dev/ttyUSB0 aura

# Monitor serial output (optional)
arduino-cli monitor --port /dev/ttyUSB0 --config baudrate=115200
```

### Option B: Arduino IDE

If you prefer a graphical interface, use the Arduino IDE.

#### Step 1: Install Arduino IDE

1. Download Arduino IDE 2.x from [arduino.cc/software](https://www.arduino.cc/en/software)
2. Install following the platform-specific instructions
3. Launch the Arduino IDE

#### Step 2: Add ESP32 Board Support

1. Go to **File** → **Preferences** (Windows/Linux) or **Arduino IDE** → **Settings** (macOS)
2. In **Additional boards manager URLs**, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click **OK**
4. Go to **Tools** → **Board** → **Boards Manager**
5. Search for "ESP32" and install **"esp32 by Espressif Systems"**

#### Step 3: Install Libraries

1. Go to **Tools** → **Manage Libraries**
2. Install each library with the **exact version** specified:

| Library Name | Exact Version | Search Term |
|--------------|---------------|-------------|
| ArduinoJson | 7.4.1 | ArduinoJson |
| HttpClient | 2.2.0 | HttpClient |
| TFT_eSPI | 2.5.43 | TFT_eSPI |
| WiFiManager | 2.0.17 | WiFiManager |
| XPT2046_Touchscreen | 1.4 | XPT2046_Touchscreen |
| lvgl | 9.2.2 | lvgl |

**Important:** Make sure to install the exact versions listed above to avoid compatibility issues.

#### Step 4: Configure Libraries

1. **Locate Arduino Libraries Folder:**
   - **Windows:** `Documents\Arduino\libraries\`
   - **macOS:** `~/Documents/Arduino/libraries/`
   - **Linux:** `~/Arduino/libraries/`

2. **Configure TFT_eSPI:**
   - Navigate to `libraries/TFT_eSPI/`
   - **Back up the original:** `cp User_Setup.h User_Setup.h.backup`
   - Copy `User_Setup.h` from this repository's `TFT_eSPI/` folder, overwriting the existing file

3. **Configure LVGL:**
   - Navigate to `libraries/lvgl/src/`
   - **Back up the original:** `cp lv_conf.h lv_conf.h.backup`
   - Copy `lv_conf.h` from this repository's `lvgl/src/` folder, overwriting the existing file

4. **Install Sketch:**
   - Copy the entire `aura/` folder to your Arduino sketches folder
   - **Windows:** `Documents\Arduino\`
   - **macOS:** `~/Documents/Arduino/`
   - **Linux:** `~/Arduino/`

#### Step 5: Compile and Upload

1. **Open the Sketch:**
   - In Arduino IDE: **File** → **Open**
   - Navigate to `Arduino/aura/weather.ino` and open it

2. **Configure Board Settings:**
   - **Tools** → **Board** → **ESP32 Arduino** → **ESP32 Dev Module**
   - **Tools** → **Partition Scheme** → **Huge APP (3MB No OTA/1MB SPIFFS)**
   - **Tools** → **Flash Size** → **4MB (32Mb)**
   - **Tools** → **CPU Frequency** → **240MHz (WiFi/BT)**

3. **Connect and Flash:**
   - Connect your ESP32 CYD via USB
   - **Tools** → **Port** → Select your device port
   - Click **Upload** (→ arrow button)

## Device Setup

After successfully flashing the firmware, your device needs initial WiFi configuration:

### First Boot Configuration

1. **Power On:** Connect the device to power (USB or external 5V)
2. **Look for WiFi Network:** The device will create a WiFi access point named **"Aura"**
3. **Connect to Aura Network:**
   - On your phone/computer, connect to the "Aura" WiFi network
   - No password required
4. **Configure WiFi:**
   - A configuration page should open automatically
   - If not, open a browser and go to `http://192.168.4.1`
   - Select your home WiFi network from the list
   - Enter your WiFi password
   - Click **Save**
5. **Device Restart:** The device will save settings and restart
6. **Automatic Connection:** Device connects to your WiFi and starts showing weather

### Using the Device

- **Touch Interface:** Tap the screen to navigate between weather views
- **Settings:** Long press on the screen to access configuration menu
- **Location Setup:** Set your location for accurate weather data
- **Customization:** Adjust units, language, and display preferences

## Troubleshooting

### Common Installation Issues

#### Arduino CLI Not Found
```bash
# Verify installation
arduino-cli version

# If not found, reinstall or add to PATH
export PATH=$PATH:/path/to/arduino-cli
```

#### Permission Denied (Linux/macOS)
```bash
# Add user to dialout group (Linux)
sudo usermod -a -G dialout $USER
# Log out and log back in

# Or use sudo for upload
sudo arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 aura
```

#### Library Version Conflicts
```bash
# Uninstall and reinstall specific version
arduino-cli lib uninstall ArduinoJson
arduino-cli lib install "ArduinoJson@7.4.1"
```

#### ESP32 Not Recognized
1. **Check USB Cable:** Ensure it's a data cable, not power-only
2. **Install Drivers:** Download CP210x or CH340 drivers if needed
3. **Try Different Port:** Some ESP32 boards require specific USB ports
4. **Boot Mode:** Hold BOOT button while plugging in USB

#### Compilation Errors
1. **Verify Library Versions:** Ensure exact versions are installed
2. **Check Configuration Files:** Verify `User_Setup.h` and `lv_conf.h` are copied correctly
3. **Clean Build:** Delete any cache files and recompile

### Device Issues

#### Device Won't Start WiFi Hotspot
1. **Reset Device:** Hold BOOT button for 10 seconds
2. **Factory Reset:** Flash firmware again
3. **Check Power:** Ensure adequate 5V power supply

#### Cannot Connect to WiFi
1. **Check Network Name:** Ensure "Aura" network appears
2. **Clear Browser Cache:** Try incognito/private browsing
3. **Manual Configuration:** Go directly to `http://192.168.4.1`
4. **WiFi Signal:** Ensure target network has strong signal

#### Weather Data Not Loading
1. **Check Internet Connection:** Verify device connects to WiFi
2. **Set Location:** Ensure valid coordinates are configured
3. **API Limits:** Weather service may have rate limits

### Platform-Specific Notes

#### Windows
- Use PowerShell or Command Prompt
- May need to install USB drivers manually
- Arduino IDE requires Windows 10 or later

#### macOS
- May need to allow unsigned drivers in System Preferences
- Use Terminal for command-line operations
- Some USB ports may work better than others

#### Linux
- Add user to `dialout` group for USB access
- Use package manager for Arduino CLI installation
- Check `/dev/ttyUSB*` or `/dev/ttyACM*` for device ports

## Development

### Font Asset Management

The UI fonts are pre-compiled into the firmware. If you modify string literals with new special characters (e.g., for translations), you must regenerate the font files.

-   **Tool:** `aura/extract_unicode_chars.py`
-   **Usage:** Run this script to get a list of all non-ASCII characters needed for the font.
    ```bash
    python3 aura/extract_unicode_chars.py aura/weather.ino
    ```
-   **Conversion:** Copy the output character list into the [LVGL Font Converter](https://lvgl.io/tools/fontconverter) to generate new C files. Replace the old font files in the `aura/` directory with the new ones.

### Building with Makefile

For developers, use the provided Makefile for automated builds:

```bash
# Install all dependencies and compile
make install

# Just compile
make compile

# Flash to device
make flash

# Clean build files
make clean

# Check installed tools
make check
```

## License

The `weather.ino` application source code is licensed under the [GPL-3.0 License](./LICENSE).

The weather icons and images are derived from the [Google Weather Icons project by @mrdarrengriffin](https://github.com/mrdarrengriffin/google-weather-icons/tree/main/v2) and are subject to their original license terms.

## Credits and Acknowledgements

This project would not have been possible without the incredible work of the open-source community.

-   **UI Framework:** [LVGL](https://lvgl.io/)
-   **Libraries:** `ArduinoJson`, `HttpClient`, `TFT_eSPI`, `WifiManager`, `XPT2046_Touchscreen`
-   **Inspiration & Resources:**
    -   [witnessmenow's CYD GitHub Repo](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
    -   [Random Nerd Tutorials](https://randomnerdtutorials.com/)
