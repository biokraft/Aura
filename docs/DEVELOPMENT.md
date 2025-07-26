# Developer Guide

This guide provides instructions and information for developers contributing to the Aura project. It covers the development environment setup, code quality tools, and available `Makefile` commands.

[⬅️ Back to Home](../README.md)

## 🚀 Development Environment Setup

The quickest way to get started is to use the automated `Makefile` target to install all required tools for development, including `arduino-cli` and the necessary linting utilities.

```bash
make setup
```

This command will:
- Install `arduino-cli`.
- Install `clang-tidy` and `clang-format` for code analysis and formatting.
- Install `arduino-lint` for Arduino-specific project checks.

**Supported Platforms:** macOS, Linux, and Windows (with WSL or Git Bash).

## 🛠️ Code Quality and Linting

We use a combination of tools to maintain code quality and a consistent style. For a detailed overview of the tools and their configuration, please refer to the [**Linting and Code Quality Guide**](./linting.md).

### Common Commands

-   **Run All Linters:**
    ```bash
    make lint
    ```
-   **Format All Code:**
    ```bash
    make format
    ```
-   **Check Formatting (for CI):**
    ```bash
    make format/check
    ```
-   **Auto-fix and Format:**
    ```bash
    make format/fix
    ```

## 📦 Makefile Commands

The `Makefile` provides a comprehensive set of commands to streamline the development workflow.

### Main Commands
| Command | Description |
|---|---|
| `make install` | Performs a complete installation: installs dependencies, configures libraries, and flashes the firmware. |
| `make compile` | Compiles the firmware without flashing it to a device. |
| `make flash` | Flashes the compiled firmware to a connected ESP32 device. |
| `make monitor` | Opens a serial monitor to view output from the device. |
| `make clean` | Removes all generated build files and temporary directories. |

### Development & Quality
| Command | Description |
|---|---|
| `make setup` | Installs all tools required for development (linters, formatters). |
| `make check-tools` | Verifies that all required development tools are installed. |
| `make lint` | Runs all linters (`clang-tidy` and `arduino-lint`). |
| `make format` | Formats all source code using `clang-format`. |
| `make format/fix`| Attempts to automatically fix issues with `clang-tidy` and then formats the code. |
| `make analyze` | Generates detailed analysis reports in the `build/` directory. |

### Font Asset Management

The UI fonts are pre-compiled. If you add new special characters (e.g., for translations), you must regenerate the font files.

1.  **Extract Characters:** Run the Python script to get a list of all unique non-ASCII characters.
    ```bash
    python3 aura/extract_unicode_chars.py aura/aura.ino
    ```
2.  **Generate New Font:** Copy the output characters into the [LVGL Font Converter](https://lvgl.io/tools/fontconverter) to generate a new C file.
3.  **Replace Font File:** Replace the old font file in the `aura/` directory with the new one. 