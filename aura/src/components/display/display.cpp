#include "display.h"

// Static instance for callbacks
Display* Display::instance = nullptr;

Display::Display() : 
    touchscreenSPI(VSPI),
    touchscreen(XPT2046_CS, XPT2046_IRQ),
    display(nullptr),
    indev(nullptr) {
    instance = this;
}

Display::~Display() {
    instance = nullptr;
}

bool Display::init() {
    // Initialize TFT display
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    
    // Setup touchscreen
    setupTouchscreen();
    
    // Setup LVGL
    setupLVGL();
    
    // Set default backlight
    setBacklight(255);
    
    return true;
}

void Display::setupTouchscreen() {
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(0);
}

void Display::setupLVGL() {
    // Initialize LVGL
    lv_init();
    
    // Create display using LVGL 9.x API
    display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Setup display buffers using new API
    lv_display_set_buffers(display, draw_buf, NULL, DRAW_BUF_SIZE * sizeof(uint32_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // Set display flush callback
    lv_display_set_flush_cb(display, disp_flush_cb);
    
    // Setup input device (touchscreen) using new API
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
}

void Display::flush(const lv_area_t *area, uint8_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();
    
    lv_display_flush_ready(display);
}

void Display::touchRead(lv_indev_data_t *data) {
    if (touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        
        // Map touch coordinates to screen coordinates
        int x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        int y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x;
        data->point.y = y;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void Display::setBacklight(uint8_t brightness) {
    analogWrite(LCD_BACKLIGHT_PIN, brightness);
}

void Display::task() {
    lv_timer_handler();
}

// Static callback functions
void Display::disp_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    if (instance) {
        instance->flush(area, color_p);
    }
}

void Display::touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    if (instance) {
        instance->touchRead(data);
    }
} 