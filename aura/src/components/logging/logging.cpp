#include "logging.h"
#include <string.h>

// Static variables for runtime log level management
static bool logging_initialized = false;

void logging_init(void) {
    if (logging_initialized) {
        return;
    }
    
    // Set default log level for all components
    esp_log_level_set("*", ESP_LOG_INFO);
    
    // Initialize component-specific log levels
    esp_log_level_set(TAG_MAIN, ESP_LOG_INFO);
    esp_log_level_set(TAG_DISPLAY, ESP_LOG_INFO);
    esp_log_level_set(TAG_UI, ESP_LOG_INFO);
    esp_log_level_set(TAG_WEATHER, ESP_LOG_INFO);
    esp_log_level_set(TAG_WIFI, ESP_LOG_INFO);
    esp_log_level_set(TAG_CONFIG, ESP_LOG_INFO);
    
    logging_initialized = true;
    
    // Log initialization complete
    ESP_LOGI(TAG_MAIN, "Logging system initialized");
    ESP_LOGI(TAG_MAIN, "Default log level: INFO");
    ESP_LOGI(TAG_MAIN, "Available log levels: ERROR, WARN, INFO, DEBUG, VERBOSE");
}

void logging_set_level(esp_log_level_t level) {
    if (!logging_initialized) {
        logging_init();
    }
    
    // Set global log level
    esp_log_level_set("*", level);
    
    const char* level_names[] = {"NONE", "ERROR", "WARN", "INFO", "DEBUG", "VERBOSE"};
    if (level <= ESP_LOG_VERBOSE) {
        ESP_LOGI(TAG_MAIN, "Global log level set to: %s", level_names[level]);
    }
}

void logging_set_component_level(const char* tag, esp_log_level_t level) {
    if (!logging_initialized) {
        logging_init();
    }
    
    esp_log_level_set(tag, level);
    
    const char* level_names[] = {"NONE", "ERROR", "WARN", "INFO", "DEBUG", "VERBOSE"};
    if (level <= ESP_LOG_VERBOSE) {
        ESP_LOGI(TAG_MAIN, "Log level for component %s set to: %s", tag, level_names[level]);
    }
}

void logging_handle_serial_command(const char* command) {
    if (!command || strlen(command) == 0) {
        return;
    }
    
    // Handle global log level commands
    if (strncmp(command, "log_level ", 10) == 0) {
        const char* level_str = command + 10;
        
        if (strcmp(level_str, "error") == 0) {
            logging_set_level(ESP_LOG_ERROR);
        } else if (strcmp(level_str, "warn") == 0) {
            logging_set_level(ESP_LOG_WARN);
        } else if (strcmp(level_str, "info") == 0) {
            logging_set_level(ESP_LOG_INFO);
        } else if (strcmp(level_str, "debug") == 0) {
            logging_set_level(ESP_LOG_DEBUG);
        } else if (strcmp(level_str, "verbose") == 0) {
            logging_set_level(ESP_LOG_VERBOSE);
        } else {
            ESP_LOGW(TAG_MAIN, "Invalid log level: %s", level_str);
            ESP_LOGI(TAG_MAIN, "Available levels: error, warn, info, debug, verbose");
        }
    }
    // Handle component-specific log level commands
    else if (strncmp(command, "log_component ", 14) == 0) {
        const char* params = command + 14;
        
        // Parse component and level (format: "log_component DISPLAY debug")
        char component[16] = {0};
        char level_str[16] = {0};
        
        if (sscanf(params, "%15s %15s", component, level_str) == 2) {
            esp_log_level_t level = ESP_LOG_INFO; // default
            
            if (strcmp(level_str, "error") == 0) level = ESP_LOG_ERROR;
            else if (strcmp(level_str, "warn") == 0) level = ESP_LOG_WARN;
            else if (strcmp(level_str, "info") == 0) level = ESP_LOG_INFO;
            else if (strcmp(level_str, "debug") == 0) level = ESP_LOG_DEBUG;
            else if (strcmp(level_str, "verbose") == 0) level = ESP_LOG_VERBOSE;
            else {
                ESP_LOGW(TAG_MAIN, "Invalid level %s for component %s", level_str, component);
                return;
            }
            
            logging_set_component_level(component, level);
        } else {
            ESP_LOGW(TAG_MAIN, "Invalid component command format");
            ESP_LOGI(TAG_MAIN, "Usage: log_component <COMPONENT> <level>");
            ESP_LOGI(TAG_MAIN, "Example: log_component DISPLAY debug");
        }
    }
    // Handle status command
    else if (strcmp(command, "log_status") == 0) {
        logging_print_status();
    }
    // Handle help command
    else if (strcmp(command, "log_help") == 0 || strcmp(command, "help") == 0) {
        ESP_LOGI(TAG_MAIN, "=== Logging Commands ===");
        ESP_LOGI(TAG_MAIN, "log_level <level>         - Set global log level");
        ESP_LOGI(TAG_MAIN, "log_component <tag> <level> - Set component log level");
        ESP_LOGI(TAG_MAIN, "log_status                - Show current log configuration");
        ESP_LOGI(TAG_MAIN, "log_help                  - Show this help");
        ESP_LOGI(TAG_MAIN, "");
        ESP_LOGI(TAG_MAIN, "Available levels: error, warn, info, debug, verbose");
        ESP_LOGI(TAG_MAIN, "Available components: MAIN, DISPLAY, UI, WEATHER, WIFI, CONFIG");
    }
}

void logging_print_status(void) {
    ESP_LOGI(TAG_MAIN, "=== Logging System Status ===");
    ESP_LOGI(TAG_MAIN, "Initialized: %s", logging_initialized ? "YES" : "NO");
    ESP_LOGI(TAG_MAIN, "ESP-IDF Log Version: 2");
    ESP_LOGI(TAG_MAIN, "");
    ESP_LOGI(TAG_MAIN, "Component Tags:");
    ESP_LOGI(TAG_MAIN, "  MAIN     - Main application");
    ESP_LOGI(TAG_MAIN, "  DISPLAY  - Display and touch handling");
    ESP_LOGI(TAG_MAIN, "  UI       - User interface logic");
    ESP_LOGI(TAG_MAIN, "  WEATHER  - Weather data fetching");
    ESP_LOGI(TAG_MAIN, "  WIFI     - WiFi connection management");
    ESP_LOGI(TAG_MAIN, "  CONFIG   - Configuration management");
    ESP_LOGI(TAG_MAIN, "");
    ESP_LOGI(TAG_MAIN, "Memory Status:");
    LOG_MEMORY_INFO(TAG_MAIN);
    ESP_LOGI(TAG_MAIN, "=== End Status ===");
} 