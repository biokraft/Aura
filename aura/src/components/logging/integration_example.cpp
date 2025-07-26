/*
 * Integration Example for Aura Logging Module
 * 
 * Add these code snippets to your main aura.ino file to test the logging system:
 */

// 1. ADD THIS TO THE TOP OF aura.ino (after other includes):
/*
#include "src/components/logging/logging.h"

// Optional: include test functions
void run_logging_tests(); // declare if using test file
*/

// 2. ADD THIS TO setup() FUNCTION IN aura.ino:
/*
void setup() {
    Serial.begin(115200);
    
    // Initialize logging system early
    logging_init();
    
    LOG_MAIN_I("Aura Weather Display starting...");
    LOG_MAIN_I("Version: 1.0");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Your existing setup code...
    
    // Optional: Run full logging test suite
    #ifdef LOGGING_TEST_MODE
    run_logging_tests();
    #endif
}
*/

// 3. ADD THIS TO loop() FUNCTION IN aura.ino FOR SERIAL COMMAND HANDLING:
/*
void loop() {
    // Handle serial commands for logging control
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("log_")) {
            logging_handle_serial_command(command.c_str());
        }
        // Handle other serial commands...
    }
    
    // Your existing loop code...
}
*/

// 4. EXAMPLE USAGE IN COMPONENTS:

// In display.cpp:
/*
#include "../logging/logging.h"

bool Display::init() {
    LOG_DISPLAY_I("Initializing display...");
    
    if (!tft.begin()) {
        LOG_DISPLAY_E("Failed to initialize TFT display");
        return false;
    }
    
    LOG_DISPLAY_I("TFT display initialized successfully");
    LOG_DISPLAY_D("Display resolution: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return true;
}
*/

// In weather.cpp:
/*
#include "../logging/logging.h"

bool Weather::fetchWeatherData() {
    LOG_WEATHER_I("Fetching weather data...");
    
    if (!isWiFiConnected()) {
        LOG_WEATHER_W("WiFi not connected, skipping weather update");
        return false;
    }
    
    String url = buildWeatherApiUrl();
    LOG_WEATHER_D("API URL: %s", url.c_str());
    
    String response = makeHttpRequest(url);
    if (response.isEmpty()) {
        LOG_WEATHER_E("Failed to fetch weather data - empty response");
        return false;
    }
    
    LOG_WEATHER_V("Raw weather response: %s", response.c_str());
    
    if (parseWeatherResponse(response)) {
        LOG_WEATHER_I("Weather data updated successfully");
        return true;
    } else {
        LOG_WEATHER_E("Failed to parse weather response");
        return false;
    }
}
*/

// 5. SERIAL COMMANDS YOU CAN TEST:
/*
Send these commands via Serial Monitor:

log_help                     - Show all available commands
log_status                   - Show current logging configuration  
log_level debug             - Set global log level to debug
log_level info              - Set global log level to info
log_component DISPLAY debug - Set DISPLAY component to debug level
log_component WEATHER verbose - Set WEATHER component to verbose level
*/

// 6. EXPECTED LOG OUTPUT FORMAT:
/*
I (1234) MAIN: Aura Weather Display starting...
I (1235) MAIN: Version: 1.0
I (1236) MAIN: Free heap: 298432 bytes, Min free: 295840 bytes
I (1450) DISPLAY: Initializing display...
I (1455) DISPLAY: TFT display initialized successfully
D (1456) DISPLAY: Display resolution: 240x320
I (2100) WEATHER: Fetching weather data...
D (2105) WEATHER: API URL: https://api.open-meteo.com/v1/forecast?...
I (2850) WEATHER: Weather data updated successfully
*/ 