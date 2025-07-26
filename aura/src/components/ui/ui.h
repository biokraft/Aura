#ifndef UI_H
#define UI_H

#include "../../config.h"
#include "../display/display.h"
#include <ArduinoJson.h>
#include <lvgl.h>

class UI {
public:
    UI();
    ~UI();
    
    // Initialization
    bool init(Display* display);
    
    // Screen management
    void createMainScreen();
    void createWiFiConfigScreen();
    void createSettingsWindow();
    void createLocationWindow();
    
    // UI updates
    void updateWeatherData(const JsonDocument& weatherData);
    void updateClock();
    void updateTemperature(float temp, float feelsLike);
    void updateForecast(const JsonArray& daily, const JsonArray& hourly);
    void updateLocation(const String& locationName);
    
    // Event handlers
    static void settingsEventHandler(lv_event_t* e);
    static void screenEventHandler(lv_event_t* e);
    static void locationEventHandler(lv_event_t* e);
    
    // Getters
    lv_obj_t* getMainScreen() { return main_screen; }
    
private:
    Display* display_ref;
    
    // Main screen elements
    lv_obj_t* main_screen;
    lv_obj_t* lbl_today_temp;
    lv_obj_t* lbl_today_feels_like;
    lv_obj_t* img_today_icon;
    lv_obj_t* lbl_forecast;
    lv_obj_t* box_daily;
    lv_obj_t* box_hourly;
    lv_obj_t* lbl_daily_day[7];
    lv_obj_t* lbl_daily_high[7];
    lv_obj_t* lbl_daily_low[7];
    lv_obj_t* img_daily[7];
    lv_obj_t* lbl_hourly[7];
    lv_obj_t* lbl_precipitation_probability[7];
    lv_obj_t* lbl_hourly_temp[7];
    lv_obj_t* img_hourly[7];
    lv_obj_t* lbl_clock;
    
    // Settings window elements
    lv_obj_t* settings_win;
    lv_obj_t* unit_switch;
    lv_obj_t* clock_24hr_switch;
    lv_obj_t* language_dropdown;
    
    // Location window elements
    lv_obj_t* location_win;
    lv_obj_t* lbl_loc;
    lv_obj_t* loc_ta;
    lv_obj_t* results_dd;
    lv_obj_t* btn_close_loc;
    lv_obj_t* btn_close_obj;
    lv_obj_t* kb;
    
    // Helper methods
    void createDailyForecastBox();
    void createHourlyForecastBox();
    void updateBackground(int wmo_code, int is_day);
    String formatTime(int hour);
    String formatTemperature(float temp);
    
    // Static instance for callbacks
    static UI* instance;
};

#endif // UI_H 