#include "ui.h"
#include <Arduino.h>
#include <time.h>

// Static instance for callbacks
UI* UI::instance = nullptr;

UI::UI() : display_ref(nullptr), main_screen(nullptr), settings_win(nullptr), location_win(nullptr) {
    instance = this;
    
    // Initialize all UI element pointers to nullptr for safety
    lbl_today_temp = nullptr;
    lbl_today_feels_like = nullptr;
    img_today_icon = nullptr;
    lbl_forecast = nullptr;
    box_daily = nullptr;
    box_hourly = nullptr;
    lbl_clock = nullptr;
    
    for (int i = 0; i < 7; i++) {
        lbl_daily_day[i] = nullptr;
        lbl_daily_high[i] = nullptr;
        lbl_daily_low[i] = nullptr;
        img_daily[i] = nullptr;
        lbl_hourly[i] = nullptr;
        lbl_precipitation_probability[i] = nullptr;
        lbl_hourly_temp[i] = nullptr;
        img_hourly[i] = nullptr;
    }
}

UI::~UI() {
    // Clean up all UI elements properly
    cleanupMainScreen();
    instance = nullptr;
}

bool UI::init(Display* display) {
    if (!display) {
        Serial.println("ERROR: Display reference is null");
        return false;
    }
    
    display_ref = display;
    Serial.println("UI initialization completed successfully");
    return true;
}

void UI::cleanupMainScreen() {
    // LVGL automatically handles object cleanup when parent is deleted
    // Just need to clean up the main screen if it exists
    if (main_screen) {
        lv_obj_del(main_screen);
        main_screen = nullptr;
        Serial.println("Main screen cleaned up");
    }
    
    // Reset all pointers to nullptr for safety
    lbl_today_temp = nullptr;
    lbl_today_feels_like = nullptr;
    img_today_icon = nullptr;
    lbl_forecast = nullptr;
    box_daily = nullptr;
    box_hourly = nullptr;
    lbl_clock = nullptr;
    
    for (int i = 0; i < 7; i++) {
        lbl_daily_day[i] = nullptr;
        lbl_daily_high[i] = nullptr;
        lbl_daily_low[i] = nullptr;
        img_daily[i] = nullptr;
        lbl_hourly[i] = nullptr;
        lbl_precipitation_probability[i] = nullptr;
        lbl_hourly_temp[i] = nullptr;
        img_hourly[i] = nullptr;
    }
}

