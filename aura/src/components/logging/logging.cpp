#include "logging.h"
#include <string.h>
#include <stdio.h>

// Static variables for runtime log level management
static bool logging_initialized = false;
static bool time_initialized = false;
static char current_timezone[32] = "UTC";

// Default NTP servers
static const char* default_ntp_servers[] = {
    "pool.ntp.org",
    "time.nist.gov",
    "time.google.com"
};

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
    
    // Initialize time system
    logging_init_time();
    
    logging_initialized = true;
    
    // Log initialization complete
    LOG_MAIN_I("Logging system initialized with timestamp support");
    LOG_MAIN_I("Default log level: INFO");
    LOG_MAIN_I("Available log levels: ERROR, WARN, INFO, DEBUG, VERBOSE");
    LOG_MAIN_I("Timestamp format: %s", AURA_LOG_TIMESTAMP_FORMAT_DATE_TIME ? "Date and Time" : "Time Only");
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
            LOG_MAIN_W("Invalid log level: %s", level_str);
            LOG_MAIN_I("Available levels: error, warn, info, debug, verbose");
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
                LOG_MAIN_W("Invalid level %s for component %s", level_str, component);
                return;
            }
            
            logging_set_component_level(component, level);
        } else {
            LOG_MAIN_W("Invalid component command format");
            LOG_MAIN_I("Usage: log_component <COMPONENT> <level>");
            LOG_MAIN_I("Example: log_component DISPLAY debug");
        }
    }
    // Handle status command
    else if (strcmp(command, "log_status") == 0) {
        logging_print_status();
    }
    // Handle timezone command
    else if (strncmp(command, "log_timezone ", 13) == 0) {
        const char* timezone = command + 13;
        if (strlen(timezone) > 0) {
            logging_set_timezone(timezone);
        } else {
            LOG_MAIN_W("Invalid timezone command format");
            LOG_MAIN_I("Usage: log_timezone <timezone>");
            LOG_MAIN_I("Example: log_timezone EST5EDT");
        }
    }
    // Handle SNTP server command
    else if (strncmp(command, "log_sntp ", 9) == 0) {
        const char* server = command + 9;
        if (strlen(server) > 0) {
            logging_enable_sntp(server);
        } else {
            LOG_MAIN_W("Invalid SNTP command format");
            LOG_MAIN_I("Usage: log_sntp <server>");
            LOG_MAIN_I("Example: log_sntp pool.ntp.org");
        }
    }
    // Handle help command
    else if (strcmp(command, "log_help") == 0 || strcmp(command, "help") == 0) {
        LOG_MAIN_I("=== Logging Commands ===");
        LOG_MAIN_I("log_level <level>           - Set global log level");
        LOG_MAIN_I("log_component <tag> <level> - Set component log level");
        LOG_MAIN_I("log_status                  - Show current log configuration");
        LOG_MAIN_I("log_timezone <timezone>     - Set timezone (e.g., EST5EDT, UTC)");
        LOG_MAIN_I("log_sntp <server>           - Set NTP server for time sync");
        LOG_MAIN_I("log_help                    - Show this help");
        LOG_MAIN_I("");
        LOG_MAIN_I("Available levels: error, warn, info, debug, verbose");
        LOG_MAIN_I("Available components: MAIN, DISPLAY, UI, WEATHER, WIFI, CONFIG");
        LOG_MAIN_I("Timestamp format: %s", AURA_LOG_TIMESTAMP_ENABLED ? "ENABLED" : "DISABLED");
    }
}

