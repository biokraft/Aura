# Use bash as the shell with strict mode
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c

# Run all recipe lines in a single shell instance
.ONESHELL:

# If a rule fails, delete its target file
.DELETE_ON_ERROR:

# Disable make's built-in rules and suffix rules
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

# Project configuration
PROJECT_DIR := $(shell pwd)
AURA_DIR := $(PROJECT_DIR)/aura
BUILD_DIR := $(PROJECT_DIR)/build
TMP_DIR := $(PROJECT_DIR)/tmp

# Arduino configuration
ARDUINO_CLI ?= arduino-cli
BOARD_FQBN := esp32:esp32:esp32:PartitionScheme=huge_app
SKETCH_NAME := aura
BAUD_RATE := 115200

# Detect platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
    ARDUINO_DIR := $(HOME)/Documents/Arduino
    LIBRARIES_DIR := $(ARDUINO_DIR)/libraries
endif
ifeq ($(UNAME_S),Linux)
    PLATFORM := linux
    ARDUINO_DIR := $(HOME)/Arduino
    LIBRARIES_DIR := $(ARDUINO_DIR)/libraries
endif
ifneq (,$(findstring MINGW,$(UNAME_S)))
    PLATFORM := windows
    ARDUINO_DIR := $(HOME)/Documents/Arduino
    LIBRARIES_DIR := $(ARDUINO_DIR)/libraries
endif
ifneq (,$(findstring CYGWIN,$(UNAME_S)))
    PLATFORM := windows
    ARDUINO_DIR := $(HOME)/Documents/Arduino
    LIBRARIES_DIR := $(ARDUINO_DIR)/libraries
endif

# Library versions (must match exactly)
LIBRARIES := \
	"ArduinoJson@7.4.1" \
	"HttpClient@2.2.0" \
	"TFT_eSPI@2.5.43" \
	"WiFiManager@2.0.17" \
	"XPT2046_Touchscreen@1.4" \
	"lvgl@9.2.2"

# ESP32 Platform version (compatible with the libraries above)
ESP32_PLATFORM_VERSION := 2.0.17

# Tool configuration
CLANG_TIDY ?= clang-tidy
CLANG_FORMAT ?= clang-format
ARDUINO_LINT ?= arduino-lint