void UI::createMainScreen() {
    // Clean up any existing main screen first
    cleanupMainScreen();
    
    // Create main screen
    main_screen = lv_obj_create(NULL);
    if (!main_screen) {
        Serial.println("ERROR: Failed to create main screen");
        return;
    }
    Serial.println("Main screen created successfully");
    
    // Set background to dark gradient (like original design)
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x2c5282), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(main_screen, lv_color_hex(0x1a202c), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(main_screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
    Serial.println("Main screen background gradient applied");
    
    // Add event handler for screen interactions
    lv_obj_add_event_cb(main_screen, screenEventHandler, LV_EVENT_CLICKED, NULL);
    
    // Create UI elements with error checking
    if (!createTemperatureDisplay()) {
        Serial.println("ERROR: Failed to create temperature display");
        return;
    }
    
    if (!createClockDisplay()) {
        Serial.println("ERROR: Failed to create clock display");
        return;
    }
    
    if (!createWeatherIcon()) {
        Serial.println("ERROR: Failed to create weather icon");
        return;
    }
    
    // Create forecast sections
    createDailyForecastBox();
    createHourlyForecastBox();
    
    // Load the screen
    lv_screen_load(main_screen);
    Serial.println("Main screen loaded and displayed");
}

bool UI::createTemperatureDisplay() {
    // Temperature label (large) - original v1.0.1 position
    lbl_today_temp = lv_label_create(main_screen);
    if (!lbl_today_temp) {
        Serial.println("ERROR: Failed to create temperature label");
        return false;
    }
    
    lv_obj_set_style_text_font(lbl_today_temp, get_font_42(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_today_temp, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(lbl_today_temp, get_strings()->temp_placeholder);
    lv_obj_align(lbl_today_temp, LV_ALIGN_TOP_MID, 45, 25); // Original v1.0.1 position
    
    // Feels like label - original v1.0.1 position  
    lbl_today_feels_like = lv_label_create(main_screen);
    if (!lbl_today_feels_like) {
        Serial.println("ERROR: Failed to create feels-like label");
        return false;
    }
    
    lv_obj_set_style_text_font(lbl_today_feels_like, get_font_14(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_today_feels_like, lv_color_hex(0xe4ffff), LV_PART_MAIN);
    lv_label_set_text(lbl_today_feels_like, ""); // Start empty, will be set by updateTemperature
    lv_obj_align(lbl_today_feels_like, LV_ALIGN_TOP_MID, 45, 75); // Original v1.0.1 position
    
    Serial.println("Temperature display created successfully");
    return true;
}

bool UI::createClockDisplay() {
    // Clock label - original v1.0.1 position in top-right corner
    lbl_clock = lv_label_create(main_screen);
    if (!lbl_clock) {
        Serial.println("ERROR: Failed to create clock label");
        return false;
    }
    
    lv_obj_set_style_text_font(lbl_clock, get_font_14(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_clock, lv_color_hex(0xb9ecff), LV_PART_MAIN);
    lv_label_set_text(lbl_clock, "");
    lv_obj_align(lbl_clock, LV_ALIGN_TOP_RIGHT, -10, 5); // Original v1.0.1 position
    
    Serial.println("Clock display created successfully");
    return true;
}

bool UI::createWeatherIcon() {
    // Today's weather icon - original v1.0.1 position
    img_today_icon = lv_image_create(main_screen);
    if (!img_today_icon) {
        Serial.println("ERROR: Failed to create weather icon");
        return false;
    }
    
    lv_obj_align(img_today_icon, LV_ALIGN_TOP_MID, -64, 4); // Original v1.0.1 position
    
    Serial.println("Weather icon created successfully");
    return true;
}

void UI::createWiFiConfigScreen() {
    // Clean up main screen if it exists
    cleanupMainScreen();
    
    // Create WiFi configuration screen
    lv_obj_t* wifi_screen = lv_obj_create(NULL);
    if (!wifi_screen) {
        Serial.println("ERROR: Failed to create WiFi config screen");
        return;
    }
    
    lv_obj_set_style_bg_color(wifi_screen, lv_color_hex(0x2c5282), LV_PART_MAIN);
    
    // Title
    lv_obj_t* lbl_title = lv_label_create(wifi_screen);
    if (lbl_title) {
        lv_obj_set_style_text_font(lbl_title, get_font_20(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_title, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_align(lbl_title, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_text(lbl_title, get_strings()->wifi_config);
        lv_obj_align(lbl_title, LV_ALIGN_TOP_MID, 0, 40);
    }
    
    // Instructions
    lv_obj_t* lbl_instructions = lv_label_create(wifi_screen);
    if (lbl_instructions) {
        lv_obj_set_style_text_font(lbl_instructions, get_font_14(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_instructions, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_align(lbl_instructions, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_width(lbl_instructions, SCREEN_WIDTH - 40);
        lv_label_set_text(lbl_instructions, 
            "1. Connect to 'Aura' WiFi\n"
            "2. Open browser\n" 
            "3. Go to 192.168.4.1\n"
            "4. Configure your WiFi");
        lv_obj_center(lbl_instructions);
    }
    
    // Load the WiFi screen
    lv_screen_load(wifi_screen);
    Serial.println("WiFi configuration screen displayed");
}

void UI::createDailyForecastBox() {
    // Create forecast title label - original v1.0.1 style
    lbl_forecast = lv_label_create(main_screen);
    if (lbl_forecast) {
        lv_label_set_text(lbl_forecast, get_strings()->seven_day_forecast);
        lv_obj_set_style_text_font(lbl_forecast, get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_forecast, lv_color_hex(0xe4ffff), LV_PART_MAIN);
        lv_obj_align(lbl_forecast, LV_ALIGN_TOP_LEFT, 20, 110); // Original v1.0.1 position
    }
    
    // Daily forecast container - original v1.0.1 styling
    box_daily = lv_obj_create(main_screen);
    if (!box_daily) {
        Serial.println("ERROR: Failed to create daily forecast box");
        return;
    }
    
    // Apply original v1.0.1 styling
    lv_obj_set_size(box_daily, 220, 180); // Original v1.0.1 size
    lv_obj_align(box_daily, LV_ALIGN_TOP_LEFT, 10, 135); // Original v1.0.1 position
    lv_obj_set_style_bg_color(box_daily, lv_color_hex(0x5e9bc8), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(box_daily, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(box_daily, 4, LV_PART_MAIN);
    lv_obj_set_style_border_width(box_daily, 0, LV_PART_MAIN);
    lv_obj_clear_flag(box_daily, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(box_daily, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(box_daily, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(box_daily, 0, LV_PART_MAIN);
    
    // Add click event to toggle to hourly view
    lv_obj_add_event_cb(box_daily, screenEventHandler, LV_EVENT_CLICKED, NULL);
    
    // Create vertical list of daily forecast items (original v1.0.1 layout)
    for (int i = 0; i < 7; i++) {
        // Day label - left aligned
        lbl_daily_day[i] = lv_label_create(box_daily);
        if (lbl_daily_day[i]) {
            lv_obj_set_style_text_font(lbl_daily_day[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_daily_day[i], lv_color_white(), LV_PART_MAIN);
            lv_obj_align(lbl_daily_day[i], LV_ALIGN_TOP_LEFT, 2, i * 24); // Original spacing
            
            const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            lv_label_set_text(lbl_daily_day[i], days[i]);
        }
        
        // High temperature - right aligned
        lbl_daily_high[i] = lv_label_create(box_daily);
        if (lbl_daily_high[i]) {
            lv_obj_set_style_text_font(lbl_daily_high[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_daily_high[i], lv_color_white(), LV_PART_MAIN);
            lv_obj_align(lbl_daily_high[i], LV_ALIGN_TOP_RIGHT, 0, i * 24); // Original position
            lv_label_set_text(lbl_daily_high[i], "24°");
        }
        
        // Low temperature - right side with offset
        lbl_daily_low[i] = lv_label_create(box_daily);
        if (lbl_daily_low[i]) {
            lv_obj_set_style_text_font(lbl_daily_low[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_daily_low[i], lv_color_hex(0xb9ecff), LV_PART_MAIN);
            lv_obj_align(lbl_daily_low[i], LV_ALIGN_TOP_RIGHT, -50, i * 24); // Original position
            lv_label_set_text(lbl_daily_low[i], "18°");
        }
        
        // Weather icon - center left
        img_daily[i] = lv_image_create(box_daily);
        if (img_daily[i]) {
            lv_obj_align(img_daily[i], LV_ALIGN_TOP_LEFT, 72, i * 24); // Original position
            // Set default icon (will be updated by weather data)
            // lv_img_set_src(img_daily[i], &icon_partly_cloudy);
        }
    }
    
    Serial.println("Daily forecast box created with original v1.0.1 layout");
}

void UI::createHourlyForecastBox() {
    // Hourly forecast container - initially hidden (original v1.0.1 behavior)
    box_hourly = lv_obj_create(main_screen);
    if (!box_hourly) {
        Serial.println("ERROR: Failed to create hourly forecast box");
        return;
    }
    
    // Apply same styling as daily box
    lv_obj_set_size(box_hourly, 220, 180); // Same size as daily
    lv_obj_align(box_hourly, LV_ALIGN_TOP_LEFT, 10, 135); // Same position as daily
    lv_obj_set_style_bg_color(box_hourly, lv_color_hex(0x5e9bc8), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(box_hourly, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(box_hourly, 4, LV_PART_MAIN);
    lv_obj_set_style_border_width(box_hourly, 0, LV_PART_MAIN);
    lv_obj_clear_flag(box_hourly, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(box_hourly, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(box_hourly, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(box_hourly, 0, LV_PART_MAIN);
    
    // Add click event to toggle back to daily view
    lv_obj_add_event_cb(box_hourly, screenEventHandler, LV_EVENT_CLICKED, NULL);
    
    // Create vertical list of hourly forecast items (original v1.0.1 layout)
    for (int i = 0; i < 7; i++) {
        // Hour label - left aligned
        lbl_hourly[i] = lv_label_create(box_hourly);
        if (lbl_hourly[i]) {
            lv_obj_set_style_text_font(lbl_hourly[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_hourly[i], lv_color_white(), LV_PART_MAIN);
            lv_obj_align(lbl_hourly[i], LV_ALIGN_TOP_LEFT, 2, i * 24); // Original spacing
            
            char hour_str[8];
            snprintf(hour_str, sizeof(hour_str), "%02d:00", (i * 3) % 24);
            lv_label_set_text(lbl_hourly[i], hour_str);
        }
        
        // Temperature - right aligned
        lbl_hourly_temp[i] = lv_label_create(box_hourly);
        if (lbl_hourly_temp[i]) {
            lv_obj_set_style_text_font(lbl_hourly_temp[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_hourly_temp[i], lv_color_white(), LV_PART_MAIN);
            lv_obj_align(lbl_hourly_temp[i], LV_ALIGN_TOP_RIGHT, 0, i * 24); // Original position
            lv_label_set_text(lbl_hourly_temp[i], "22°");
        }
        
        // Precipitation probability - right side with offset
        lbl_precipitation_probability[i] = lv_label_create(box_hourly);
        if (lbl_precipitation_probability[i]) {
            lv_obj_set_style_text_font(lbl_precipitation_probability[i], get_font_16(), LV_PART_MAIN);
            lv_obj_set_style_text_color(lbl_precipitation_probability[i], lv_color_hex(0xb9ecff), LV_PART_MAIN);
            lv_obj_align(lbl_precipitation_probability[i], LV_ALIGN_TOP_RIGHT, -55, i * 24); // Original position
            lv_label_set_text(lbl_precipitation_probability[i], "10%");
        }
        
        // Weather icon - center left
        img_hourly[i] = lv_image_create(box_hourly);
        if (img_hourly[i]) {
            lv_obj_align(img_hourly[i], LV_ALIGN_TOP_LEFT, 72, i * 24); // Original position
            // Set default icon (will be updated by weather data)
            // lv_img_set_src(img_hourly[i], &icon_partly_cloudy);
        }
    }
    
    // Start with hourly box hidden (original v1.0.1 behavior)
    lv_obj_add_flag(box_hourly, LV_OBJ_FLAG_HIDDEN);
    
    Serial.println("Hourly forecast box created with original v1.0.1 layout");
}

void UI::createSettingsWindow() {
    // Implementation for settings window would be created here
    // For now, just create a simple placeholder
    Serial.println("Settings window creation not yet implemented");
}

void UI::createLocationWindow() {
    // Implementation for location window would be created here  
    // For now, just create a simple placeholder
    Serial.println("Location window creation not yet implemented");
}

void UI::updateWeatherData(const JsonDocument& weatherData) {
    // Implementation for updating weather data
    // This would parse the JSON and update all UI elements
    Serial.println("Weather data update not yet implemented");
}

void UI::updateClock() {
    if (lbl_clock) {
        // Update clock display
        time_t now;
        time(&now);
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);
        
        char timeStr[16];
        if (use_24_hour) {
            strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
        } else {
            strftime(timeStr, sizeof(timeStr), "%I:%M %p", &timeinfo);
        }
        
        lv_label_set_text(lbl_clock, timeStr);
    }
}

void UI::updateTemperature(float temp, float feelsLike) {
    if (lbl_today_temp) {
        lv_label_set_text(lbl_today_temp, formatTemperature(temp).c_str());
    }
    
    if (lbl_today_feels_like) {
        String feelsLikeText = String(get_strings()->feels_like_temp) + " " + formatTemperature(feelsLike);
        lv_label_set_text(lbl_today_feels_like, feelsLikeText.c_str());
    }
}

void UI::updateForecast(const JsonArray& daily, const JsonArray& hourly) {
    // Implementation for updating forecast data
    // This would iterate through the arrays and update the forecast elements
    Serial.println("Forecast update not yet implemented");
}

void UI::updateLocation(const String& locationName) {
    // Implementation for updating location display
    Serial.println("Location update not yet implemented");
}

void UI::updateBackground(int wmo_code, int is_day) {
    if (main_screen) {
        const lv_image_dsc_t* bg_image = choose_image(wmo_code, is_day);
        if (bg_image) {
            lv_obj_set_style_bg_image_src(main_screen, bg_image, LV_PART_MAIN);
        }
    }
}

String UI::formatTime(int hour) {
    const LocalizedStrings* strings = get_strings();
    
    if (use_24_hour) {
        return String(hour) + ":00";
    } else {
        if (hour == 0) return "12" + String(strings->am);
        if (hour == 12) return String(strings->noon);
        if (hour < 12) return String(hour) + String(strings->am);
        return String(hour - 12) + String(strings->pm);
    }
}

String UI::formatTemperature(float temp) {
    int displayTemp = use_fahrenheit ? (int)(temp * 9.0 / 5.0 + 32) : (int)temp;
    String unit = use_fahrenheit ? "°F" : "°C";
    return String(displayTemp) + unit;
}

// Static event handlers
void UI::settingsEventHandler(lv_event_t* e) {
    if (instance) {
        // Handle settings events
    }
}

void UI::screenEventHandler(lv_event_t* e) {
    if (instance) {
        // Handle screen touch events
    }
}

void UI::locationEventHandler(lv_event_t* e) {
    if (instance) {
        // Handle location window events
    }
} 