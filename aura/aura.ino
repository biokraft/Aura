// Aura Weather Display - Main Application
// Complete implementation with all components

#include "src/config.h"
#include "src/components/logging/logging.h"
#include "src/components/display/display.h"
#include "src/components/ui/ui.h"
#include "src/components/wifi/wifi.h"
#include "src/components/weather/weather.h"

#include <lvgl.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// Global component instances
Display display;
UI ui;
WiFiComponent wifi_component;
Weather weather;

// Global variables (required by config.h extern declarations)
Language current_language = LANG_EN;
bool use_fahrenheit = false;
bool use_24_hour = true;
char latitude[16] = LATITUDE_DEFAULT;
char longitude[16] = LONGITUDE_DEFAULT;
String location = LOCATION_DEFAULT;

// Timing variables
unsigned long lastWeatherUpdate = 0;
unsigned long lastClockUpdate = 0;
unsigned long lastLVGLTick = 0;

void setup() {
    Serial.begin(115200);
    delay(2000); // Allow serial to stabilize
    
    // Initialize logging system first
    LOG_MAIN_I("=== AURA WEATHER DISPLAY STARTING ===");
    logging_init();
    LOG_MAIN_I("Logging system initialized");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Initialize display system
    LOG_MAIN_I("Initializing display...");
    if (!display.init()) {
        LOG_MAIN_E("Display initialization failed");
        while(1) delay(1000); // Halt on critical failure
    }
    LOG_MAIN_I("Display initialized successfully");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Initialize UI system
    LOG_MAIN_I("Initializing UI...");
    if (!ui.init(&display)) {
        LOG_MAIN_E("UI initialization failed");
        while(1) delay(1000); // Halt on critical failure
    }
    LOG_MAIN_I("UI initialized successfully");
    
    // Create splash screen first
    LOG_MAIN_I("Creating splash screen...");
    ui.createSimpleSplashScreen();
    lv_timer_handler(); // Process LVGL tasks
    delay(2000); // Show splash screen
    
    // Initialize WiFi component
    LOG_MAIN_I("Initializing WiFi...");
    if (!wifi_component.init()) {
        LOG_MAIN_E("WiFi initialization failed");
    } else {
        LOG_MAIN_I("WiFi initialized successfully");
        
        // Try to connect to WiFi
        LOG_MAIN_I("Attempting WiFi connection...");
        if (wifi_component.connect()) {
            LOG_MAIN_I("WiFi connected successfully");
            LOG_MAIN_I("IP address: %s", wifi_component.getLocalIP().c_str());
            
            // Enable SNTP now that WiFi is connected
            logging_enable_sntp("pool.ntp.org");
        } else {
            LOG_MAIN_W("WiFi connection failed - continuing without network");
        }
    }
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Initialize weather component
    LOG_MAIN_I("Initializing weather...");
    if (!weather.init()) {
        LOG_MAIN_E("Weather initialization failed");
    } else {
        LOG_MAIN_I("Weather initialized successfully");
        
        // Fetch initial weather data if WiFi is connected
        if (wifi_component.isConnected()) {
            LOG_MAIN_I("Fetching initial weather data...");
            weather.fetchWeatherData();
        }
    }
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Create main application screen
    LOG_MAIN_I("Creating main application screen...");
    ui.createMainScreen();
    
    // Update UI with any available data
    if (weather.isDataValid()) {
        LOG_MAIN_I("Updating UI with weather data");
        // Convert weather data to JSON for UI update
        JsonDocument weatherDoc;
        const WeatherData& data = weather.getCurrentWeather();
        weatherDoc["current"]["temperature_2m"] = data.current_temp;
        weatherDoc["current"]["apparent_temperature"] = data.feels_like;
        weatherDoc["current"]["weather_code"] = data.weather_code;
        weatherDoc["current"]["is_day"] = data.is_day;
        ui.updateWeatherData(weatherDoc);
    }
    
    LOG_MAIN_I("=== AURA INITIALIZATION COMPLETE ===");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    // Initialize timing
    lastWeatherUpdate = millis();
    lastClockUpdate = millis();
    lastLVGLTick = millis();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Handle LVGL timer tasks (required for UI updates)
    if (currentTime - lastLVGLTick >= 5) {
        lv_tick_inc(currentTime - lastLVGLTick);
        lastLVGLTick = currentTime;
    }
    lv_timer_handler();
    
    // Update clock every minute
    if (currentTime - lastClockUpdate >= 60000) {
        ui.updateClock();
        lastClockUpdate = currentTime;
    }
    
    // Update weather data every 10 minutes if connected
    if (wifi_component.isConnected() && 
        (currentTime - lastWeatherUpdate >= UPDATE_INTERVAL)) {
        
        LOG_MAIN_I("Updating weather data...");
        if (weather.fetchWeatherData()) {
            LOG_MAIN_I("Weather data updated successfully");
            
            // Update UI with new weather data
            JsonDocument weatherDoc;
            const WeatherData& data = weather.getCurrentWeather();
            weatherDoc["current"]["temperature_2m"] = data.current_temp;
            weatherDoc["current"]["apparent_temperature"] = data.feels_like;
            weatherDoc["current"]["weather_code"] = data.weather_code;
            weatherDoc["current"]["is_day"] = data.is_day;
            ui.updateWeatherData(weatherDoc);
            
            lastWeatherUpdate = currentTime;
        } else {
            LOG_MAIN_W("Weather update failed");
        }
    }
    
    // Small delay to prevent watchdog issues
    delay(10);
}
