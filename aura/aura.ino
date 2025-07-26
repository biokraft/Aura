// Aura Weather Display - Main Application
// Modular Architecture - Simplified Main File

#include "src/components/display/display.h"
#include "src/components/ui/ui.h"
#include "src/components/weather/weather.h"
#include "src/config.h"

#include "esp_system.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include <time.h>

// Global component instances
Display display;
UI ui;
Weather weather;

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

// Language string definitions - localized strings
static const LocalizedStrings en_strings = {
    .temp_placeholder = "22°C",
    .feels_like_temp = "Feels Like", // Just the prefix, temperature will be appended
    .seven_day_forecast = "7 day forecast",
    .hourly_forecast = "Hourly forecast",
    .today = "Today",
    .now = "Now",
    .am = "AM",
    .pm = "PM",
    .noon = "12PM",
    .invalid_hour = "??",
    .brightness = "Brightness:",
    .location = "Location:",
    .use_fahrenheit = "°F",
    .use_24hr = "24H",
    .save = "Save",
    .cancel = "Cancel",
    .close = "Close",
    .location_btn = "Change",
    .reset_wifi = "Reset WiFi",
    .reset = "Reset",
    .change_location = "Change Location",
    .aura_settings = "Aura Settings",
    .city = "City:",
    .search_results = "Search Results:",
    .city_placeholder = "e.g. London",
    .wifi_config = "Configuring WiFi...",
    .reset_confirmation = "Reset WiFi settings?",
    .language_label = "Language:",
    .weekdays = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}};

static const LocalizedStrings es_strings = {
    .temp_placeholder = "22°C",
    .feels_like_temp = "Se siente como",  // Just prefix
    .seven_day_forecast = "Pronóstico de 7 días",
    .hourly_forecast = "Pronóstico por horas",
    .today = "Hoy",
    .now = "Ahora",
    .am = "AM",
    .pm = "PM",
    .noon = "12PM",
    .invalid_hour = "??",
    .brightness = "Brillo:",
    .location = "Ubicación:",
    .use_fahrenheit = "°F",
    .use_24hr = "24H",
    .save = "Guardar",
    .cancel = "Cancelar",
    .close = "Cerrar",
    .location_btn = "Cambiar",
    .reset_wifi = "Reiniciar WiFi",
    .reset = "Reiniciar",
    .change_location = "Cambiar Ubicación",
    .aura_settings = "Configuración Aura",
    .city = "Ciudad:",
    .search_results = "Resultados:",
    .city_placeholder = "ej. Madrid",
    .wifi_config = "Configurando WiFi...",
    .reset_confirmation = "¿Reiniciar configuración WiFi?",
    .language_label = "Idioma:",
    .weekdays = {"Dom", "Lun", "Mar", "Mié", "Jue", "Vie", "Sáb"}};

static const LocalizedStrings de_strings = {
  .temp_placeholder = "22°C",
  .feels_like_temp = "Gefühlt wie",  // Just prefix
  .seven_day_forecast = "7-Tage-Vorhersage",
  .hourly_forecast = "Stündliche Vorhersage",
  .today = "Heute",
  .now = "Jetzt",
  .am = "",
  .pm = "",
  .noon = "12:00",
  .invalid_hour = "??",
  .brightness = "Helligkeit:",
  .location = "Standort:",
  .use_fahrenheit = "°F",
  .use_24hr = "24H",
  .save = "Speichern",
  .cancel = "Abbrechen",
  .close = "Schließen",
  .location_btn = "Ändern",
  .reset_wifi = "WiFi zurücksetzen",
  .reset = "Zurücksetzen",
  .change_location = "Standort ändern",
  .aura_settings = "Aura Einstellungen",
  .city = "Stadt:",
  .search_results = "Suchergebnisse:",
  .city_placeholder = "z.B. Berlin",
  .wifi_config = "WiFi konfigurieren...",
  .reset_confirmation = "WiFi-Einstellungen zurücksetzen?",
  .language_label = "Sprache:",
  .weekdays = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"}
};