void logging_print_status(void) {
    LOG_MAIN_I("=== Logging System Status ===");
    LOG_MAIN_I("Initialized: %s", logging_initialized ? "YES" : "NO");
    LOG_MAIN_I("Time Initialized: %s", time_initialized ? "YES" : "NO");
    LOG_MAIN_I("ESP-IDF Log Version: 2");
    LOG_MAIN_I("Timestamp Support: %s", AURA_LOG_TIMESTAMP_ENABLED ? "ENABLED" : "DISABLED");
    LOG_MAIN_I("Current Timezone: %s", current_timezone);
    LOG_MAIN_I("Time Set: %s", logging_is_time_set() ? "YES" : "NO");
    LOG_MAIN_I("");
    LOG_MAIN_I("Component Tags:");
    LOG_MAIN_I("  MAIN     - Main application");
    LOG_MAIN_I("  DISPLAY  - Display and touch handling");
    LOG_MAIN_I("  UI       - User interface logic");
    LOG_MAIN_I("  WEATHER  - Weather data fetching");
    LOG_MAIN_I("  WIFI     - WiFi connection management");
    LOG_MAIN_I("  CONFIG   - Configuration management");
    LOG_MAIN_I("");
    LOG_MAIN_I("Memory Status:");
    LOG_MEMORY_INFO(TAG_MAIN);
    LOG_MAIN_I("=== End Status ===");
}

// ============================================================================
// Timestamp Implementation
// ============================================================================

void logging_init_time(void) {
    if (time_initialized) {
        return;
    }
    
    // Set default timezone to UTC (safe to do without network)
    setenv("TZ", "UTC", 1);
    tzset();
    
    time_initialized = true;
    
    // Note: SNTP initialization is deferred until WiFi is available
    // Call logging_enable_sntp() after WiFi connection is established
    ESP_LOGI(TAG_MAIN, "Time system initialized (SNTP deferred until WiFi ready)");
}

void logging_set_timezone(const char* timezone) {
    if (!timezone) {
        return;
    }
    
    strncpy(current_timezone, timezone, sizeof(current_timezone) - 1);
    current_timezone[sizeof(current_timezone) - 1] = '\0';
    
    setenv("TZ", timezone, 1);
    tzset();
    
    LOG_MAIN_I("Timezone set to: %s", timezone);
}

void logging_get_timestamp(char* buffer, size_t buffer_size) {
    if (!buffer || buffer_size < AURA_LOG_TIMESTAMP_BUFFER_SIZE) {
        return;
    }
    
    time_t now;
    struct tm timeinfo;
    
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Check if time is set (year > 2020 indicates valid time)
    if (timeinfo.tm_year < (2020 - 1900)) {
        // Time not set, use system uptime instead
        uint32_t uptime_ms = millis();
        uint32_t seconds = uptime_ms / 1000;
        uint32_t minutes = seconds / 60;
        uint32_t hours = minutes / 60;
        
        snprintf(buffer, buffer_size, "UP:%02u:%02u:%02u", 
                 hours % 24, minutes % 60, seconds % 60);
        return;
    }
    
    // Format timestamp based on configuration
    #if AURA_LOG_TIMESTAMP_FORMAT_DATE_TIME
        // Full date and time format: "2024-01-15 14:30:45"
        strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", &timeinfo);
    #else
        // Time only format: "14:30:45"
        strftime(buffer, buffer_size, "%H:%M:%S", &timeinfo);
    #endif
}

bool logging_is_time_set(void) {
    time_t now;
    struct tm timeinfo;
    
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Consider time set if year is 2020 or later
    return (timeinfo.tm_year >= (2020 - 1900));
}

void logging_enable_sntp(const char* ntp_server) {
    if (!ntp_server) {
        return;
    }
    
    // Check if SNTP was already initialized
    static bool sntp_initialized = false;
    
    if (sntp_initialized) {
        // Stop SNTP if running
        esp_sntp_stop();
    }
    
    // Initialize/configure SNTP
    esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, ntp_server);
    esp_sntp_setservername(1, default_ntp_servers[1]);
    esp_sntp_setservername(2, default_ntp_servers[2]);
    
    // Start SNTP (safe to call after WiFi is connected)
    esp_sntp_init();
    sntp_initialized = true;
    
    LOG_MAIN_I("SNTP initialized with server: %s", ntp_server);
} 