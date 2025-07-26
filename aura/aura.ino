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

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    
    // Initialize logging system first
    logging_init();
    LOG_MAIN_I("=== Aura Weather Display Starting ===");
    LOG_MAIN_I("Version: 1.0");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    Serial.println("Aura Weather Display Starting...");

    // Initialize display first
    LOG_MAIN_I("Initializing display...");
    if (!display.init()) {
        LOG_MAIN_E("Display initialization failed!");
        Serial.println("ERROR: Display initialization failed!");
        while (1); // Halt on critical error
    }
    LOG_MAIN_I("Display initialized successfully");
    Serial.println("Display initialized successfully");

    // Initialize UI now that LVGL is confirmed working
    LOG_MAIN_I("Initializing UI...");
    if (!ui.init(&display)) {
        LOG_MAIN_E("UI initialization failed!");
        Serial.println("ERROR: UI initialization failed!");
        while (1); // Halt on critical error
    }
    
    // Set UI language from global setting
    ui.setLanguage(current_language);
    LOG_MAIN_I("UI initialized successfully");
    Serial.println("UI initialized successfully");

    // Show WiFi configuration screen first
    ui.createWiFiConfigScreen();
    lv_refr_now(NULL); // Force immediate display update
    Serial.println("WiFi config screen displayed");

    // Initialize WiFi component
    if (!wifi_component.init()) {
        Serial.println("ERROR: WiFi component initialization failed!");
        while (1); // Halt on critical error
    }
    Serial.println("WiFi component initialized successfully");

    // Initialize weather component
    if (!weather.init()) {
        Serial.println("ERROR: Weather initialization failed!");
        while (1); // Halt on critical error
    }
    Serial.println("Weather initialized successfully");

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    if (!wifi_component.connect()) {
        Serial.println("Failed to connect to WiFi");
        ESP.restart(); // Restart and try again
    }
    
    Serial.println("WiFi connected successfully");
    Serial.printf("IP address: %s\n", wifi_component.getLocalIP().c_str());

    // Configure time
    configTime(0, 0, "pool.ntp.org");
    Serial.println("Time configuration completed");

    // Now create the main screen and populate with data
    ui.createMainScreen();

    // Initialize weather data with default values
    Serial.println("Setting up initial weather display...");
    
    // Set initial data to make the display look correct
    ui.updateTemperature(22.0, 24.0); // 22°C, feels like 24°C
    ui.updateLocation("London");
    ui.updateClock(); // Update clock immediately
    
    // Force immediate redraw
    lv_refr_now(NULL);
    Serial.println("Main UI displayed with initial data");

    // Trigger first weather update
    Serial.println("Triggering initial weather data fetch...");
    if (weather.fetchWeatherData()) {
        Serial.println("Initial weather data fetched successfully");
        updateUIWithWeatherData();
    } else {
        Serial.println("Initial weather fetch failed, using placeholder data");
    }

    Serial.println("Aura Weather Display Ready!");
}

void loop() {
    unsigned long current_time = millis();

    // Handle serial commands for logging control
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("log_")) {
            logging_handle_serial_command(command.c_str());
        }
        // Add other command handling here in the future
    }

    // Handle LVGL timer (high frequency)
    if (current_time - last_lvgl_update >= 5) {
        display.task(); // This calls lv_timer_handler()
        last_lvgl_update = current_time;
    }

    // Update clock every second
    if (current_time - last_clock_update >= 1000) {
        ui.updateClock();
        last_clock_update = current_time;
    }

    // Update weather every 5 minutes
    if (current_time - last_weather_update >= 300000 || last_weather_update == 0) {
        Serial.println("Updating weather data...");
        if (weather.fetchWeatherData()) {
            Serial.println("Weather data updated successfully");
            updateUIWithWeatherData();
        } else {
            Serial.println("Failed to fetch weather data - keeping current display");
        }
        last_weather_update = current_time;
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
