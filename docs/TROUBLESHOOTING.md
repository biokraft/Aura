# Troubleshooting Guide

This guide helps resolve common issues during installation or device operation.

## Common Installation Issues

#### `make` command not found
If you get an error like `make: command not found`, you need to install Make on your system.
- **macOS**: `xcode-select --install`
- **Linux (Debian/Ubuntu)**: `sudo apt-get install build-essential`
- **Windows**: Use [Chocolatey](https://chocolatey.org/) (`choco install make`) or install it with Git Bash.

#### Arduino CLI Not Found by `make`
```bash
# Verify installation
arduino-cli version

# If not found, reinstall or add to PATH
export PATH=$PATH:/path/to/arduino-cli
```
The `make install` script handles installing `arduino-cli` automatically. If it fails, you may need to install it manually. See the [Manual Installation Guide](./MANUAL_INSTALLATION.md).

#### Permission Denied (Linux/macOS)
```bash
# Add user to dialout group (Linux) for serial port access
sudo usermod -a -G dialout $USER
# Log out and log back in for the change to take effect

# If flashing still fails, you may need to use sudo
sudo make flash
```

#### Library Version Conflicts
The `make install` process uses exact library versions. If you suspect a conflict from a previous manual installation:
```bash
# Uninstall a library
arduino-cli lib uninstall ArduinoJson

# Reinstall with the correct version
arduino-cli lib install "ArduinoJson@7.4.1"
```

#### ESP32 Not Recognized
1. **Check USB Cable:** Ensure it's a **data cable**, not a power-only cable.
2. **Install Drivers:** If your computer doesn't recognize the device, you may need to install USB-to-serial drivers. The most common chips are **CP210x** or **CH340**.
3. **Try a Different Port:** Some USB ports or hubs can be unreliable.
4. **Enter Boot Mode:** If flashing fails to start, hold the `BOOT` button on the ESP32, plug in the USB cable, and then release the button.

#### Compilation Errors
1. **Check Library Versions:** Ensure the exact versions from the `Makefile` are installed.
2. **Verify Configuration Files:** `make install` handles this automatically. If you tinkered manually, ensure `User_Setup.h` and `lv_conf.h` are copied correctly.
3. **Clean Build:** Remove temporary files and recompile from scratch:
   ```bash
   make clean
   make compile
   ```

## Device Issues

#### Device Won't Start WiFi Hotspot ("Aura" network)
1. **Reset Device:** Unplug and replug the device. You can also press the `RST` (Reset) button.
2. **Check Power:** Ensure the device is receiving adequate power from a reliable USB port or 5V power supply.
3. **Factory Reset:** If the device has corrupted WiFi credentials, it may fail to start the hotspot. You can force a reset by flashing the firmware again:
   ```bash
   make flash
   ```

#### Cannot Connect to WiFi During Setup
1. **Check Network Name:** Ensure the "Aura" network appears in your WiFi list.
2. **Clear Browser Cache:** Try connecting in an incognito/private browser window to avoid old caches.
3. **Manual Configuration:** After connecting to the "Aura" network, manually navigate to `http://192.168.4.1` in your browser.
4. **WiFi Signal Strength:** Ensure the home WiFi network you are trying to connect to has a strong signal.

#### Weather Data Not Loading
1. **Check Internet Connection:** Verify the device successfully connected to your WiFi network. You can check your router's client list.
2. **Set Location:** A valid location must be configured in the settings menu (long-press the screen) for the weather API to work.
3. **API Limits:** The free weather service may have rate limits. If you have many devices or are reloading frequently, you might hit a temporary limit.

## Platform-Specific Notes

#### Windows
- Use **Git Bash** or **WSL** for the best experience with the `Makefile`.
- You may need to install USB drivers manually if the device is not detected.

#### macOS
- You may need to allow unsigned drivers in `System Settings` → `Privacy & Security`.
- Use the built-in `Terminal` for all command-line operations.

#### Linux
- You must add your user to the `dialout` group to access the serial port for flashing.
- Use your system's package manager (e.g., `apt`, `dnf`, `pacman`) to install `make` if it's not already present.
- Device ports usually appear as `/dev/ttyUSB*` or `/dev/ttyACM*`. 