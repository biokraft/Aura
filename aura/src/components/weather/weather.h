#ifndef WEATHER_H
#define WEATHER_H

#include "../../config.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>

struct WeatherData {
    float current_temp;
    float feels_like;
    int weather_code;
    bool is_day;
    String location_name;
    String description;
    
    // Arrays for forecast data
    float daily_high[7];
    float daily_low[7];
    int daily_codes[7];
    float hourly_temps[24];
    int hourly_codes[24];
    int hourly_precipitation[24];
    String hourly_times[24];
    String daily_days[7];
};

class Weather {
public:
    Weather();
    ~Weather();
    
    // Initialization
    bool init();
    
    // Weather data management
    bool fetchWeatherData();
    bool updateLocation(const String& lat, const String& lon, const String& locationName);
    bool searchLocations(const String& query, JsonArray& results);
    
    // Data access
    const WeatherData& getCurrentWeather() const { return currentWeather; }
    bool isDataValid() const { return dataValid; }
    unsigned long getLastUpdateTime() const { return lastUpdateTime; }
    
    // Settings management
    void loadSettings();
    void saveSettings();
    void setTemperatureUnit(bool fahrenheit) { use_fahrenheit = fahrenheit; }
    void setTimeFormat(bool hour24) { use_24_hour = hour24; }
    void setLanguage(Language lang) { current_language = lang; }
    
    // Location management
    String getLatitude() const { return String(latitude); }
    String getLongitude() const { return String(longitude); }
    String getLocationName() const { return location; }
    
private:
    WeatherData currentWeather;
    bool dataValid;
    unsigned long lastUpdateTime;
    
    // Settings storage
    Preferences prefs;
    
    // Location and settings
    char latitude[16];
    char longitude[16];
    String location;
    bool use_fahrenheit;
    bool use_24_hour;
    Language current_language;
    
    // Helper methods
    bool parseWeatherResponse(const String& response);
    String buildWeatherApiUrl();
    String buildGeocodingUrl(const String& query);
    void updateTimeAndLocation();
    int mapWeatherCode(int openMeteoCode);
    
    // Network helpers
    String makeHttpRequest(const String& url);
    bool isWiFiConnected();
};

#endif // WEATHER_H 