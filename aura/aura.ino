// Aura Weather Display - Main Application
// Modular Architecture - Simplified Main File

#include "src/components/display/display.h"
#include "src/components/ui/ui.h"
#include "src/components/weather/weather.h"
#include "src/components/wifi/wifi.h"
#include "src/components/logging/logging.h"
#include "src/config.h"

#include "esp_system.h"
#include <time.h>

// Global component instances
Display display;
UI ui;
Weather weather;
WiFiComponent wifi_component;

// Global variables (defined in config.h as extern)
Language current_language = LANG_EN;
bool use_fahrenheit = false;
bool use_24_hour = false;
char latitude[16] = LATITUDE_DEFAULT;
char longitude[16] = LONGITUDE_DEFAULT;
String location = String(LOCATION_DEFAULT);

// Timing variables
unsigned long last_weather_update = 0;
unsigned long last_clock_update = 0;
unsigned long last_lvgl_update = 0;
unsigned long last_memory_report = 0;

// Memory monitoring function
void printMemoryStatus() {
    size_t free_heap = esp_get_free_heap_size();
    size_t min_free_heap = esp_get_minimum_free_heap_size();
    size_t total_heap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    
    LOG_MAIN_I("=== Memory Status ===");
    LOG_MAIN_I("Free heap: %u bytes", (unsigned int)free_heap);
    LOG_MAIN_I("Minimum free heap: %u bytes", (unsigned int)min_free_heap);
    
    // Safe fragmentation calculation with bounds checking
    if (total_heap > 0 && free_heap <= total_heap) {
        unsigned int fragmentation = (unsigned int)(100 - (free_heap * 100) / total_heap);
        LOG_MAIN_I("Heap fragmentation: %u%%", fragmentation);
    } else {
        LOG_MAIN_I("Heap fragmentation: N/A");
    }
    
    LOG_MAIN_I("Total heap: %u bytes", (unsigned int)total_heap);
    LOG_MAIN_I("PSRAM available: %s", heap_caps_get_total_size(MALLOC_CAP_SPIRAM) > 0 ? "Yes" : "No");
    LOG_MAIN_I("=====================");
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000); // Give serial time to initialize
    
    // Print initial memory status
    printMemoryStatus();
    
    // Initialize logging system first
    logging_init();
    LOG_MAIN_I("=== Aura Weather Display Starting ===");
    LOG_MAIN_I("Version: 1.0");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    LOG_MAIN_I("Aura Weather Display Starting...");

    // Initialize display first
    LOG_MAIN_I("Initializing display...");
    
    if (!display.init()) {
        LOG_MAIN_E("Display initialization failed!");
        while (1) delay(1000); // Halt on critical error with delay to prevent watchdog
    }
    
    LOG_MAIN_I("Display initialized successfully");
    printMemoryStatus(); // Check memory after display init

    // Small delay to let display settle
    delay(100);

    // Initialize UI now that LVGL is confirmed working
    LOG_MAIN_I("Initializing UI...");
    
    if (!ui.init(&display)) {
        LOG_MAIN_E("UI initialization failed!");
        while (1) delay(1000); // Halt on critical error
    }
    
    // Set UI language from global setting
    ui.setLanguage(current_language);
    LOG_MAIN_I("UI initialized successfully");
    printMemoryStatus(); // Check memory after UI init

    // Create a simple splash screen first instead of WiFi config screen
    LOG_MAIN_I("Creating simple splash screen...");
    ui.createSimpleSplashScreen();
    
    // Force LVGL update with error checking
    LOG_MAIN_I("Updating display...");
    try {
        display.task(); // Process LVGL tasks
        delay(10);
        display.task(); // Process again to ensure completion
    } catch (...) {
        LOG_MAIN_E("LVGL update failed!");
        while (1) delay(1000);
    }
    
    LOG_MAIN_I("Initial UI displayed successfully");
    printMemoryStatus(); // Check memory after initial UI

    // Initialize WiFi component with retry
    LOG_MAIN_I("Initializing WiFi component...");
    int wifi_init_retries = 3;
    while (wifi_init_retries > 0) {
        if (wifi_component.init()) {
            LOG_MAIN_I("WiFi component initialized successfully");
            break;
        } else {
            wifi_init_retries--;
            LOG_MAIN_W("WiFi init failed, retries left: %d", wifi_init_retries);
            if (wifi_init_retries == 0) {
                LOG_MAIN_E("WiFi component initialization failed after retries!");
                // Don't halt - continue without WiFi
                break;
            }
            delay(1000);
        }
    }

    // Initialize weather component with retry
    LOG_MAIN_I("Initializing weather component...");
    int weather_init_retries = 3;
    while (weather_init_retries > 0) {
        if (weather.init()) {
            LOG_MAIN_I("Weather initialized successfully");
            break;
        } else {
            weather_init_retries--;
            LOG_MAIN_W("Weather init failed, retries left: %d", weather_init_retries);
            if (weather_init_retries == 0) {
                LOG_MAIN_E("Weather initialization failed after retries!");
                // Don't halt - continue without weather
                break;
            }
            delay(1000);
        }
    }

    // Now show WiFi config screen
    LOG_MAIN_I("Showing WiFi configuration screen...");
    ui.createWiFiConfigScreen();
    display.task(); // Update display
    delay(10);
    display.task(); // Update again
    LOG_MAIN_I("WiFi config screen displayed");

    // Try to connect to WiFi (non-blocking)
    LOG_MAIN_I("Attempting WiFi connection...");
    bool wifi_connected = wifi_component.connect();
    
    if (wifi_connected) {
        LOG_MAIN_I("WiFi connected successfully");
        LOG_MAIN_I("IP address: %s", wifi_component.getLocalIP().c_str());
        
        // Configure time
        configTime(0, 0, "pool.ntp.org");
        LOG_MAIN_I("Time configuration completed");
    } else {
        LOG_MAIN_W("WiFi connection failed - continuing with offline mode");
    }

    // Always create the main screen regardless of WiFi status
    LOG_MAIN_I("Creating main screen...");
    ui.createMainScreen();
    display.task();

    // Initialize weather data with default values
    LOG_MAIN_I("Setting up initial weather display...");
    
    // Set initial data to make the display look correct
    ui.updateTemperature(22.0, 24.0); // 22°C, feels like 24°C
    ui.updateLocation("London");
    ui.updateClock(); // Update clock immediately
    
    // Update weather icon to test our positioning fix
    ui.updateBackground(2, 1); // Partly cloudy, day
    
    // Force immediate redraw
    display.task();
    LOG_MAIN_I("Main UI displayed with initial data");

    // Only try to fetch weather data if WiFi is connected
    if (wifi_connected) {
        LOG_MAIN_I("Triggering initial weather data fetch...");
        if (weather.fetchWeatherData()) {
            LOG_MAIN_I("Initial weather data fetched successfully");
            updateUIWithWeatherData();
        } else {
            LOG_MAIN_W("Initial weather fetch failed, using placeholder data");
        }
    } else {
        LOG_MAIN_I("Offline mode - using sample weather data");
    }

    printMemoryStatus(); // Final memory check
    LOG_MAIN_I("Aura Weather Display Setup Complete!");
}

