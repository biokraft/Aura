/*
 * Example Usage of Enhanced Aura Logging System with Timestamps
 * 
 * This file demonstrates how to use the enhanced logging system
 * that includes date and time timestamps in square brackets.
 */

#include "logging.h"
#include "esp_wifi.h"

void example_logging_usage() {
    // Initialize the logging system (this sets up timestamps too)
    logging_init();
    
    // Basic logging examples - each will include timestamp in square brackets
    LOG_MAIN_I("System startup initiated");
    LOG_MAIN_W("This is a warning message");
    LOG_MAIN_E("This is an error message");
    
    // Component-specific logging
    LOG_DISPLAY_I("Display initialization started");
    LOG_DISPLAY_D("Setting up display buffer: 240x320 pixels");
    
    LOG_WIFI_I("Starting WiFi connection");
    LOG_WIFI_W("WiFi signal strength is weak: -75 dBm");
    
    LOG_WEATHER_I("Fetching weather data from API");
    LOG_WEATHER_D("API endpoint: https://api.weather.com/current");
    
    LOG_UI_I("Loading main screen");
    LOG_UI_V("Button click detected at coordinates: (120, 80)");
    
    LOG_CONFIG_I("Loading configuration from SPIFFS");
    LOG_CONFIG_D("Config file size: 1024 bytes");
    
    // Function tracing with timestamps
    LOG_FUNCTION_ENTRY(TAG_MAIN);
    // ... function logic here ...
    LOG_FUNCTION_EXIT(TAG_MAIN);
    
    // Memory monitoring with timestamps
    LOG_MEMORY_INFO(TAG_MAIN);
    LOG_TASK_STACK_INFO(TAG_MAIN);
}

void example_time_configuration() {
    // Set timezone (affects timestamp display)
    logging_set_timezone("EST5EDT");  // Eastern Time
    // logging_set_timezone("PST8PDT");  // Pacific Time
    // logging_set_timezone("UTC");      // UTC time
    
    // Configure custom NTP server
    logging_enable_sntp("pool.ntp.org");
    
    // Check if time is properly set
    if (logging_is_time_set()) {
        LOG_MAIN_I("System time is synchronized");
    } else {
        LOG_MAIN_W("System time not yet synchronized, using uptime");
    }
}

void example_serial_commands() {
    // Examples of serial commands you can send to control logging:
    
    // Set global log level
    // logging_handle_serial_command("log_level debug");
    
    // Set component-specific log level
    // logging_handle_serial_command("log_component WIFI verbose");
    // logging_handle_serial_command("log_component DISPLAY error");
    
    // Set timezone
    // logging_handle_serial_command("log_timezone PST8PDT");
    
    // Set NTP server
    // logging_handle_serial_command("log_sntp time.google.com");
    
    // Show status
    // logging_handle_serial_command("log_status");
    
    // Show help
    // logging_handle_serial_command("log_help");
}

void example_conditional_logging() {
    static uint32_t error_count = 0;
    static uint32_t loop_counter = 0;
    
    // Example of conditional logging to avoid spam
    if (++error_count % 100 == 0) {
        LOG_MAIN_W("Error count reached %u", error_count);
    }
    
    // Log every 1000 iterations
    if (++loop_counter % 1000 == 0) {
        LOG_MAIN_D("Main loop iteration: %u", loop_counter);
        LOG_MEMORY_INFO(TAG_MAIN);
    }
}

/*
 * Expected Output Examples:
 * 
 * When time is synchronized:
 * I (12345) MAIN: [2024-01-15 14:30:45] System startup initiated
 * W (12346) MAIN: [2024-01-15 14:30:45] This is a warning message
 * E (12347) MAIN: [2024-01-15 14:30:45] This is an error message
 * I (12348) DISPLAY: [2024-01-15 14:30:45] Display initialization started
 * 
 * When time is not yet synchronized (shows uptime):
 * I (12345) MAIN: [UP:00:00:12] System startup initiated
 * W (12346) MAIN: [UP:00:00:12] This is a warning message
 * E (12347) MAIN: [UP:00:00:12] This is an error message
 * 
 * Function tracing:
 * D (12350) MAIN: [2024-01-15 14:30:45] >> example_logging_usage
 * D (12380) MAIN: [2024-01-15 14:30:45] << example_logging_usage
 * 
 * Memory information:
 * I (12385) MAIN: [2024-01-15 14:30:45] Free heap: 245760 bytes, Min free: 243520 bytes
 */ 