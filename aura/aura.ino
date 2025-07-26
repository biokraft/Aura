// Aura Weather Display - Main Application
// Modular Architecture - Simplified Main File

#include "src/components/display/display.h"
#include "src/components/ui/ui.h"
#include "src/components/weather/weather.h"
#include "src/config.h"

#include "esp_system.h"
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

// Language support implementation
static const LocalizedStrings strings_en = {
    "--°C",
    "Feels Like",
    "SEVEN DAY FORECAST",
    "HOURLY FORECAST",
    "Today",
    "Now",
    "am",
    "pm",
    "Noon",
    "Invalid hour",
    "Brightness:",
    "Location:",
    "Use °F:",
    "24hr:",
    "Save",
    "Cancel",
    "Close",
    "Location",
    "Reset Wi-Fi",
    "Reset",
    "Change Location",
    "Aura Settings",
    "City:",
    "Search Results",
    "e.g. London",
    "Wi-Fi Configuration:\n\n"
    "Please connect your\n"
    "phone or laptop to the\n"
    "temporary Wi-Fi access\n point " DEFAULT_CAPTIVE_SSID "\n"
    "to configure.\n\n"
    "If you don't see a \n"
    "configuration screen \n"
    "after connecting,\n"
    "visit http://192.168.4.1\n"
    "in your web browser.",
    "Are you sure you want to reset "
    "Wi-Fi credentials?\n\n"
    "You'll need to reconnect to the Wifi SSID " DEFAULT_CAPTIVE_SSID
    " with your phone or browser to "
    "reconfigure Wi-Fi credentials.",
    "Language:",
    {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"}};

static const LocalizedStrings strings_es = {"--°C",
                                            "Sensación",
                                            "PRONÓSTICO 7 DÍAS",
                                            "PRONÓSTICO POR HORAS",
                                            "Hoy",
                                            "Ahora",
                                            "am",
                                            "pm",
                                            "Mediodía",
                                            "Hora inválida",
                                            "Brillo:",
                                            "Ubicación:",
                                            "Usar °F:",
                                            "24h:",
                                            "Guardar",
                                            "Cancelar",
                                            "Cerrar",
                                            "Ubicación",
                                            "Wi-Fi",
                                            "Restablecer",
                                            "Cambiar Ubicación",
                                            "Configuración Aura",
                                            "Ciudad:",
                                            "Resultados de Búsqueda",
                                            "ej. Madrid",
                                            "Configuración Wi-Fi:\n\n"
                                            "Conecte su teléfono\n"
                                            "o portátil al punto de\n"
                                            "acceso Wi-Fi temporal\n" DEFAULT_CAPTIVE_SSID "\n"
                                            "para configurar.\n\n"
                                            "Si no ve una pantalla\n"
                                            "de configuración después\n"
                                            "de conectarse, visite\n"
                                            "http://192.168.4.1\n"
                                            "en su navegador.",
                                            "¿Está seguro de que desea\n"
                                            "restablecer las credenciales\n"
                                            "Wi-Fi?\n\n"
                                            "Deberá reconectarse al SSID " DEFAULT_CAPTIVE_SSID
                                            " con su teléfono o navegador\n"
                                            "para reconfigurar las\n"
                                            "credenciales Wi-Fi.",
                                            "Idioma:",
                                            {"Dom", "Lun", "Mar", "Mié", "Jue", "Vie", "Sáb"}};

static const LocalizedStrings strings_de = {"--°C",
                                            "Gefühlt",
                                            "7-TAGE VORHERSAGE",
                                            "STÜNDLICHE VORHERSAGE",
                                            "Heute",
                                            "Jetzt",
                                            "",
                                            "",
                                            "Mittag",
                                            "Ungültige Stunde",
                                            "Helligkeit:",
                                            "Standort:",
                                            "°F:",
                                            "24h:",
                                            "Speichern",
                                            "Abbrechen",
                                            "Schließen",
                                            "Standort",
                                            "Wi-Fi",
                                            "Zurücksetzen",
                                            "Standort ändern",
                                            "Aura Einstellungen",
                                            "Stadt:",
                                            "Suchergebnisse",
                                            "z.B. Berlin",
                                            "Wi-Fi Konfiguration:\n\n"
                                            "Verbinden Sie Ihr Telefon\n"
                                            "oder Laptop mit dem\n"
                                            "temporären Wi-Fi\n"
                                            "Zugangspunkt " DEFAULT_CAPTIVE_SSID "\n"
                                            "zum Konfigurieren.\n\n"
                                            "Wenn Sie keinen\n"
                                            "Konfigurationsbildschirm\n"
                                            "sehen, besuchen Sie\n"
                                            "http://192.168.4.1\n"
                                            "in Ihrem Browser.",
                                            "Sind Sie sicher, dass Sie\n"
                                            "die Wi-Fi Zugangsdaten\n"
                                            "zurücksetzen möchten?\n\n"
                                            "Sie müssen sich erneut mit\n"
                                            "der SSID " DEFAULT_CAPTIVE_SSID " verbinden, um die\n"
                                            "Wi-Fi Zugangsdaten\n"
                                            "neu zu konfigurieren.",
                                            "Sprache:",
                                            {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"}};

static const LocalizedStrings strings_fr = {"--°C",
                                            "Ressenti",
                                            "PRÉVISIONS 7 JOURS",
                                            "PRÉVISIONS HORAIRES",
                                            "Aujourd'hui",
                                            "Maintenant",
                                            "h",
                                            "h",
                                            "Midi",
                                            "Heure invalide",
                                            "Luminosité:",
                                            "Lieu:",
                                            "Utiliser °F:",
                                            "24h:",
                                            "Sauvegarder",
                                            "Annuler",
                                            "Fermer",
                                            "Lieu",
                                            "Wi-Fi",
                                            "Réinitialiser",
                                            "Changer de lieu",
                                            "Paramètres Aura",
                                            "Ville:",
                                            "Résultats de recherche",
                                            "ex. Paris",
                                            "Configuration Wi-Fi:\n\n"
                                            "Connectez votre téléphone\n"
                                            "ou ordinateur portable au\n"
                                            "point d'accès Wi-Fi\n"
                                            "temporaire " DEFAULT_CAPTIVE_SSID "\n"
                                            "pour configurer.\n\n"
                                            "Si vous ne voyez pas\n"
                                            "d'écran de configuration\n"
                                            "après connexion, visitez\n"
                                            "http://192.168.4.1\n"
                                            "dans votre navigateur.",
                                            "Êtes-vous sûr de vouloir\n"
                                            "réinitialiser les\n"
                                            "identifiants Wi-Fi?\n\n"
                                            "Vous devrez vous reconnecter\n"
                                            "au SSID " DEFAULT_CAPTIVE_SSID
                                            " avec votre téléphone ou\n"
                                            "navigateur pour reconfigurer\n"
                                            "les identifiants Wi-Fi.",
                                            "Langue:",
                                            {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"}};

// Language support functions
const LocalizedStrings *get_strings() {
  switch (current_language) {
    case LANG_ES:
      return &strings_es;
    case LANG_DE:
      return &strings_de;
    case LANG_FR:
      return &strings_fr;
    default:
      return &strings_en;
  }
}

// Font selection functions
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

// Helper functions
String generateUniqueHostname() {
  uint8_t mac[6];
  WiFi.macAddress(mac);

  String hostname = DEVICE_HOSTNAME;
  hostname += "_";
  for (int i = 3; i < 6; i++) {
    if (mac[i] < 16)
      hostname += "0";
    hostname += String(mac[i], HEX);
  }
  hostname.toUpperCase();

  return hostname;
}

void apModeCallback(WiFiManager *mgr) {
  String uniqueHostname = generateUniqueHostname();
  WiFi.setHostname(uniqueHostname.c_str());

  // Show WiFi configuration screen
  lv_obj_t *scr = lv_scr_act();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x4c8cb9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(scr, lv_color_hex(0xa6cdec), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

  const LocalizedStrings *strings = get_strings();
  lv_obj_t *lbl = lv_label_create(scr);
  lv_label_set_text(lbl, strings->wifi_config);
  lv_obj_set_style_text_font(lbl, get_font_14(), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(lbl);
  lv_scr_load(scr);

  // Allow UI updates while in AP mode
  uint32_t start = millis();
  while (millis() - start < 200) {
    lv_timer_handler();
    delay(5);
  }
}

void update_clock(lv_timer_t *timer) {
  ui.updateClock();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialize display component
  if (!display.init()) {
    Serial.println("Failed to initialize display");
    return;
  }

  // Initialize weather component
  if (!weather.init()) {
    Serial.println("Failed to initialize weather");
    return;
  }

  // Initialize UI component
  if (!ui.init(&display)) {
    Serial.println("Failed to initialize UI");
    return;
  }

  // Set unique device hostname for network identification
  String uniqueHostname = generateUniqueHostname();
  WiFi.setHostname(uniqueHostname.c_str());
  Serial.println("Device hostname: " + uniqueHostname);

  // Setup WiFi
  WiFiManager wm;
  wm.setAPCallback(apModeCallback);
  wm.autoConnect(DEFAULT_CAPTIVE_SSID);

  // Setup periodic updates
  lv_timer_create(update_clock, 1000, NULL);

  // Initial weather fetch
  weather.fetchWeatherData();
  Serial.println("Setup complete");
}

void loop() {
  // Handle display updates
  display.task();

  // Check for weather data updates
  static uint32_t lastWeatherUpdate = millis();
  if (millis() - lastWeatherUpdate >= UPDATE_INTERVAL) {
    weather.fetchWeatherData();
    lastWeatherUpdate = millis();
  }

  // Small delay for stability
  lv_tick_inc(5);
  delay(5);
}

// Weather icon and image selection functions
const lv_img_dsc_t *choose_image(int wmo_code, int is_day) {
  switch (wmo_code) {
    case 0:
      return is_day ? &image_sunny : &image_clear_night;
    case 1:
      return is_day ? &image_mostly_sunny : &image_mostly_clear_night;
    case 2:
      return is_day ? &image_partly_cloudy : &image_partly_cloudy_night;
    case 3:
      return &image_cloudy;
    case 45:
    case 48:
      return &image_haze_fog_dust_smoke;
    case 51:
    case 53:
    case 55:
      return &image_drizzle;
    case 56:
    case 57:
      return &image_wintry_mix_rain_snow;
    case 61:
      return is_day ? &image_scattered_showers_day : &image_scattered_showers_night;
    case 63:
      return &image_showers_rain;
    case 65:
      return &image_heavy_rain;
    case 66:
    case 67:
      return &image_wintry_mix_rain_snow;
    case 71:
    case 73:
    case 75:
    case 85:
      return &image_snow_showers_snow;
    case 77:
      return &image_flurries;
    case 80:
    case 81:
      return is_day ? &image_scattered_showers_day : &image_scattered_showers_night;
    case 82:
      return &image_heavy_rain;
    case 86:
      return &image_heavy_snow;
    case 95:
      return is_day ? &image_isolated_scattered_tstorms_day
                    : &image_isolated_scattered_tstorms_night;
    case 96:
    case 99:
      return &image_strong_tstorms;
    default:
      return is_day ? &image_mostly_cloudy_day : &image_mostly_cloudy_night;
  }
}

const lv_img_dsc_t *choose_icon(int wmo_code, int is_day) {
  switch (wmo_code) {
    case 0:
      return is_day ? &icon_sunny : &icon_clear_night;
    case 1:
      return is_day ? &icon_mostly_sunny : &icon_mostly_clear_night;
    case 2:
      return is_day ? &icon_partly_cloudy : &icon_partly_cloudy_night;
    case 3:
      return &icon_cloudy;
    case 45:
    case 48:
      return &icon_haze_fog_dust_smoke;
    case 51:
    case 53:
    case 55:
      return &icon_drizzle;
    case 56:
    case 57:
      return &icon_wintry_mix_rain_snow;
    case 61:
      return is_day ? &icon_scattered_showers_day : &icon_scattered_showers_night;
    case 63:
      return &icon_showers_rain;
    case 65:
      return &icon_heavy_rain;
    case 66:
    case 67:
      return &icon_wintry_mix_rain_snow;
    case 71:
    case 73:
    case 75:
    case 85:
      return &icon_snow_showers_snow;
    case 77:
      return &icon_flurries;
    case 80:
    case 81:
      return is_day ? &icon_scattered_showers_day : &icon_scattered_showers_night;
    case 82:
      return &icon_heavy_rain;
    case 86:
      return &icon_heavy_snow;
    case 95:
      return is_day ? &icon_isolated_scattered_tstorms_day : &icon_isolated_scattered_tstorms_night;
    case 96:
    case 99:
      return &icon_strong_tstorms;
    default:
      return is_day ? &icon_mostly_cloudy_day : &icon_mostly_cloudy_night;
  }
}
