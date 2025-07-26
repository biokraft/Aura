# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.1] - 2025-07-26

### Added
- **Developer Guide**: New comprehensive developer documentation in `docs/DEVELOPMENT.md`
  - Development environment setup instructions
  - Complete Makefile commands reference
  - Code quality and linting workflow
- **Documentation Structure**: Organized documentation with proper navigation
  - `docs/MANUAL_INSTALLATION.md` - Detailed manual installation guide
  - `docs/TROUBLESHOOTING.md` - Comprehensive troubleshooting guide
  - `docs/linting.md` - Code quality and linting documentation
  - `SPECS.md` - Central hub for all project specifications
- **Navigation Links**: Added "Back to" links across all documentation files for better navigation
- **Project Attribution**: Proper attribution to original Surrey-Homeware/Aura project
- **Build System**: Enhanced Makefile with automatic fallback flashing for ESP32 devices
- **CI Integration**: Added build status badge and improved CI/CD workflow support

### Changed
- **README.md**: Restructured for better user experience
  - Streamlined installation process focusing on `make install` command
  - Simplified development section with reference to dedicated Developer Guide
  - Updated project attribution and fork information
  - Improved quick start instructions
- **Makefile**: Enhanced ESP32 platform version specification (2.0.17) for better compatibility
- **Documentation Organization**: Moved detailed technical content to dedicated files
- **Specification Files**: Added consistent navigation links across all spec documents

### Improved
- **Installation Process**: Simplified user experience with single-command installation
- **Code Quality**: Enhanced linting and formatting tools integration
- **Developer Experience**: Consolidated all development-related information in one place
- **Project Navigation**: Better cross-linking between documentation files

### Fixed
- **Tool Detection**: Improved compatibility for clang-tidy and arduino-lint path detection
- **Flashing Process**: Added automatic fallback mechanism for ESP32 firmware uploads
- **Documentation Links**: Corrected internal references and navigation paths
