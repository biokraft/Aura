#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <lvgl.h>

// Hardware Pin Definitions
#define XPT2046_IRQ 36  // T_IRQ
#define XPT2046_MOSI 32 // T_DIN
#define XPT2046_MISO 39 // T_OUT
#define XPT2046_CLK 25  // T_CLK
#define XPT2046_CS 33   // T_CS
#define LCD_BACKLIGHT_PIN 21

// Screen Configuration
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))

// Default Configuration Values
#define LATITUDE_DEFAULT "51.5074"
#define LONGITUDE_DEFAULT "-0.1278"
#define LOCATION_DEFAULT "London"
#define DEFAULT_CAPTIVE_SSID "Aura"
#define DEVICE_HOSTNAME "AuraSmartDevice"
#define UPDATE_INTERVAL 600000UL // 10 minutes

// Language Support
enum Language { LANG_EN = 0, LANG_ES = 1, LANG_DE = 2, LANG_FR = 3 };

struct LocalizedStrings {
  const char *temp_placeholder;
  const char *feels_like_temp;
  const char *seven_day_forecast;
  const char *hourly_forecast;
  const char *today;
  const char *now;
  const char *am;
  const char *pm;
  const char *noon;
  const char *invalid_hour;
  const char *brightness;
  const char *location;
  const char *use_fahrenheit;
  const char *use_24hr;
  const char *save;
  const char *cancel;
  const char *close;
  const char *location_btn;
  const char *reset_wifi;
  const char *reset;
  const char *change_location;
  const char *aura_settings;
  const char *city;
  const char *search_results;
  const char *city_placeholder;
  const char *wifi_config;
  const char *reset_confirmation;
  const char *language_label;
  const char *weekdays[7];
};

// Font Declarations
LV_FONT_DECLARE(lv_font_montserrat_latin_12);
LV_FONT_DECLARE(lv_font_montserrat_latin_14);
LV_FONT_DECLARE(lv_font_montserrat_latin_16);
LV_FONT_DECLARE(lv_font_montserrat_latin_20);
LV_FONT_DECLARE(lv_font_montserrat_latin_42);

// Weather Icon Declarations
LV_IMG_DECLARE(icon_blizzard);
LV_IMG_DECLARE(icon_blowing_snow);
LV_IMG_DECLARE(icon_clear_night);
LV_IMG_DECLARE(icon_cloudy);
LV_IMG_DECLARE(icon_drizzle);
LV_IMG_DECLARE(icon_flurries);
LV_IMG_DECLARE(icon_haze_fog_dust_smoke);
LV_IMG_DECLARE(icon_heavy_rain);
LV_IMG_DECLARE(icon_heavy_snow);
LV_IMG_DECLARE(icon_isolated_scattered_tstorms_day);
LV_IMG_DECLARE(icon_isolated_scattered_tstorms_night);
LV_IMG_DECLARE(icon_mostly_clear_night);
LV_IMG_DECLARE(icon_mostly_cloudy_day);
LV_IMG_DECLARE(icon_mostly_cloudy_night);
LV_IMG_DECLARE(icon_mostly_sunny);
LV_IMG_DECLARE(icon_partly_cloudy);
LV_IMG_DECLARE(icon_partly_cloudy_night);
LV_IMG_DECLARE(icon_scattered_showers_day);
LV_IMG_DECLARE(icon_scattered_showers_night);
LV_IMG_DECLARE(icon_showers_rain);
LV_IMG_DECLARE(icon_sleet_hail);
LV_IMG_DECLARE(icon_snow_showers_snow);
LV_IMG_DECLARE(icon_strong_tstorms);
LV_IMG_DECLARE(icon_sunny);
LV_IMG_DECLARE(icon_tornado);
LV_IMG_DECLARE(icon_wintry_mix_rain_snow);

// Weather Background Image Declarations
LV_IMG_DECLARE(image_blizzard);
LV_IMG_DECLARE(image_blowing_snow);
LV_IMG_DECLARE(image_clear_night);
LV_IMG_DECLARE(image_cloudy);
LV_IMG_DECLARE(image_drizzle);
LV_IMG_DECLARE(image_flurries);
LV_IMG_DECLARE(image_haze_fog_dust_smoke);
LV_IMG_DECLARE(image_heavy_rain);
LV_IMG_DECLARE(image_heavy_snow);
LV_IMG_DECLARE(image_isolated_scattered_tstorms_day);
LV_IMG_DECLARE(image_isolated_scattered_tstorms_night);
LV_IMG_DECLARE(image_mostly_clear_night);
LV_IMG_DECLARE(image_mostly_cloudy_day);
LV_IMG_DECLARE(image_mostly_cloudy_night);
LV_IMG_DECLARE(image_mostly_sunny);
LV_IMG_DECLARE(image_partly_cloudy);
LV_IMG_DECLARE(image_partly_cloudy_night);
LV_IMG_DECLARE(image_scattered_showers_day);
LV_IMG_DECLARE(image_scattered_showers_night);
LV_IMG_DECLARE(image_showers_rain);
LV_IMG_DECLARE(image_sleet_hail);
LV_IMG_DECLARE(image_snow_showers_snow);
LV_IMG_DECLARE(image_strong_tstorms);
LV_IMG_DECLARE(image_sunny);
LV_IMG_DECLARE(image_tornado);
LV_IMG_DECLARE(image_wintry_mix_rain_snow);

// Global Variables (declared as extern for use in other modules)
extern Language current_language;
extern bool use_fahrenheit;
extern bool use_24_hour;
extern char latitude[16];
extern char longitude[16];
extern String location;

// Function Declarations
// Language functions
const LocalizedStrings *get_strings();

// Font functions
const lv_font_t *get_font_12();
const lv_font_t *get_font_14();
const lv_font_t *get_font_16();
const lv_font_t *get_font_20();
const lv_font_t *get_font_42();

// Weather functions
const lv_image_dsc_t *choose_image(int wmo_code, int is_day);
const lv_image_dsc_t *choose_icon(int wmo_code, int is_day);

#endif // CONFIG_H 