# Linting and Code Quality Setup

This document describes the linting and code quality tools configured for the Aura project.

## Overview

The project uses two complementary linting tools:

1. **Clang-Tidy**: Static analysis for C/C++ code to detect bugs, performance issues, and style violations
2. **arduino-lint**: Arduino ecosystem-specific linting for project structure and conventions

## Quick Start

### 1. Install Tools

```bash
# Install all required tools automatically
make setup

# Or check which tools are already available
make check-tools
```

### 2. Run Linting

```bash
# Run all linters
make lint

# Run specific linters
make lint/clang-tidy
make lint/arduino
```

### 3. Format Code

```bash
# Format all source files
make format

# Check formatting without modifying files
make format/check

# Auto-fix issues and format
make format/fix
```

## Tool Installation

### Clang-Tidy and Clang-Format

**macOS (Homebrew):**
```bash
brew install llvm
```

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y clang-tidy clang-format
```

**RHEL/CentOS/Fedora:**
```bash
sudo yum install -y clang-tools-extra
# or
sudo dnf install -y clang-tools-extra
```

### arduino-lint

**With Go installed:**
```bash
go install github.com/arduino/arduino-lint@latest
```

**Download binary:**
Visit [arduino-lint releases](https://github.com/arduino/arduino-lint/releases) and download the appropriate binary for your platform.

## Configuration Files

### `.clang-tidy`

The Clang-Tidy configuration is optimized for embedded C development:

- **Enabled checks**: `bugprone-*`, `cert-*`, `clang-analyzer-*`, `performance-*`, `portability-*`, `readability-*`, `misc-*`
- **Disabled checks**: Overly strict checks that don't apply well to embedded development
- **Header filter**: Only analyzes files in the `aura/` directory

Key disabled checks:
- `readability-magic-numbers`: Common in embedded code for hardware constants
- `readability-function-cognitive-complexity`: Embedded code often has complex hardware interaction
- `cert-err34-c`: Allows ignoring return values from certain functions

### `.clang-format`

The Clang-Format configuration follows Arduino conventions:

- **Base style**: LLVM with modifications
- **Indentation**: 2 spaces (no tabs)
- **Column limit**: 100 characters
- **Brace style**: Attach (K&R style)
- **Pointer alignment**: Right (`int *ptr`)

## Available Make Targets

### Linting
- `make lint` - Run all linters
- `make lint/clang-tidy` - Run only Clang-Tidy static analysis
- `make lint/arduino` - Run only arduino-lint structure checks

### Formatting
- `make format` - Format all source files using clang-format
- `make format/check` - Check if files are properly formatted (CI-friendly)
- `make format/fix` - Auto-fix Clang-Tidy issues and format code

### Analysis and Reports
- `make analyze` - Generate detailed analysis reports in `build/`
  - `build/lint-report.txt` - Combined text report
  - `build/clang-tidy-report.html` - HTML report (requires `pip install codereport`)

### CI/CD
- `make ci/lint` - Run linting with stricter settings for CI environments

### Maintenance
- `make clean` - Remove generated files and temporary directories
- `make check-tools` - Verify all required tools are installed
- `make setup` - Install missing tools

## Integration with Development Workflow

### Pre-commit Hook

Add this to `.git/hooks/pre-commit` to run linting before commits:

```bash
#!/bin/bash
set -e

echo "Running pre-commit linting..."
make format/check
make lint
echo "✅ Pre-commit checks passed!"
```

### IDE Integration

#### Visual Studio Code

Install the following extensions:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)

Add to `.vscode/settings.json`:
```json
{
  "clang-format.executable": "clang-format",
  "editor.formatOnSave": true,
  "C_Cpp.codeAnalysis.clangTidy.enabled": true,
  "C_Cpp.codeAnalysis.clangTidy.path": "clang-tidy"
}
```

#### CLion

CLion has built-in support for Clang-Tidy and Clang-Format. Enable them in:
- **Settings > Tools > Clang-Tidy**
- **Settings > Editor > Code Style > C/C++**

## Troubleshooting

### "Command not found" Errors

If you get "command not found" for the linting tools:

1. Run `make check-tools` to see what's missing
2. Run `make setup` to install tools automatically
3. Ensure the tools are in your PATH

### macOS Path Issues

If using Homebrew on macOS and tools aren't found:

```bash
# Add LLVM to PATH
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
# or for Intel Macs:
export PATH="/usr/local/opt/llvm/bin:$PATH"
```

### Too Many Warnings

If clang-tidy reports too many issues:

1. Start with `make format/fix` to auto-fix many issues
2. Temporarily disable strict checks by editing `.clang-tidy`
3. Use `make analyze` to generate reports and prioritize fixes

### Arduino-Lint Failures

Common arduino-lint issues:

- **Missing library.properties**: Required for Arduino libraries
- **Invalid sketch structure**: Ensure `.ino` files are in correctly named directories
- **Missing README**: Arduino projects should have documentation

## Customization

### Adding New Clang-Tidy Checks

Edit `.clang-tidy` to add or remove checks:

```yaml
Checks: >
  -*,
  bugprone-*,
  your-new-check-category-*,
  -specific-check-to-disable
```

### Modifying Code Style

Edit `.clang-format` to change formatting rules:

```yaml
# Example: Change indentation to 4 spaces
IndentWidth: 4

# Example: Change brace style
BreakBeforeBraces: Allman
```

### CI/CD Configuration

For GitHub Actions, add to `.github/workflows/lint.yml`:

```yaml
name: Lint

on: [push, pull_request]

jobs:
  lint:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install tools
        run: make setup
      - name: Run linting
        run: make ci/lint
```

## Performance Notes

- **First run**: May take longer as compilation database is generated
- **Incremental runs**: Only re-analyzes changed files
- **Large files**: Consider splitting very large files for better analysis performance
- **Parallel execution**: Use `make -j4 lint` for faster execution on multi-core systems

## References

- [Clang-Tidy Documentation](https://clang.llvm.org/extra/clang-tidy/)
- [Clang-Format Style Options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- [arduino-lint Documentation](https://arduino.github.io/arduino-lint/)
- [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard) 