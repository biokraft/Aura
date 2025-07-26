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
    
    Serial.printf("=== Memory Status ===\n");
    Serial.printf("Free heap: %u bytes\n", (unsigned int)free_heap);
    Serial.printf("Minimum free heap: %u bytes\n", (unsigned int)min_free_heap);
    
    // Safe fragmentation calculation with bounds checking
    if (total_heap > 0 && free_heap <= total_heap) {
        unsigned int fragmentation = (unsigned int)(100 - (free_heap * 100) / total_heap);
        Serial.printf("Heap fragmentation: %u%%\n", fragmentation);
    } else {
        Serial.printf("Heap fragmentation: N/A\n");
    }
    
    Serial.printf("Total heap: %u bytes\n", (unsigned int)total_heap);
    Serial.printf("PSRAM available: %s\n", heap_caps_get_total_size(MALLOC_CAP_SPIRAM) > 0 ? "Yes" : "No");
    Serial.printf("=====================\n");
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
    
    Serial.println("Aura Weather Display Starting...");

    // Initialize display first
    LOG_MAIN_I("Initializing display...");
    Serial.println("Initializing display...");
    
    if (!display.init()) {
        LOG_MAIN_E("Display initialization failed!");
        Serial.println("ERROR: Display initialization failed!");
        while (1) delay(1000); // Halt on critical error with delay to prevent watchdog
    }
    
    LOG_MAIN_I("Display initialized successfully");
    Serial.println("Display initialized successfully");
    printMemoryStatus(); // Check memory after display init

    // Small delay to let display settle
    delay(100);

    // Initialize UI now that LVGL is confirmed working
    LOG_MAIN_I("Initializing UI...");
    Serial.println("Initializing UI...");
    
    if (!ui.init(&display)) {
        LOG_MAIN_E("UI initialization failed!");
        Serial.println("ERROR: UI initialization failed!");
        while (1) delay(1000); // Halt on critical error
    }
    
    // Set UI language from global setting
    ui.setLanguage(current_language);
    LOG_MAIN_I("UI initialized successfully");
    Serial.println("UI initialized successfully");
    printMemoryStatus(); // Check memory after UI init

    // Create a simple splash screen first instead of WiFi config screen
    Serial.println("Creating simple splash screen...");
    ui.createSimpleSplashScreen();
    
    // Force LVGL update with error checking
    Serial.println("Updating display...");
    try {
        display.task(); // Process LVGL tasks
        delay(10);
        display.task(); // Process again to ensure completion
    } catch (...) {
        Serial.println("ERROR: LVGL update failed!");
        while (1) delay(1000);
    }
    
    Serial.println("Initial UI displayed successfully");
    printMemoryStatus(); // Check memory after initial UI

    // Initialize WiFi component with retry
    Serial.println("Initializing WiFi component...");
    int wifi_init_retries = 3;
    while (wifi_init_retries > 0) {
        if (wifi_component.init()) {
            Serial.println("WiFi component initialized successfully");
            break;
        } else {
            wifi_init_retries--;
            Serial.printf("WiFi init failed, retries left: %d\n", wifi_init_retries);
            if (wifi_init_retries == 0) {
                Serial.println("ERROR: WiFi component initialization failed after retries!");
                // Don't halt - continue without WiFi
                break;
            }
            delay(1000);
        }
    }

    // Initialize weather component with retry
    Serial.println("Initializing weather component...");
    int weather_init_retries = 3;
    while (weather_init_retries > 0) {
        if (weather.init()) {
            Serial.println("Weather initialized successfully");
            break;
        } else {
            weather_init_retries--;
            Serial.printf("Weather init failed, retries left: %d\n", weather_init_retries);
            if (weather_init_retries == 0) {
                Serial.println("ERROR: Weather initialization failed after retries!");
                // Don't halt - continue without weather
                break;
            }
            delay(1000);
        }
    }

    // Now show WiFi config screen
    Serial.println("Showing WiFi configuration screen...");
    ui.createWiFiConfigScreen();
    display.task(); // Update display
    delay(10);
    display.task(); // Update again
    Serial.println("WiFi config screen displayed");

    // Try to connect to WiFi (non-blocking)
    Serial.println("Attempting WiFi connection...");
    if (wifi_component.connect()) {
        Serial.println("WiFi connected successfully");
        Serial.printf("IP address: %s\n", wifi_component.getLocalIP().c_str());
        
        // Configure time
        configTime(0, 0, "pool.ntp.org");
        Serial.println("Time configuration completed");

        // Now create the main screen and populate with data
        ui.createMainScreen();
        display.task();

        // Initialize weather data with default values
        Serial.println("Setting up initial weather display...");
        
        // Set initial data to make the display look correct
        ui.updateTemperature(22.0, 24.0); // 22°C, feels like 24°C
        ui.updateLocation("London");
        ui.updateClock(); // Update clock immediately
        
        // Force immediate redraw
        display.task();
        Serial.println("Main UI displayed with initial data");

        // Trigger first weather update
        Serial.println("Triggering initial weather data fetch...");
        if (weather.fetchWeatherData()) {
            Serial.println("Initial weather data fetched successfully");
            updateUIWithWeatherData();
        } else {
            Serial.println("Initial weather fetch failed, using placeholder data");
        }
    } else {
        Serial.println("WiFi connection failed - continuing with offline mode");
    }

    printMemoryStatus(); // Final memory check
    Serial.println("Aura Weather Display Setup Complete!");
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
            Serial.println("Restarting...");
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
            Serial.println("LVGL task error - restarting in 5 seconds");
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
        Serial.println("Updating weather data...");
        if (weather.fetchWeatherData()) {
            Serial.println("Weather data updated successfully");
            updateUIWithWeatherData();
        } else {
            Serial.println("Failed to fetch weather data - keeping current display");
        }
        last_weather_update = current_time;
    }

    // Print memory status every 30 seconds in debug builds
    if (current_time - last_memory_report >= 30000) {
        size_t free_heap = esp_get_free_heap_size();
        Serial.printf("Free heap: %d bytes\n", free_heap);
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
        Serial.printf("UI updated with temperature: %.1f°C (feels like %.1f°C)\n", 
                      weatherData.current_temp, weatherData.feels_like);
        
        // Update weather background using UI component's image selection
        ui.updateBackground(weatherData.weather_code, weatherData.is_day ? 1 : 0);
        Serial.printf("UI updated with weather code: %d (day: %d)\n", 
                      weatherData.weather_code, weatherData.is_day ? 1 : 0);
        
        // Update location
        if (!weatherData.location_name.isEmpty()) {
            ui.updateLocation(weatherData.location_name);
            Serial.printf("UI updated with location: %s\n", weatherData.location_name.c_str());
        } else if (!location.isEmpty()) {
            ui.updateLocation(location);
            Serial.printf("UI updated with configured location: %s\n", location.c_str());
        }
        
        Serial.println("Weather data integration completed successfully");
    } else {
        Serial.println("Weather data is not valid, keeping current UI state");
    }
}
