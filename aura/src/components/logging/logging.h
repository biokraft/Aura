#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

// ESP-IDF includes for logging functionality
#ifdef ESP32
#include "esp_log.h"
#include "esp_system.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

// Enable ESP-IDF logging
#ifdef __cplusplus
extern "C" {
#endif

// Global logging configuration
#define AURA_LOG_LEVEL_DEFAULT ESP_LOG_INFO

// Timestamp configuration
#define AURA_LOG_TIMESTAMP_ENABLED 1
#define AURA_LOG_TIMESTAMP_FORMAT_DATE_TIME 0  // Include date and time
#define AURA_LOG_TIMESTAMP_FORMAT_TIME_ONLY 1  // Only time (faster)
#define AURA_LOG_TIMESTAMP_BUFFER_SIZE 32

// Component log tags - each component gets a unique tag for filtering
#define TAG_MAIN     "MAIN"
#define TAG_DISPLAY  "DISPLAY"
#define TAG_UI       "UI"
#define TAG_WEATHER  "WEATHER"
#define TAG_WIFI     "WIFI"
#define TAG_CONFIG   "CONFIG"

// Enhanced logging macros with timestamp support
#if AURA_LOG_TIMESTAMP_ENABLED
    // Internal macro for timestamped logging
    #define _LOG_WITH_TIMESTAMP(level, tag, format, ...) do { \
        char timestamp_str[AURA_LOG_TIMESTAMP_BUFFER_SIZE]; \
        logging_get_timestamp(timestamp_str, sizeof(timestamp_str)); \
        ESP_LOG##level(tag, "[%s] " format, timestamp_str, ##__VA_ARGS__); \
    } while(0)
    
    // Main application logging with timestamps
    #define LOG_MAIN_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_MAIN, format, ##__VA_ARGS__)

    // Display component logging with timestamps
    #define LOG_DISPLAY_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_DISPLAY, format, ##__VA_ARGS__)

    // UI component logging with timestamps
    #define LOG_UI_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_UI, format, ##__VA_ARGS__)

    // Weather component logging with timestamps
    #define LOG_WEATHER_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_WEATHER, format, ##__VA_ARGS__)

    // WiFi component logging with timestamps
    #define LOG_WIFI_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_WIFI, format, ##__VA_ARGS__)

    // Configuration logging with timestamps
    #define LOG_CONFIG_E(format, ...) _LOG_WITH_TIMESTAMP(E, TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_W(format, ...) _LOG_WITH_TIMESTAMP(W, TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_I(format, ...) _LOG_WITH_TIMESTAMP(I, TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_D(format, ...) _LOG_WITH_TIMESTAMP(D, TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_V(format, ...) _LOG_WITH_TIMESTAMP(V, TAG_CONFIG, format, ##__VA_ARGS__)
#else
    // Fallback to standard ESP-IDF logging without timestamps
    #define LOG_MAIN_E(format, ...) ESP_LOGE(TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_W(format, ...) ESP_LOGW(TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_I(format, ...) ESP_LOGI(TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_D(format, ...) ESP_LOGD(TAG_MAIN, format, ##__VA_ARGS__)
    #define LOG_MAIN_V(format, ...) ESP_LOGV(TAG_MAIN, format, ##__VA_ARGS__)

    #define LOG_DISPLAY_E(format, ...) ESP_LOGE(TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_W(format, ...) ESP_LOGW(TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_I(format, ...) ESP_LOGI(TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_D(format, ...) ESP_LOGD(TAG_DISPLAY, format, ##__VA_ARGS__)
    #define LOG_DISPLAY_V(format, ...) ESP_LOGV(TAG_DISPLAY, format, ##__VA_ARGS__)

    #define LOG_UI_E(format, ...) ESP_LOGE(TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_W(format, ...) ESP_LOGW(TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_I(format, ...) ESP_LOGI(TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_D(format, ...) ESP_LOGD(TAG_UI, format, ##__VA_ARGS__)
    #define LOG_UI_V(format, ...) ESP_LOGV(TAG_UI, format, ##__VA_ARGS__)

    #define LOG_WEATHER_E(format, ...) ESP_LOGE(TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_W(format, ...) ESP_LOGW(TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_I(format, ...) ESP_LOGI(TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_D(format, ...) ESP_LOGD(TAG_WEATHER, format, ##__VA_ARGS__)
    #define LOG_WEATHER_V(format, ...) ESP_LOGV(TAG_WEATHER, format, ##__VA_ARGS__)

    #define LOG_WIFI_E(format, ...) ESP_LOGE(TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_W(format, ...) ESP_LOGW(TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_I(format, ...) ESP_LOGI(TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_D(format, ...) ESP_LOGD(TAG_WIFI, format, ##__VA_ARGS__)
    #define LOG_WIFI_V(format, ...) ESP_LOGV(TAG_WIFI, format, ##__VA_ARGS__)

    #define LOG_CONFIG_E(format, ...) ESP_LOGE(TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_W(format, ...) ESP_LOGW(TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_I(format, ...) ESP_LOGI(TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_D(format, ...) ESP_LOGD(TAG_CONFIG, format, ##__VA_ARGS__)
    #define LOG_CONFIG_V(format, ...) ESP_LOGV(TAG_CONFIG, format, ##__VA_ARGS__)
