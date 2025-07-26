/*
 * Test file for the Aura Logging Module
 * This file can be used to verify logging functionality
 * Include this test in the main application temporarily for verification
 */

#include "logging.h"

void test_logging_basic() {
    // Initialize logging system
    logging_init();
    
    // Test all log levels for main component
    LOG_MAIN_E("This is an ERROR message from MAIN component");
    LOG_MAIN_W("This is a WARNING message from MAIN component");
    LOG_MAIN_I("This is an INFO message from MAIN component");
    LOG_MAIN_D("This is a DEBUG message from MAIN component");
    LOG_MAIN_V("This is a VERBOSE message from MAIN component");
    
    // Test different components
    LOG_DISPLAY_I("Display component initialized successfully");
    LOG_UI_I("UI component ready for user interaction");
    LOG_WEATHER_I("Weather service starting up");
    LOG_WIFI_I("WiFi manager initialized");
    LOG_CONFIG_I("Configuration loaded from preferences");
}

void test_logging_levels() {
    LOG_MAIN_I("=== Testing Log Level Changes ===");
    
    // Test setting global log level to DEBUG
    logging_set_level(ESP_LOG_DEBUG);
    LOG_MAIN_D("DEBUG level enabled - this should be visible");
    LOG_MAIN_V("VERBOSE level - this should NOT be visible");
    
    // Test setting global log level to ERROR only
    logging_set_level(ESP_LOG_ERROR);
    LOG_MAIN_E("ERROR level - this should be visible");
    LOG_MAIN_W("WARNING level - this should NOT be visible");
    LOG_MAIN_I("INFO level - this should NOT be visible");
    
    // Reset to INFO level
    logging_set_level(ESP_LOG_INFO);
    LOG_MAIN_I("INFO level restored - this should be visible");
}

void test_logging_component_levels() {
    LOG_MAIN_I("=== Testing Component-Specific Log Levels ===");
    
    // Set DISPLAY component to DEBUG level
    logging_set_component_level(TAG_DISPLAY, ESP_LOG_DEBUG);
    
    // Test that DISPLAY debug messages appear but UI debug messages don't
    LOG_DISPLAY_D("DISPLAY debug message - should be visible");
    LOG_UI_D("UI debug message - should NOT be visible");
    
    // Set UI component to ERROR only
    logging_set_component_level(TAG_UI, ESP_LOG_ERROR);
    LOG_UI_E("UI error message - should be visible");
    LOG_UI_I("UI info message - should NOT be visible");
    
    // Reset component levels
    logging_set_component_level(TAG_DISPLAY, ESP_LOG_INFO);
    logging_set_component_level(TAG_UI, ESP_LOG_INFO);
}

void test_logging_memory_info() {
    LOG_MAIN_I("=== Testing Memory Logging Utilities ===");
    LOG_MEMORY_INFO(TAG_MAIN);
    LOG_TASK_STACK_INFO(TAG_MAIN);
}

void test_logging_serial_commands() {
    LOG_MAIN_I("=== Testing Serial Commands ===");
    
    // Test help command
    logging_handle_serial_command("log_help");
    
    // Test status command
    logging_handle_serial_command("log_status");
    
    // Test log level changes via serial
    logging_handle_serial_command("log_level debug");
    LOG_MAIN_D("Debug message after serial command - should be visible");
    
    // Test component level changes via serial
    logging_handle_serial_command("log_component WEATHER verbose");
    LOG_WEATHER_V("Verbose weather message - should be visible");
    
    // Test invalid commands
    logging_handle_serial_command("log_level invalid");
    logging_handle_serial_command("log_component INVALID debug");
    
    // Reset to default
    logging_handle_serial_command("log_level info");
}

void test_logging_function_tracing() {
    LOG_MAIN_I("=== Testing Function Tracing ===");
    
    // Enable debug level to see function tracing
    logging_set_level(ESP_LOG_DEBUG);
    
    LOG_FUNCTION_ENTRY(TAG_MAIN);
    LOG_MAIN_I("Inside test function");
    LOG_FUNCTION_EXIT(TAG_MAIN);
    
    // Reset to info level
    logging_set_level(ESP_LOG_INFO);
}

// Complete test suite
void run_logging_tests() {
    LOG_MAIN_I("============================================");
    LOG_MAIN_I("       AURA LOGGING MODULE TEST SUITE      ");
    LOG_MAIN_I("============================================");
    
    test_logging_basic();
    delay(1000);
    
    test_logging_levels();
    delay(1000);
    
    test_logging_component_levels();
    delay(1000);
    
    test_logging_memory_info();
    delay(1000);
    
    test_logging_serial_commands();
    delay(1000);
    
    test_logging_function_tracing();
    
    LOG_MAIN_I("============================================");
    LOG_MAIN_I("       LOGGING TEST SUITE COMPLETED        ");
    LOG_MAIN_I("============================================");
} 