static const LocalizedStrings fr_strings = {
  .temp_placeholder = "22°C",
  .feels_like_temp = "Ressenti",  // Just prefix
  .seven_day_forecast = "Prévisions 7 jours",
  .hourly_forecast = "Prévisions horaires",
  .today = "Aujourd'hui",
  .now = "Maintenant",
  .am = "",
  .pm = "",
  .noon = "12:00",
  .invalid_hour = "??",
  .brightness = "Luminosité:",
  .location = "Lieu:",
  .use_fahrenheit = "°F",
  .use_24hr = "24H",
  .save = "Sauvegarder",
  .cancel = "Annuler",
  .close = "Fermer",
  .location_btn = "Changer",
  .reset_wifi = "Reset WiFi",
  .reset = "Reset",
  .change_location = "Changer lieu",
  .aura_settings = "Paramètres Aura",
  .city = "Ville:",
  .search_results = "Résultats:",
  .city_placeholder = "ex. Paris",
  .wifi_config = "Configuration WiFi...",
  .reset_confirmation = "Reset paramètres WiFi?",
  .language_label = "Langue:",
  .weekdays = {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"}
};

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Aura Weather Display Starting...");

  // Initialize display first
  if (!display.init()) {
    Serial.println("ERROR: Display initialization failed!");
    while (1)
      ; // Halt on critical error
  }
  Serial.println("Display initialized successfully");

  // Initialize UI now that LVGL is confirmed working
  if (!ui.init(&display)) {
    Serial.println("ERROR: UI initialization failed!");
    while (1)
      ; // Halt on critical error
  }
  Serial.println("UI initialized successfully");

  // Show WiFi configuration screen first
  ui.createWiFiConfigScreen();
  lv_refr_now(NULL); // Force immediate display update
  Serial.println("WiFi config screen displayed");

  // Initialize weather component
  if (!weather.init()) {
    Serial.println("ERROR: Weather initialization failed!");
    while (1)
      ; // Halt on critical error
  }
  Serial.println("Weather initialized successfully");

  // Initialize WiFi with proper configuration
  Serial.println("Configuring WiFi...");
  
  // Set unique device hostname
  String uniqueHostname = generateUniqueHostname();
  WiFi.setHostname(uniqueHostname.c_str());
  Serial.println("Device hostname: " + uniqueHostname);
  
  // Configure WiFiManager
  WiFiManager wifiManager;
  wifiManager.setAPCallback(apModeCallback);
  
  // Try to connect, will create AP if no saved credentials
  if (!wifiManager.autoConnect(DEFAULT_CAPTIVE_SSID)) {
    Serial.println("Failed to connect to WiFi");
    // Restart and try again
    ESP.restart();
  }
  
  Serial.println("WiFi connected successfully");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure time
  configTime(0, 0, "pool.ntp.org");
  Serial.println("Time configuration completed");

  // Now create the main screen and populate with data
  ui.createMainScreen();

  // Set proper data to make the display look correct
  ui.updateTemperature(22.0, 24.0); // 22°C, feels like 24°C
  ui.updateLocation("London");

  // Force immediate redraw
  lv_refr_now(NULL);
  Serial.println("Main UI displayed with data");

  Serial.println("Aura Weather Display Ready!");
}

void loop() {
  unsigned long current_time = millis();

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
    if (weather.fetchWeatherData()) {
      Serial.println("Weather data updated successfully");
      // You could update UI with weather data here
      // ui.updateWeatherData(weather.getCurrentWeather());
    } else {
      Serial.println("Failed to fetch weather data");
    }
    last_weather_update = current_time;
  }

  // LVGL tick increment for timing
  lv_tick_inc(5);

  // Small delay to prevent watchdog issues
  delay(5);
}

// Helper function implementations
const LocalizedStrings *get_strings() {
  switch (current_language) {
    case LANG_ES: return &es_strings;
    case LANG_DE: return &de_strings;
    case LANG_FR: return &fr_strings;
    default: return &en_strings;
  }
}

const lv_font_t *get_font_12() {
  return &lv_font_montserrat_latin_12;
}

const lv_font_t *get_font_14() {
  return &lv_font_montserrat_latin_14;
}

const lv_font_t *get_font_16() {
  return &lv_font_montserrat_latin_16;
}

const lv_font_t *get_font_20() {
  return &lv_font_montserrat_latin_20;
}

const lv_font_t *get_font_42() {
  return &lv_font_montserrat_latin_42;
}

// WiFi callback functions
void apModeCallback(WiFiManager *mgr) {
  Serial.println("Entering WiFi AP Mode");
  // Update WiFi config screen to show AP mode instructions
  ui.createWiFiConfigScreen();
  lv_refr_now(NULL);
}

String generateUniqueHostname() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  
  // Create a simple hash from MAC address
  uint32_t hash = 0;
  for (int i = 0; i < 6; i++) {
    hash = hash * 31 + mac[i];
  }
  
  // Convert to 5-character alphanumeric string
  String hashStr = "";
  const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  for (int i = 0; i < 5; i++) {
    hashStr += chars[hash % 36];
    hash /= 36;
  }
  
  return String(DEVICE_HOSTNAME) + "-" + hashStr;
}

