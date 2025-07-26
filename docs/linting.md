# 🧹 Linting and Code Quality

This document outlines the tools and workflows for maintaining code quality in the Aura project. A consistent code style and automated checks help ensure the code is readable, maintainable, and free of common bugs.

[⬅️ Back to Home](../README.md)

## Table of Contents

- [Quick Start](#-quick-start)
- [Available Tools](#-available-tools)
  - [Clang-Tidy](#clang-tidy)
  - [Clang-Format](#clang-format)
  - [arduino-lint](#arduino-lint)
- [Makefile Commands](#-makefile-commands)
- [IDE Integration](#-ide-integration)
- [Troubleshooting](#-troubleshooting)

## 🚀 Quick Start

The fastest way to manage code quality is through the provided `Makefile` targets.

### 1. Install Tools

The `setup` command installs all necessary linting and formatting tools.

```bash
make setup
```

### 2. Run Linters and Formatters

Use the `lint` and `format` commands to check and fix the entire codebase.

```bash
# Run all linters (Clang-Tidy + arduino-lint)
make lint

# Automatically format all C/C++ source files
make format
```

For more granular control, you can auto-fix many common issues found by `clang-tidy` and then format the code:

```bash
make format/fix
```

## 🛠️ Available Tools

The project leverages a combination of tools to enforce code standards.

### Clang-Tidy

A powerful static analysis tool for C/C++ code. It detects a wide range of issues, including:
- Potential bugs and logic errors
- Performance inefficiencies
- Style violations
- Portability problems

Configuration is managed in the [`.clang-tidy`](../.clang-tidy) file, which is tailored for embedded C development.

### Clang-Format

A tool to automatically format C/C++ code according to a defined style. This ensures a consistent look and feel across the entire codebase, making it easier to read and review.

The formatting rules are defined in the [`.clang-format`](../.clang-format) file, which is based on Arduino conventions with some modifications.

### arduino-lint

An Arduino-specific linter that checks for common issues related to the Arduino ecosystem, such as:
- Correct project structure
- Presence of required metadata files (`library.properties`)
- Adherence to Arduino library conventions

## 📦 Makefile Commands

The `Makefile` provides a convenient interface for all quality-related tasks.

| Command             | Description                                                              |
| ------------------- | ------------------------------------------------------------------------ |
| `make setup`        | Installs all required linting and formatting tools.                      |
| `make check-tools`  | Verifies that all required tools are installed and available in `PATH`.  |
| `make lint`         | Runs all available linters (`clang-tidy` and `arduino-lint`).            |
| `make lint/clang-tidy`| Runs only the Clang-Tidy static analyzer.                                |
| `make lint/arduino` | Runs only the arduino-lint checker.                                      |
| `make format`       | Automatically formats all C/C++ source files.                            |
| `make format/check` | Checks formatting without modifying files (ideal for CI).                |
| `make format/fix`   | Attempts to automatically fix issues found by `clang-tidy` and formats.  |
| `make analyze`      | Generates detailed analysis reports in the `build/` directory.           |
| `make ci/lint`      | Runs a stricter version of linting suitable for CI/CD pipelines.         |

## 💻 IDE Integration

For a seamless development experience, integrate the tools directly into your editor.

### Visual Studio Code

1.  **Install Extensions:**
    -   [C/C++ by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
    -   [clangd by LLVM](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)

2.  **Configure Settings:**
    Create or update `.vscode/settings.json` in your project root:
    ```json
    {
      "editor.formatOnSave": true,
      "[cpp]": {
        "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
      },
      "clangd.arguments": [
        "--compile-commands-dir=build"
      ]
    }
    ```

### CLion

CLion offers native support for both Clang-Tidy and Clang-Format.
-   Enable them in **Settings/Preferences** → **Editor** → **Code Style** → **C/C++**.
-   Configure Clang-Tidy checks in **Settings/Preferences** → **Editor** → **Inspections**.

## 🤔 Troubleshooting

If you encounter issues with the linting tools, refer to the main [**Troubleshooting Guide**](./TROUBLESHOOTING.md) for solutions to common problems like "command not found" errors or permission issues. 