#endif

// Logging system functions
void logging_init(void);
void logging_set_level(esp_log_level_t level);
void logging_set_component_level(const char* tag, esp_log_level_t level);
void logging_handle_serial_command(const char* command);
void logging_print_status(void);

// Timestamp functions
void logging_init_time(void);
void logging_set_timezone(const char* timezone);
void logging_get_timestamp(char* buffer, size_t buffer_size);
bool logging_is_time_set(void);
void logging_enable_sntp(const char* ntp_server);

// Utility macros for common logging patterns
#if AURA_LOG_TIMESTAMP_ENABLED
    #define LOG_FUNCTION_ENTRY(tag) do { \
        char timestamp_str[AURA_LOG_TIMESTAMP_BUFFER_SIZE]; \
        logging_get_timestamp(timestamp_str, sizeof(timestamp_str)); \
        ESP_LOGD(tag, "[%s] >> %s", timestamp_str, __FUNCTION__); \
    } while(0)
    
    #define LOG_FUNCTION_EXIT(tag) do { \
        char timestamp_str[AURA_LOG_TIMESTAMP_BUFFER_SIZE]; \
        logging_get_timestamp(timestamp_str, sizeof(timestamp_str)); \
        ESP_LOGD(tag, "[%s] << %s", timestamp_str, __FUNCTION__); \
    } while(0)
    
    // Memory and system state logging helpers with timestamps
    #define LOG_MEMORY_INFO(tag) do { \
        char timestamp_str[AURA_LOG_TIMESTAMP_BUFFER_SIZE]; \
        logging_get_timestamp(timestamp_str, sizeof(timestamp_str)); \
        ESP_LOGI(tag, "[%s] Free heap: %d bytes, Min free: %d bytes", \
                 timestamp_str, esp_get_free_heap_size(), esp_get_minimum_free_heap_size()); \
    } while(0)
    
    #define LOG_TASK_STACK_INFO(tag) do { \
        char timestamp_str[AURA_LOG_TIMESTAMP_BUFFER_SIZE]; \
        logging_get_timestamp(timestamp_str, sizeof(timestamp_str)); \
        ESP_LOGD(tag, "[%s] Task high water mark: %d bytes", \
                 timestamp_str, uxTaskGetStackHighWaterMark(NULL)); \
    } while(0)
#else
    #define LOG_FUNCTION_ENTRY(tag) ESP_LOGD(tag, ">> %s", __FUNCTION__)
    #define LOG_FUNCTION_EXIT(tag) ESP_LOGD(tag, "<< %s", __FUNCTION__)
    
    #define LOG_MEMORY_INFO(tag) do { \
        ESP_LOGI(tag, "Free heap: %d bytes, Min free: %d bytes", \
                 esp_get_free_heap_size(), esp_get_minimum_free_heap_size()); \
    } while(0)
    
    #define LOG_TASK_STACK_INFO(tag) do { \
        ESP_LOGD(tag, "Task high water mark: %d bytes", uxTaskGetStackHighWaterMark(NULL)); \
    } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif // LOGGING_H 