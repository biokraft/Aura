#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

// ESP-IDF includes for logging functionality
#ifdef ESP32
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

// Enable ESP-IDF logging
#ifdef __cplusplus
extern "C" {
#endif

// Global logging configuration
#define AURA_LOG_LEVEL_DEFAULT ESP_LOG_INFO

// Component log tags - each component gets a unique tag for filtering
#define TAG_MAIN     "MAIN"
#define TAG_DISPLAY  "DISPLAY"
#define TAG_UI       "UI"
#define TAG_WEATHER  "WEATHER"
#define TAG_WIFI     "WIFI"
#define TAG_CONFIG   "CONFIG"

// Convenience macros for each component
// Main application logging
#define LOG_MAIN_E(format, ...) ESP_LOGE(TAG_MAIN, format, ##__VA_ARGS__)
#define LOG_MAIN_W(format, ...) ESP_LOGW(TAG_MAIN, format, ##__VA_ARGS__)
#define LOG_MAIN_I(format, ...) ESP_LOGI(TAG_MAIN, format, ##__VA_ARGS__)
#define LOG_MAIN_D(format, ...) ESP_LOGD(TAG_MAIN, format, ##__VA_ARGS__)
#define LOG_MAIN_V(format, ...) ESP_LOGV(TAG_MAIN, format, ##__VA_ARGS__)

// Display component logging
#define LOG_DISPLAY_E(format, ...) ESP_LOGE(TAG_DISPLAY, format, ##__VA_ARGS__)
#define LOG_DISPLAY_W(format, ...) ESP_LOGW(TAG_DISPLAY, format, ##__VA_ARGS__)
#define LOG_DISPLAY_I(format, ...) ESP_LOGI(TAG_DISPLAY, format, ##__VA_ARGS__)
#define LOG_DISPLAY_D(format, ...) ESP_LOGD(TAG_DISPLAY, format, ##__VA_ARGS__)
#define LOG_DISPLAY_V(format, ...) ESP_LOGV(TAG_DISPLAY, format, ##__VA_ARGS__)

// UI component logging
#define LOG_UI_E(format, ...) ESP_LOGE(TAG_UI, format, ##__VA_ARGS__)
#define LOG_UI_W(format, ...) ESP_LOGW(TAG_UI, format, ##__VA_ARGS__)
#define LOG_UI_I(format, ...) ESP_LOGI(TAG_UI, format, ##__VA_ARGS__)
#define LOG_UI_D(format, ...) ESP_LOGD(TAG_UI, format, ##__VA_ARGS__)
#define LOG_UI_V(format, ...) ESP_LOGV(TAG_UI, format, ##__VA_ARGS__)

// Weather component logging
#define LOG_WEATHER_E(format, ...) ESP_LOGE(TAG_WEATHER, format, ##__VA_ARGS__)
#define LOG_WEATHER_W(format, ...) ESP_LOGW(TAG_WEATHER, format, ##__VA_ARGS__)
#define LOG_WEATHER_I(format, ...) ESP_LOGI(TAG_WEATHER, format, ##__VA_ARGS__)
#define LOG_WEATHER_D(format, ...) ESP_LOGD(TAG_WEATHER, format, ##__VA_ARGS__)
#define LOG_WEATHER_V(format, ...) ESP_LOGV(TAG_WEATHER, format, ##__VA_ARGS__)

// WiFi component logging
#define LOG_WIFI_E(format, ...) ESP_LOGE(TAG_WIFI, format, ##__VA_ARGS__)
#define LOG_WIFI_W(format, ...) ESP_LOGW(TAG_WIFI, format, ##__VA_ARGS__)
#define LOG_WIFI_I(format, ...) ESP_LOGI(TAG_WIFI, format, ##__VA_ARGS__)
#define LOG_WIFI_D(format, ...) ESP_LOGD(TAG_WIFI, format, ##__VA_ARGS__)
#define LOG_WIFI_V(format, ...) ESP_LOGV(TAG_WIFI, format, ##__VA_ARGS__)

// Configuration logging
#define LOG_CONFIG_E(format, ...) ESP_LOGE(TAG_CONFIG, format, ##__VA_ARGS__)
#define LOG_CONFIG_W(format, ...) ESP_LOGW(TAG_CONFIG, format, ##__VA_ARGS__)
#define LOG_CONFIG_I(format, ...) ESP_LOGI(TAG_CONFIG, format, ##__VA_ARGS__)
#define LOG_CONFIG_D(format, ...) ESP_LOGD(TAG_CONFIG, format, ##__VA_ARGS__)
#define LOG_CONFIG_V(format, ...) ESP_LOGV(TAG_CONFIG, format, ##__VA_ARGS__)

// Logging system functions
void logging_init(void);
void logging_set_level(esp_log_level_t level);
void logging_set_component_level(const char* tag, esp_log_level_t level);
void logging_handle_serial_command(const char* command);
void logging_print_status(void);

// Utility macros for common logging patterns
#define LOG_FUNCTION_ENTRY(tag) ESP_LOGD(tag, ">> %s", __FUNCTION__)
#define LOG_FUNCTION_EXIT(tag) ESP_LOGD(tag, "<< %s", __FUNCTION__)

// Memory and system state logging helpers
#define LOG_MEMORY_INFO(tag) do { \
    ESP_LOGI(tag, "Free heap: %d bytes, Min free: %d bytes", \
             esp_get_free_heap_size(), esp_get_minimum_free_heap_size()); \
} while(0)

#define LOG_TASK_STACK_INFO(tag) do { \
    ESP_LOGD(tag, "Task high water mark: %d bytes", uxTaskGetStackHighWaterMark(NULL)); \
} while(0)

#ifdef __cplusplus
}
#endif

#endif // LOGGING_H 