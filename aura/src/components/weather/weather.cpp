#include "weather.h"
#include "../logging/logging.h"

Weather::Weather() : dataValid(false), lastUpdateTime(0) {
    // Initialize weather data
    memset(&currentWeather, 0, sizeof(WeatherData));
}

Weather::~Weather() {
    prefs.end();
}

bool Weather::init() {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_I("Initializing weather component...");
    
    // Initialize preferences
    if (!prefs.begin("aura_settings", false)) {
        LOG_WEATHER_E("Failed to initialize preferences storage");
        return false;
    }
    LOG_WEATHER_D("Preferences storage initialized");
    
    // Load saved settings
    loadSettings();
    LOG_WEATHER_I("Weather component initialized successfully");
    
    LOG_FUNCTION_EXIT(TAG_WEATHER);
    return true;
}

bool Weather::fetchWeatherData() {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_I("Fetching weather data...");
    
    if (!isWiFiConnected()) {
        LOG_WEATHER_W("WiFi not connected, skipping weather update");
        return false;
    }
    
    // Build API URL
    String apiUrl = buildWeatherApiUrl();
    LOG_WEATHER_D("API URL: %s", apiUrl.c_str());
    
    // Make HTTP request
    String response = makeHttpRequest(apiUrl);
    if (response.length() == 0) {
        LOG_WEATHER_E("Failed to fetch weather data - empty response");
        return false;
    }
    LOG_WEATHER_V("Raw weather response: %s", response.c_str());
    
    // Parse response
    bool success = parseWeatherResponse(response);
    if (success) {
        lastUpdateTime = millis();
        dataValid = true;
        LOG_WEATHER_I("Weather data updated successfully");
    } else {
        LOG_WEATHER_E("Failed to parse weather response");
    }
    
    LOG_FUNCTION_EXIT(TAG_WEATHER);
    return success;
}

bool Weather::updateLocation(const String& lat, const String& lon, const String& locationName) {
    // Update location variables
    strncpy(latitude, lat.c_str(), sizeof(latitude) - 1);
    strncpy(longitude, lon.c_str(), sizeof(longitude) - 1);
    location = locationName;
    
    // Save to preferences
    prefs.putString("latitude", lat);
    prefs.putString("longitude", lon);
    prefs.putString("location", locationName);
    
    // Invalidate current data to force refresh
    dataValid = false;
    
    return true;
}

bool Weather::searchLocations(const String& query, JsonArray& results) {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_I("Searching for locations: %s", query.c_str());
    
    if (!isWiFiConnected()) {
        LOG_WEATHER_W("WiFi not connected, cannot search locations");
        return false;
    }
    
    String geoUrl = buildGeocodingUrl(query);
    LOG_WEATHER_D("Geocoding URL: %s", geoUrl.c_str());
    String response = makeHttpRequest(geoUrl);
    
    if (response.length() == 0) {
        LOG_WEATHER_E("Failed to fetch geocoding data - empty response");
        return false;
    }
    
    // Parse geocoding response
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, response);
    
    if (error) {
        LOG_WEATHER_E("Failed to parse geocoding JSON: %s", error.c_str());
        return false;
    }
    
    if (doc["results"].is<JsonArray>()) {
        results = doc["results"].as<JsonArray>();
        LOG_WEATHER_I("Found %d location results", results.size());
        return true;
    }
    
    LOG_WEATHER_W("No location results found in response");
    return false;
}

void Weather::loadSettings() {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_D("Loading weather settings from preferences");
    
    // Load location settings
    String savedLat = prefs.getString("latitude", LATITUDE_DEFAULT);
    String savedLon = prefs.getString("longitude", LONGITUDE_DEFAULT);
    String savedLocation = prefs.getString("location", LOCATION_DEFAULT);
    
    strncpy(latitude, savedLat.c_str(), sizeof(latitude) - 1);
    strncpy(longitude, savedLon.c_str(), sizeof(longitude) - 1);
    location = savedLocation;
    
    // Load unit preferences
    use_fahrenheit = prefs.getBool("use_fahrenheit", false);
    use_24_hour = prefs.getBool("use_24_hour", false);
    
    // Load language preference
    current_language = (Language)prefs.getInt("language", LANG_EN);
    
    LOG_WEATHER_I("Settings loaded - Location: %s (%s, %s), Units: %s, Time: %s",
                  location.c_str(), latitude, longitude,
                  use_fahrenheit ? "°F" : "°C",
                  use_24_hour ? "24h" : "12h");
    LOG_FUNCTION_EXIT(TAG_WEATHER);
}