const lv_image_dsc_t *choose_image(int wmo_code, int is_day) {
  switch (wmo_code) {
    // Clear sky
    case 0:
      return is_day ? &image_sunny : &image_clear_night;
    // Mainly clear
    case 1:
      return is_day ? &image_mostly_sunny : &image_mostly_clear_night;
    // Partly cloudy
    case 2:
      return is_day ? &image_partly_cloudy : &image_partly_cloudy_night;
    // Overcast
    case 3:
      return &image_cloudy;
    // Fog / mist
    case 45:
    case 48:
      return &image_haze_fog_dust_smoke;
    // Drizzle (light → dense)
    case 51:
    case 53:
    case 55:
      return &image_drizzle;
    // Freezing drizzle
    case 56:
    case 57:
      return &image_sleet_hail;
    // Rain: slight showers
    case 61:
      return is_day ? &image_scattered_showers_day : &image_scattered_showers_night;
    // Rain: moderate
    case 63:
      return &image_showers_rain;
    // Rain: heavy
    case 65:
      return &image_heavy_rain;
    // Freezing rain
    case 66:
    case 67:
      return &image_wintry_mix_rain_snow;
    // Snow fall (light, moderate, heavy) & snow showers (light)
    case 71:
    case 73:
    case 75:
    case 85:
      return &image_snow_showers_snow;
    // Snow grains
    case 77:
      return &image_flurries;
    // Rain showers (slight → moderate)
    case 80:
    case 81:
      return is_day ? &image_scattered_showers_day : &image_scattered_showers_night;
    // Rain showers: violent
    case 82:
      return &image_heavy_rain;
    // Heavy snow showers
    case 86:
      return &image_heavy_snow;
    // Thunderstorm (light)
    case 95:
      return is_day ? &image_isolated_scattered_tstorms_day
                    : &image_isolated_scattered_tstorms_night;
    // Thunderstorm with hail
    case 96:
    case 99:
      return &image_strong_tstorms;
    // Fallback for any other code
    default:
      return is_day ? &image_mostly_cloudy_day : &image_mostly_cloudy_night;
  }
}

const lv_image_dsc_t *choose_icon(int wmo_code, int is_day) {
  switch (wmo_code) {
    // Clear sky
    case 0:
      return is_day ? &icon_sunny : &icon_clear_night;
    // Mainly clear
    case 1:
      return is_day ? &icon_mostly_sunny : &icon_mostly_clear_night;
    // Partly cloudy
    case 2:
      return is_day ? &icon_partly_cloudy : &icon_partly_cloudy_night;
    // Overcast
    case 3:
      return &icon_cloudy;
    // Fog / mist
    case 45:
    case 48:
      return &icon_haze_fog_dust_smoke;
    // Drizzle (light → dense)
    case 51:
    case 53:
    case 55:
      return &icon_drizzle;
    // Freezing drizzle
    case 56:
    case 57:
      return &icon_sleet_hail;
    // Rain: slight showers
    case 61:
      return is_day ? &icon_scattered_showers_day : &icon_scattered_showers_night;
    // Rain: moderate
    case 63:
      return &icon_showers_rain;
    // Rain: heavy
    case 65:
      return &icon_heavy_rain;
    // Freezing rain
    case 66:
    case 67:
      return &icon_wintry_mix_rain_snow;
    // Snow fall (light, moderate, heavy) & snow showers (light)
    case 71:
    case 73:
    case 75:
    case 85:
      return &icon_snow_showers_snow;
    // Snow grains
    case 77:
      return &icon_flurries;
    // Rain showers (slight → moderate)
    case 80:
    case 81:
      return is_day ? &icon_scattered_showers_day : &icon_scattered_showers_night;
    // Rain showers: violent
    case 82:
      return &icon_heavy_rain;
    // Heavy snow showers
    case 86:
      return &icon_heavy_snow;
    // Thunderstorm (light)
    case 95:
      return is_day ? &icon_isolated_scattered_tstorms_day : &icon_isolated_scattered_tstorms_night;
    // Thunderstorm with hail
    case 96:
    case 99:
      return &icon_strong_tstorms;
    // Fallback for any other code
    default:
      return is_day ? &icon_mostly_cloudy_day : &icon_mostly_cloudy_night;
  }
}