# Source files
C_SOURCES := $(wildcard $(AURA_DIR)/*.c)
CPP_SOURCES := $(wildcard $(AURA_DIR)/*.cpp)
INO_SOURCES := $(wildcard $(AURA_DIR)/*.ino)
HEADER_SOURCES := $(wildcard $(AURA_DIR)/*.h) $(wildcard $(AURA_DIR)/*.hpp)
ALL_SOURCES := $(C_SOURCES) $(CPP_SOURCES) $(INO_SOURCES) $(HEADER_SOURCES)

# Default goal
.DEFAULT_GOAL := help

##@ Help

## help: Show this help message.
help:
	@awk '/^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } /^## / { desc = substr($$0, 4); getline; if (match($$0, /^[a-zA-Z0-9_\/\-]+:/)) { target = substr($$0, 1, RLENGTH-1); printf "  \033[36m%-20s\033[0m %s\n", target, desc } } BEGIN { printf "\nUsage:\n  make \033[36m<target>\033[0m\n\nTargets:\n" }' $(MAKEFILE_LIST)
.PHONY: help

##@ Arduino Development

## install: Complete installation - Arduino CLI, libraries, configuration, and flash firmware.
install: install/arduino-cli install/esp32 install/libraries configure flash
	@echo "✅ Complete installation finished! Your Aura device should now be running."
	@echo "📱 Connect to the 'Aura' WiFi network to configure your location and WiFi settings."
.PHONY: install

## compile: Compile the firmware without flashing.
compile: check/arduino-cli $(TMP_DIR)/.libraries-installed $(TMP_DIR)/.configured
	@echo "🔨 Compiling firmware..."
	@PORT=$$($(ARDUINO_CLI) board list | grep -E "(ttyUSB|ttyACM|COM|cu\.usbserial)" | head -1 | awk '{print $$1}' || echo ""); \
	if [ -n "$$PORT" ]; then \
		$(ARDUINO_CLI) compile --fqbn $(BOARD_FQBN) --port $$PORT $(AURA_DIR); \
	else \
		$(ARDUINO_CLI) compile --fqbn $(BOARD_FQBN) $(AURA_DIR); \
	fi
	@echo "✅ Compilation complete!"
.PHONY: compile

## flash: Flash the compiled firmware to the connected ESP32 device.
flash: check/arduino-cli check/device compile
	@echo "📡 Flashing firmware to device..."
	@PORT=$$($(ARDUINO_CLI) board list | grep -E "(ttyUSB|ttyACM|COM|cu\.usbserial)" | head -1 | awk '{print $$1}'); \
	if [ -z "$$PORT" ]; then \
		echo "❌ No ESP32 device found. Please connect your device and try again."; \
		exit 1; \
	fi; \
	echo "🔌 Using port: $$PORT"; \
	$(ARDUINO_CLI) upload --fqbn $(BOARD_FQBN) --port $$PORT $(AURA_DIR)
	@echo "✅ Firmware flashed successfully!"
	@echo "💡 Disconnect and reconnect power to start the device."
.PHONY: flash

## monitor: Monitor serial output from the ESP32 device.
monitor: check/arduino-cli check/device
	@echo "📺 Starting serial monitor (Ctrl+C to exit)..."
	@PORT=$$($(ARDUINO_CLI) board list | grep -E "(ttyUSB|ttyACM|COM|cu\.usbserial)" | head -1 | awk '{print $$1}'); \
	if [ -z "$$PORT" ]; then \
		echo "❌ No ESP32 device found. Please connect your device and try again."; \
		exit 1; \
	fi; \
	echo "🔌 Monitoring port: $$PORT at $(BAUD_RATE) baud"; \
	$(ARDUINO_CLI) monitor --port $$PORT --config baudrate=$(BAUD_RATE)
.PHONY: monitor

## configure: Copy configuration files to Arduino libraries.
configure: $(TMP_DIR)/.configured
$(TMP_DIR)/.configured: $(TMP_DIR)/.libraries-installed TFT_eSPI/User_Setup.h lvgl/src/lv_conf.h
	@echo "⚙️  Configuring libraries..."
	@mkdir -p $(TMP_DIR)
	@if [ ! -d "$(LIBRARIES_DIR)" ]; then \
		echo "❌ Arduino libraries directory not found: $(LIBRARIES_DIR)"; \
		echo "💡 Please ensure Arduino CLI is properly configured."; \
		exit 1; \
	fi
	@echo "📁 Configuring TFT_eSPI..."
	@if [ -d "$(LIBRARIES_DIR)/TFT_eSPI" ]; then \
		cp "$(LIBRARIES_DIR)/TFT_eSPI/User_Setup.h" "$(LIBRARIES_DIR)/TFT_eSPI/User_Setup.h.backup" 2>/dev/null || true; \
		cp "TFT_eSPI/User_Setup.h" "$(LIBRARIES_DIR)/TFT_eSPI/User_Setup.h"; \
		echo "✅ TFT_eSPI configured"; \
	else \
		echo "❌ TFT_eSPI library not found. Run 'make install/libraries' first."; \
		exit 1; \
	fi
	@echo "📁 Configuring LVGL..."
	@if [ -d "$(LIBRARIES_DIR)/lvgl/src" ]; then \
		cp "$(LIBRARIES_DIR)/lvgl/src/lv_conf.h" "$(LIBRARIES_DIR)/lvgl/src/lv_conf.h.backup" 2>/dev/null || true; \
		cp "lvgl/src/lv_conf.h" "$(LIBRARIES_DIR)/lvgl/src/lv_conf.h"; \
		echo "✅ LVGL configured"; \
	else \
		echo "❌ LVGL library not found. Run 'make install/libraries' first."; \
		exit 1; \
	fi
	@touch $@
	@echo "✅ Library configuration complete!"
.PHONY: configure

##@ Installation Targets

## install/arduino-cli: Install Arduino CLI.
install/arduino-cli: $(TMP_DIR)/.arduino-cli-installed
$(TMP_DIR)/.arduino-cli-installed:
	@echo "🛠️  Installing Arduino CLI..."
	@mkdir -p $(TMP_DIR)
	@if command -v $(ARDUINO_CLI) >/dev/null 2>&1; then \
		echo "✅ Arduino CLI already installed: $$($(ARDUINO_CLI) version)"; \
		touch $@; \
		exit 0; \
	fi
	@echo "📦 Installing Arduino CLI for $(PLATFORM)..."
ifeq ($(PLATFORM),macos)
	@if command -v brew >/dev/null 2>&1; then \
		brew install arduino-cli; \
	else \
		echo "❌ Homebrew not found. Please install Homebrew or Arduino CLI manually."; \
		echo "💡 Visit: https://arduino.github.io/arduino-cli/installation/"; \
		exit 1; \
	fi
else ifeq ($(PLATFORM),linux)
	@if command -v snap >/dev/null 2>&1; then \
		sudo snap install arduino-cli; \
	elif command -v curl >/dev/null 2>&1; then \
		curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh; \
		sudo mv bin/arduino-cli /usr/local/bin/; \
	else \
		echo "❌ Please install arduino-cli manually for your Linux distribution."; \
		echo "💡 Visit: https://arduino.github.io/arduino-cli/installation/"; \
		exit 1; \
	fi
else ifeq ($(PLATFORM),windows)
	@echo "❌ Automated installation not supported on Windows."; \
	echo "💡 Please install Arduino CLI manually:"; \
	echo "   - Using Chocolatey: choco install arduino-cli"; \
	echo "   - Using Scoop: scoop install arduino-cli"; \
	echo "   - Or download from: https://arduino.github.io/arduino-cli/installation/"; \
	exit 1
endif
	@touch $@
	@echo "✅ Arduino CLI installation complete!"
.PHONY: install/arduino-cli

## install/esp32: Install ESP32 board support.
install/esp32: $(TMP_DIR)/.esp32-installed
$(TMP_DIR)/.esp32-installed: $(TMP_DIR)/.arduino-cli-installed
	@echo "🔧 Installing ESP32 board support..."
	@mkdir -p $(TMP_DIR)
	@$(ARDUINO_CLI) config init --dest-dir $(TMP_DIR) >/dev/null 2>&1 || true
	@$(ARDUINO_CLI) config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
	@echo "📦 Updating package index..."
	@$(ARDUINO_CLI) core update-index
	@echo "📦 Installing ESP32 platform..."
	@$(ARDUINO_CLI) core install esp32:esp32@$(ESP32_PLATFORM_VERSION)
	@touch $@
	@echo "✅ ESP32 board support installed!"
.PHONY: install/esp32

## install/libraries: Install all required Arduino libraries.
install/libraries: $(TMP_DIR)/.libraries-installed
$(TMP_DIR)/.libraries-installed: $(TMP_DIR)/.esp32-installed
	@echo "📚 Installing Arduino libraries..."
	@mkdir -p $(TMP_DIR)
	@echo "📦 Installing libraries with specific versions..."
	@for lib in $(LIBRARIES); do \
		echo "  Installing $$lib..."; \
		$(ARDUINO_CLI) lib install $$lib; \
	done
	@touch $@
	@echo "✅ All libraries installed!"
.PHONY: install/libraries

##@ Device Management

## check/device: Check if ESP32 device is connected.
check/device:
	@echo "🔍 Checking for connected ESP32 devices..."
	@DEVICES=$$($(ARDUINO_CLI) board list | grep -E "(ttyUSB|ttyACM|COM|cu\.usbserial)" || true); \
	if [ -z "$$DEVICES" ]; then \
		echo "❌ No ESP32 device found."; \
		echo "💡 Please ensure:"; \
		echo "   1. ESP32 is connected via USB"; \
		echo "   2. USB cable supports data transfer"; \
		echo "   3. ESP32 drivers are installed"; \
		echo "   4. Device permissions are correct (Linux: add user to dialout group)"; \
		exit 1; \
	else \
		echo "✅ ESP32 device(s) found:"; \
		echo "$$DEVICES"; \
	fi
.PHONY: check/device

## list/devices: List all connected Arduino-compatible devices.
list/devices: check/arduino-cli
	@echo "🔍 Scanning for connected devices..."
	@$(ARDUINO_CLI) board list
.PHONY: list/devices

## list/libraries: List installed Arduino libraries.
list/libraries: check/arduino-cli
	@echo "📚 Installed Arduino libraries:"
	@$(ARDUINO_CLI) lib list
.PHONY: list/libraries

##@ Setup and Dependencies

## setup: Install required tools for linting.
setup:
	@echo "🛠️  Setting up linting tools..."
	@if ! command -v $(CLANG_TIDY) >/dev/null 2>&1; then \
		echo "Installing clang-tidy..."; \
		if command -v brew >/dev/null 2>&1; then \
			brew install llvm; \
		elif command -v apt-get >/dev/null 2>&1; then \
			sudo apt-get update && sudo apt-get install -y clang-tidy; \
		elif command -v yum >/dev/null 2>&1; then \
			sudo yum install -y clang-tools-extra; \
		else \
			echo "❌ Please install clang-tidy manually for your platform"; \
			exit 1; \
		fi; \
	fi
	@if ! command -v $(CLANG_FORMAT) >/dev/null 2>&1; then \
		echo "Installing clang-format..."; \
		if command -v brew >/dev/null 2>&1; then \
			brew install clang-format; \
		elif command -v apt-get >/dev/null 2>&1; then \
			sudo apt-get install -y clang-format; \
		elif command -v yum >/dev/null 2>&1; then \
			sudo yum install -y clang-tools-extra; \
		else \
			echo "❌ Please install clang-format manually for your platform"; \
			exit 1; \
		fi; \
	fi
	@if ! command -v $(ARDUINO_LINT) >/dev/null 2>&1; then \
		echo "Installing arduino-lint..."; \
		if command -v curl >/dev/null 2>&1; then \
			echo "Downloading arduino-lint binary..."; \
			curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh; \
		elif command -v wget >/dev/null 2>&1; then \
			echo "Downloading arduino-lint binary..."; \
			wget -qO- https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh; \
		else \
			echo "📝 Please install arduino-lint manually:"; \
			echo "curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh"; \
			echo "Or download binary from: https://github.com/arduino/arduino-lint/releases"; \
		fi; \
	fi
	@echo "✅ Setup complete!"
.PHONY: setup

##@ Linting and Code Quality

## lint: Run all linters (clang-tidy + arduino-lint).
lint: lint/clang-tidy lint/arduino
	@echo "✅ All linting checks passed!"
.PHONY: lint

## lint/clang-tidy: Run clang-tidy static analysis on C/C++ code.
lint/clang-tidy: $(TMP_DIR)/.clang-tidy-passed
$(TMP_DIR)/.clang-tidy-passed: $(ALL_SOURCES) .clang-tidy
	@echo "🔍 Running clang-tidy static analysis..."
	@mkdir -p $(TMP_DIR)
	@if [ -z "$(ALL_SOURCES)" ]; then \
		echo "⚠️  No C/C++ source files found in $(AURA_DIR)"; \
		touch $@; \
		exit 0; \
	fi
	@# Create a minimal compile_commands.json for clang-tidy
	@echo '[' > $(TMP_DIR)/compile_commands.json
	@first=true; \
	for file in $(ALL_SOURCES); do \
		if [ "$$first" = true ]; then \
			first=false; \
		else \
			echo ',' >> $(TMP_DIR)/compile_commands.json; \
		fi; \
		echo '{' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "directory": "$(PROJECT_DIR)",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "command": "clang++ -DARDUINO=10607 -DARDUINO_ESP32_DEV -DESP32 -I$(AURA_DIR) $$file",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "file": "$$file"' >> $(TMP_DIR)/compile_commands.json; \
		echo '}' >> $(TMP_DIR)/compile_commands.json; \
	done
	@echo ']' >> $(TMP_DIR)/compile_commands.json
	@# Run clang-tidy with the compilation database
	@$(CLANG_TIDY) -p $(TMP_DIR) $(ALL_SOURCES) || (echo "❌ clang-tidy found issues. Run 'make format/fix' to auto-fix some issues." && exit 1)
	@touch $@
	@echo "✅ clang-tidy analysis passed!"
.PHONY: lint/clang-tidy

## lint/arduino: Run arduino-lint to check Arduino project structure.
lint/arduino: $(TMP_DIR)/.arduino-lint-passed
$(TMP_DIR)/.arduino-lint-passed: $(wildcard $(AURA_DIR)/*) $(wildcard *.md) $(wildcard *.txt)
	@echo "🔍 Running arduino-lint..."
	@mkdir -p $(TMP_DIR)
	@if ! command -v $(ARDUINO_LINT) >/dev/null 2>&1; then \
		echo "❌ arduino-lint not found. Run 'make setup' to install it."; \
		exit 1; \
	fi
	@$(ARDUINO_LINT) --project-type sketch $(PROJECT_DIR) || (echo "❌ arduino-lint found issues in project structure." && exit 1)
	@touch $@
	@echo "✅ arduino-lint checks passed!"
.PHONY: lint/arduino

##@ Code Formatting

## format: Format all code using clang-format.
format: $(TMP_DIR)/.format-applied
$(TMP_DIR)/.format-applied: $(ALL_SOURCES) .clang-format
	@echo "🎨 Formatting code with clang-format..."
	@mkdir -p $(TMP_DIR)
	@if [ -z "$(ALL_SOURCES)" ]; then \
		echo "⚠️  No source files found to format"; \
		touch $@; \
		exit 0; \
	fi
	@if ! command -v $(CLANG_FORMAT) >/dev/null 2>&1; then \
		echo "❌ clang-format not found. Run 'make setup' to install it."; \
		exit 1; \
	fi
	@$(CLANG_FORMAT) -i $(ALL_SOURCES)
	@touch $@
	@echo "✅ Code formatting complete!"
.PHONY: format

## format/check: Check if code formatting is correct without modifying files.
format/check:
	@echo "🔍 Checking code formatting..."
	@if [ -z "$(ALL_SOURCES)" ]; then \
		echo "⚠️  No source files found to check"; \
		exit 0; \
	fi
	@if ! command -v $(CLANG_FORMAT) >/dev/null 2>&1; then \
		echo "❌ clang-format not found. Run 'make setup' to install it."; \
		exit 1; \
	fi
	@for file in $(ALL_SOURCES); do \
		if ! $(CLANG_FORMAT) -style=file "$$file" | diff -u "$$file" - >/dev/null; then \
			echo "❌ File $$file is not properly formatted"; \
			echo "Run 'make format' to fix formatting issues"; \
			exit 1; \
		fi; \
	done
	@echo "✅ All files are properly formatted!"
.PHONY: format/check

## format/fix: Run clang-tidy with auto-fix and then format the code.
format/fix:
	@echo "🔧 Auto-fixing issues with clang-tidy..."
	@mkdir -p $(TMP_DIR)
	@if [ -z "$(ALL_SOURCES)" ]; then \
		echo "⚠️  No source files found"; \
		exit 0; \
	fi
	@# Create compilation database first
	@echo '[' > $(TMP_DIR)/compile_commands.json
	@first=true; \
	for file in $(ALL_SOURCES); do \
		if [ "$$first" = true ]; then \
			first=false; \
		else \
			echo ',' >> $(TMP_DIR)/compile_commands.json; \
		fi; \
		echo '{' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "directory": "$(PROJECT_DIR)",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "command": "clang++ -DARDUINO=10607 -DARDUINO_ESP32_DEV -DESP32 -I$(AURA_DIR) -I$(PROJECT_DIR)/lvgl/src -I$(PROJECT_DIR)/TFT_eSPI $$file",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "file": "$$file"' >> $(TMP_DIR)/compile_commands.json; \
		echo '}' >> $(TMP_DIR)/compile_commands.json; \
	done
	@echo ']' >> $(TMP_DIR)/compile_commands.json
	@# Run clang-tidy with fixes
	@$(CLANG_TIDY) -p $(TMP_DIR) -fix -fix-errors $(ALL_SOURCES) || true
	@# Then format the code
	@$(CLANG_FORMAT) -i $(ALL_SOURCES)
	@echo "✅ Auto-fix and formatting complete!"
.PHONY: format/fix

##@ Analysis and Reports

## analyze: Generate detailed analysis reports.
analyze: $(BUILD_DIR)/lint-report.txt $(BUILD_DIR)/clang-tidy-report.html
	@echo "✅ Analysis reports generated in $(BUILD_DIR)/"
.PHONY: analyze

$(BUILD_DIR)/lint-report.txt: $(ALL_SOURCES) .clang-tidy
	@echo "📊 Generating lint report..."
	@mkdir -p $(BUILD_DIR)
	@echo "Aura Project Lint Report" > $@
	@echo "========================" >> $@
	@echo "Generated: $$(date)" >> $@
	@echo "" >> $@
	@echo "Files analyzed:" >> $@
	@for file in $(ALL_SOURCES); do echo "  - $$file" >> $@; done
	@echo "" >> $@
	@echo "Clang-Tidy Results:" >> $@
	@echo "-------------------" >> $@
	@# Create compilation database
	@mkdir -p $(TMP_DIR)
	@echo '[' > $(TMP_DIR)/compile_commands.json
	@first=true; \
	for file in $(ALL_SOURCES); do \
		if [ "$$first" = true ]; then \
			first=false; \
		else \
			echo ',' >> $(TMP_DIR)/compile_commands.json; \
		fi; \
		echo '{' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "directory": "$(PROJECT_DIR)",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "command": "clang++ -DARDUINO=10607 -DARDUINO_ESP32_DEV -DESP32 -I$(AURA_DIR) -I$(PROJECT_DIR)/lvgl/src -I$(PROJECT_DIR)/TFT_eSPI $$file",' >> $(TMP_DIR)/compile_commands.json; \
		echo '  "file": "$$file"' >> $(TMP_DIR)/compile_commands.json; \
		echo '}' >> $(TMP_DIR)/compile_commands.json; \
	done
	@echo ']' >> $(TMP_DIR)/compile_commands.json
	@$(CLANG_TIDY) -p $(TMP_DIR) $(ALL_SOURCES) >> $@ 2>&1 || true
	@echo "" >> $@
	@echo "Arduino-Lint Results:" >> $@
	@echo "--------------------" >> $@
	@$(ARDUINO_LINT) --project-type sketch $(PROJECT_DIR) >> $@ 2>&1 || true

$(BUILD_DIR)/clang-tidy-report.html: $(ALL_SOURCES) .clang-tidy
	@echo "📊 Generating HTML clang-tidy report..."
	@mkdir -p $(BUILD_DIR)
	@# This requires installing codereport: pip install codereport
	@if command -v codereport >/dev/null 2>&1; then \
		mkdir -p $(TMP_DIR); \
		echo '[' > $(TMP_DIR)/compile_commands.json; \
		first=true; \
		for file in $(ALL_SOURCES); do \
			if [ "$$first" = true ]; then \
				first=false; \
			else \
				echo ',' >> $(TMP_DIR)/compile_commands.json; \
			fi; \
			echo '{' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "directory": "$(PROJECT_DIR)",' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "command": "clang++ -DARDUINO=10607 -DARDUINO_ESP32_DEV -DESP32 -I$(AURA_DIR) -I$(PROJECT_DIR)/lvgl/src -I$(PROJECT_DIR)/TFT_eSPI $$file",' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "file": "$$file"' >> $(TMP_DIR)/compile_commands.json; \
			echo '}' >> $(TMP_DIR)/compile_commands.json; \
		done; \
		echo ']' >> $(TMP_DIR)/compile_commands.json; \
		$(CLANG_TIDY) -p $(TMP_DIR) --export-fixes=$(BUILD_DIR)/fixes.yml $(ALL_SOURCES) > $(BUILD_DIR)/warnings.txt 2>&1 || true; \
		codereport --input $(BUILD_DIR)/warnings.txt --output $(BUILD_DIR)/clang-tidy-report.html --title "Aura Clang-Tidy Report"; \
	else \
		echo "<h1>Clang-Tidy Report</h1>" > $@; \
		echo "<p>Install <code>pip install codereport</code> for enhanced HTML reports.</p>" >> $@; \
		echo "<pre>" >> $@; \
		mkdir -p $(TMP_DIR); \
		echo '[' > $(TMP_DIR)/compile_commands.json; \
		first=true; \
		for file in $(ALL_SOURCES); do \
			if [ "$$first" = true ]; then \
				first=false; \
			else \
				echo ',' >> $(TMP_DIR)/compile_commands.json; \
			fi; \
			echo '{' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "directory": "$(PROJECT_DIR)",' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "command": "clang++ -DARDUINO=10607 -DARDUINO_ESP32_DEV -DESP32 -I$(AURA_DIR) -I$(PROJECT_DIR)/lvgl/src -I$(PROJECT_DIR)/TFT_eSPI $$file",' >> $(TMP_DIR)/compile_commands.json; \
			echo '  "file": "$$file"' >> $(TMP_DIR)/compile_commands.json; \
			echo '}' >> $(TMP_DIR)/compile_commands.json; \
		done; \
		echo ']' >> $(TMP_DIR)/compile_commands.json; \
		$(CLANG_TIDY) -p $(TMP_DIR) $(ALL_SOURCES) >> $@ 2>&1 || true; \
		echo "</pre>" >> $@; \
	fi

##@ CI/CD Integration

## ci/lint: Run linting suitable for CI environment.
ci/lint:
	@echo "🚀 Running CI linting checks..."
	@# Update clang-tidy config to treat warnings as errors for CI
	@sed -i.bak "s/WarningsAsErrors: ''/WarningsAsErrors: '*'/" .clang-tidy || true
	@$(MAKE) lint || (mv .clang-tidy.bak .clang-tidy 2>/dev/null || true; exit 1)
	@# Restore original config
	@mv .clang-tidy.bak .clang-tidy 2>/dev/null || true
	@echo "✅ CI linting checks passed!"
.PHONY: ci/lint

##@ Maintenance

## clean: Remove generated files and temporary directories.
clean:
	@echo "🧹 Cleaning up..."
	@rm -rf $(BUILD_DIR)/
	@rm -rf $(TMP_DIR)/
	@echo "✅ Clean complete!"
.PHONY: clean

## check: Verify that all required tools are available.
check: check/arduino-cli check/tools
	@echo "✅ All required tools are available!"
.PHONY: check

## check/arduino-cli: Check if Arduino CLI is installed and configured.
check/arduino-cli:
	@echo "🔧 Checking Arduino CLI..."
	@if ! command -v $(ARDUINO_CLI) >/dev/null 2>&1; then \
		echo "❌ Arduino CLI not found."; \
		echo "💡 Run 'make install/arduino-cli' to install it."; \
		exit 1; \
	fi
	@echo "✅ Arduino CLI: $$($(ARDUINO_CLI) version | head -1)"
.PHONY: check/arduino-cli

## check/tools: Verify that all required tools are available.
check/tools:
	@echo "🔧 Checking required tools..."
	@tools_missing=false; \
	if ! command -v $(CLANG_TIDY) >/dev/null 2>&1; then \
		echo "❌ clang-tidy not found"; \
		tools_missing=true; \
	else \
		echo "✅ clang-tidy: $$($(CLANG_TIDY) --version | head -1)"; \
	fi; \
	if ! command -v $(CLANG_FORMAT) >/dev/null 2>&1; then \
		echo "❌ clang-format not found"; \
		tools_missing=true; \
	else \
		echo "✅ clang-format: $$($(CLANG_FORMAT) --version | head -1)"; \
	fi; \
	if ! command -v $(ARDUINO_LINT) >/dev/null 2>&1; then \
		echo "❌ arduino-lint not found"; \
		tools_missing=true; \
	else \
		echo "✅ arduino-lint: $$($(ARDUINO_LINT) --version 2>/dev/null || echo 'installed')"; \
	fi; \
	if [ "$$tools_missing" = true ]; then \
		echo ""; \
		echo "Run 'make setup' to install missing tools."; \
		exit 1; \
	fi
.PHONY: check/tools 