# Manual Installation Guide

This guide is for users who cannot use the automated `make install` process or prefer to set up the environment manually.

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