void loop() {
    unsigned long current_time = millis();

    // Handle serial commands for logging control
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("log_")) {
            logging_handle_serial_command(command.c_str());
        } else if (command == "mem") {
            printMemoryStatus();
        } else if (command == "restart") {
            LOG_MAIN_I("Restarting...");
            ESP.restart();
        }
        // Add other command handling here in the future
    }

    // Handle LVGL timer (high frequency) with error protection
    if (current_time - last_lvgl_update >= 5) {
        try {
            display.task(); // This calls lv_timer_handler()
            last_lvgl_update = current_time;
        } catch (...) {
            LOG_MAIN_E("LVGL task error - restarting in 5 seconds");
            delay(5000);
            ESP.restart();
        }
    }

    // Update clock every second
    if (current_time - last_clock_update >= 1000) {
        ui.updateClock();
        last_clock_update = current_time;
    }

    // Update weather every 5 minutes (only if connected)
    if (wifi_component.isConnected() && 
        (current_time - last_weather_update >= 300000 || last_weather_update == 0)) {
        LOG_MAIN_D("Updating weather data...");
        if (weather.fetchWeatherData()) {
            LOG_MAIN_I("Weather data updated successfully");
            updateUIWithWeatherData();
        } else {
            LOG_MAIN_W("Failed to fetch weather data - keeping current display");
        }
        last_weather_update = current_time;
    }

    // Print memory status every 30 seconds in debug builds
    if (current_time - last_memory_report >= 30000) {
        size_t free_heap = esp_get_free_heap_size();
        LOG_MAIN_D("Free heap: %d bytes", free_heap);
        last_memory_report = current_time;
    }

    // LVGL tick increment for timing
    lv_tick_inc(5);

    // Small delay to prevent watchdog issues
    delay(5);
}

// Helper function to update UI with weather data
void updateUIWithWeatherData() {
    // Get current weather data from weather component
    const WeatherData& weatherData = weather.getCurrentWeather();
    
    if (weather.isDataValid()) {
        // Update current temperature
        ui.updateTemperature(weatherData.current_temp, weatherData.feels_like);
        LOG_MAIN_D("UI updated with temperature: %.1f°C (feels like %.1f°C)", 
                      weatherData.current_temp, weatherData.feels_like);
        
        // Update weather background using UI component's image selection
        ui.updateBackground(weatherData.weather_code, weatherData.is_day ? 1 : 0);
        LOG_MAIN_D("UI updated with weather code: %d (day: %d)", 
                      weatherData.weather_code, weatherData.is_day ? 1 : 0);
        
        // Update location
        if (!weatherData.location_name.isEmpty()) {
            ui.updateLocation(weatherData.location_name);
            LOG_MAIN_D("UI updated with location: %s", weatherData.location_name.c_str());
        } else if (!location.isEmpty()) {
            ui.updateLocation(location);
            LOG_MAIN_D("UI updated with configured location: %s", location.c_str());
        }
        
        LOG_MAIN_I("Weather data integration completed successfully");
    } else {
        LOG_MAIN_W("Weather data is not valid, keeping current UI state");
    }
}
