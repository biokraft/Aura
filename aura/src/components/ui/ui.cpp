#include "ui.h"
#include <Arduino.h>
#include <time.h>

// Static instance for callbacks
UI* UI::instance = nullptr;

UI::UI() : display_ref(nullptr), main_screen(nullptr), settings_win(nullptr), location_win(nullptr) {
    instance = this;
}

UI::~UI() {
    instance = nullptr;
}

bool UI::init(Display* display) {
    display_ref = display;
    
    // Create main screen
    createMainScreen();
    
    return true;
}

void UI::createMainScreen() {
    // Create main screen
    main_screen = lv_obj_create(NULL);
    Serial.println("Main screen created");
    
    // Set background to dark gradient (like original design)
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x2c5282), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(main_screen, lv_color_hex(0x1a202c), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(main_screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
    Serial.println("Main screen background gradient set");
    
    // Add event handler for screen interactions
    lv_obj_add_event_cb(main_screen, screenEventHandler, LV_EVENT_CLICKED, NULL);
    
    // Temperature label (large)
    lbl_today_temp = lv_label_create(main_screen);
    lv_obj_set_style_text_font(lbl_today_temp, get_font_42(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_today_temp, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(lbl_today_temp, get_strings()->temp_placeholder);
    lv_obj_align(lbl_today_temp, LV_ALIGN_CENTER, 0, -80);
    
    // Feels like label - start with empty text, will be populated by updateTemperature
    lbl_today_feels_like = lv_label_create(main_screen);
    lv_obj_set_style_text_font(lbl_today_feels_like, get_font_16(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_today_feels_like, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(lbl_today_feels_like, ""); // Start empty, will be set by updateTemperature
    lv_obj_align_to(lbl_today_feels_like, lbl_today_temp, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    
    // Today's weather icon
    img_today_icon = lv_image_create(main_screen);
    lv_obj_align(img_today_icon, LV_ALIGN_TOP_RIGHT, -10, 10);
    
    // Clock label
    lbl_clock = lv_label_create(main_screen);
    lv_obj_set_style_text_font(lbl_clock, get_font_20(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_clock, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(lbl_clock, LV_ALIGN_TOP_LEFT, 10, 10);
    
    // Create forecast sections
    createDailyForecastBox();
    createHourlyForecastBox();
    
    // Load the screen
    lv_screen_load(main_screen);
}

void UI::createWiFiConfigScreen() {
    // Create WiFi configuration screen
    lv_obj_t* wifi_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(wifi_screen, lv_color_hex(0x2c5282), LV_PART_MAIN);
    
    // WiFi config message
    lv_obj_t* lbl_wifi = lv_label_create(wifi_screen);
    lv_obj_set_style_text_font(lbl_wifi, get_font_20(), LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_wifi, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_wifi, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(lbl_wifi, get_strings()->wifi_config);
    lv_obj_center(lbl_wifi);
    
    // Load the WiFi screen
    lv_screen_load(wifi_screen);
}

void UI::createDailyForecastBox() {
    // Daily forecast container
    box_daily = lv_obj_create(main_screen);
    lv_obj_set_style_bg_opa(box_daily, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(box_daily, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(box_daily, SCREEN_WIDTH - 20, 80);
    lv_obj_align(box_daily, LV_ALIGN_BOTTOM_MID, 0, -100);
    
    // Create daily forecast items
    for (int i = 0; i < 7; i++) {
        int x_offset = i * 32 + 10;
        
        // Day label
        lbl_daily_day[i] = lv_label_create(box_daily);
        lv_obj_set_style_text_font(lbl_daily_day[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_daily_day[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_pos(lbl_daily_day[i], x_offset, 5);
        
        // Set placeholder day names
        const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        lv_label_set_text(lbl_daily_day[i], days[i]);
        
        // Weather icon
        img_daily[i] = lv_image_create(box_daily);
        lv_obj_set_pos(img_daily[i], x_offset, 20);
        
        // High temperature
        lbl_daily_high[i] = lv_label_create(box_daily);
        lv_obj_set_style_text_font(lbl_daily_high[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_daily_high[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_pos(lbl_daily_high[i], x_offset, 45);
        lv_label_set_text(lbl_daily_high[i], "24°");
        
        // Low temperature
        lbl_daily_low[i] = lv_label_create(box_daily);
        lv_obj_set_style_text_font(lbl_daily_low[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_daily_low[i], lv_color_make(180, 180, 180), LV_PART_MAIN);
        lv_obj_set_pos(lbl_daily_low[i], x_offset, 60);
        lv_label_set_text(lbl_daily_low[i], "18°");
    }
}

void UI::createHourlyForecastBox() {
    // Hourly forecast container
    box_hourly = lv_obj_create(main_screen);
    lv_obj_set_style_bg_opa(box_hourly, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(box_hourly, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(box_hourly, SCREEN_WIDTH - 20, 60);
    lv_obj_align(box_hourly, LV_ALIGN_BOTTOM_MID, 0, -40);
    
    // Create hourly forecast items
    for (int i = 0; i < 7; i++) {
        int x_offset = i * 32 + 10;
        
        // Hour label
        lbl_hourly[i] = lv_label_create(box_hourly);
        lv_obj_set_style_text_font(lbl_hourly[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_hourly[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_pos(lbl_hourly[i], x_offset, 5);
        
        // Set placeholder hours
        char hour_str[8];
        snprintf(hour_str, sizeof(hour_str), "%02d:00", (i * 3) % 24);
        lv_label_set_text(lbl_hourly[i], hour_str);
        
        // Weather icon
        img_hourly[i] = lv_image_create(box_hourly);
        lv_obj_set_pos(img_hourly[i], x_offset, 20);
        
        // Temperature
        lbl_hourly_temp[i] = lv_label_create(box_hourly);
        lv_obj_set_style_text_font(lbl_hourly_temp[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_hourly_temp[i], lv_color_white(), LV_PART_MAIN);
        lv_obj_set_pos(lbl_hourly_temp[i], x_offset, 40);
        lv_label_set_text(lbl_hourly_temp[i], "22°");
        
        // Precipitation probability
        lbl_precipitation_probability[i] = lv_label_create(box_hourly);
        lv_obj_set_style_text_font(lbl_precipitation_probability[i], get_font_12(), LV_PART_MAIN);
        lv_obj_set_style_text_color(lbl_precipitation_probability[i], lv_color_make(100, 150, 255), LV_PART_MAIN);
        lv_obj_set_pos(lbl_precipitation_probability[i], x_offset, 25);
        lv_label_set_text(lbl_precipitation_probability[i], "10%");
    }
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