void Weather::saveSettings() {
    prefs.putString("latitude", String(latitude));
    prefs.putString("longitude", String(longitude));
    prefs.putString("location", location);
    prefs.putBool("use_fahrenheit", use_fahrenheit);
    prefs.putBool("use_24_hour", use_24_hour);
    prefs.putInt("language", (int)current_language);
}

bool Weather::parseWeatherResponse(const String& response) {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_D("Parsing weather response (%d bytes)", response.length());
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, response);
    
    if (error) {
        LOG_WEATHER_E("Failed to parse weather JSON: %s", error.c_str());
        return false;
    }
    LOG_WEATHER_D("Weather JSON parsed successfully");
    
    // Parse current weather
    if (doc["current"].is<JsonObject>()) {
        JsonObject current = doc["current"];
        currentWeather.current_temp = current["temperature_2m"];
        currentWeather.feels_like = current["apparent_temperature"];
        currentWeather.weather_code = current["weather_code"];
        currentWeather.is_day = current["is_day"];
    }
    
    // Parse daily forecast
    if (doc["daily"].is<JsonObject>()) {
        JsonObject daily = doc["daily"];
        JsonArray temps_high = daily["temperature_2m_max"];
        JsonArray temps_low = daily["temperature_2m_min"];
        JsonArray codes = daily["weather_code"];
        JsonArray times = daily["time"];
        
        for (int i = 0; i < 7 && i < temps_high.size(); i++) {
            currentWeather.daily_high[i] = temps_high[i];
            currentWeather.daily_low[i] = temps_low[i];
            currentWeather.daily_codes[i] = codes[i];
            
            // Parse and format day names
            String timeStr = times[i];
            // Convert ISO date to day name
            currentWeather.daily_days[i] = timeStr; // Simplified - would need proper date parsing
        }
    }
    
    // Parse hourly forecast
    if (doc["hourly"].is<JsonObject>()) {
        JsonObject hourly = doc["hourly"];
        JsonArray temps = hourly["temperature_2m"];
        JsonArray codes = hourly["weather_code"];
        JsonArray precipitation = hourly["precipitation_probability"];
        JsonArray times = hourly["time"];
        
        for (int i = 0; i < 24 && i < temps.size(); i++) {
            currentWeather.hourly_temps[i] = temps[i];
            currentWeather.hourly_codes[i] = codes[i];
            currentWeather.hourly_precipitation[i] = precipitation[i];
            currentWeather.hourly_times[i] = times[i].as<String>();
        }
    }
    
    currentWeather.location_name = location;
    
    return true;
}

String Weather::buildWeatherApiUrl() {
    String url = "https://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + String(latitude);
    url += "&longitude=" + String(longitude);
    url += "&current=temperature_2m,apparent_temperature,is_day,weather_code";
    url += "&daily=temperature_2m_max,temperature_2m_min,weather_code";
    url += "&hourly=temperature_2m,weather_code,precipitation_probability";
    url += "&timezone=auto";
    url += "&forecast_days=7";
    
    return url;
}

String Weather::buildGeocodingUrl(const String& query) {
    String url = "https://geocoding-api.open-meteo.com/v1/search";
    url += "?name=" + query;
    url += "&count=10";
    url += "&language=en";
    url += "&format=json";
    
    return url;
}

String Weather::makeHttpRequest(const String& url) {
    LOG_FUNCTION_ENTRY(TAG_WEATHER);
    LOG_WEATHER_D("Making HTTP request to: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.addHeader("User-Agent", "Aura Weather Display");
    
    int httpCode = http.GET();
    String response = "";
    
    if (httpCode == HTTP_CODE_OK) {
        response = http.getString();
        LOG_WEATHER_D("HTTP request successful, response size: %d bytes", response.length());
    } else {
        LOG_WEATHER_E("HTTP request failed with code: %d", httpCode);
    }
    
    http.end();
    LOG_FUNCTION_EXIT(TAG_WEATHER);
    return response;
}

bool Weather::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void Weather::updateTimeAndLocation() {
    // Configure time zone based on location
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
}

int Weather::mapWeatherCode(int openMeteoCode) {
    // Map Open-Meteo weather codes to internal codes
    // This would contain the mapping logic based on the WMO codes
    return openMeteoCode;
} 