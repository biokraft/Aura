#include "display.h"
#include "../logging/logging.h"

// Static instance for callbacks
Display* Display::instance = nullptr;

// Static buffer definitions - placed in fast internal SRAM
lv_color_t Display::draw_buf_1[Display::BUFFER_SIZE] DMA_ATTR;
lv_color_t Display::draw_buf_2[Display::BUFFER_SIZE] DMA_ATTR;

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
    LOG_FUNCTION_ENTRY(TAG_DISPLAY);
    LOG_DISPLAY_I("Starting display initialization...");
    
    // Initialize TFT display
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    LOG_DISPLAY_I("TFT display initialized successfully");
    
    // Setup touchscreen
    setupTouchscreen();
    LOG_DISPLAY_I("Touchscreen setup completed");
    
    // Setup LVGL
    setupLVGL();
    LOG_DISPLAY_I("LVGL setup completed");
    
    // Set default backlight
    setBacklight(255);
    LOG_DISPLAY_I("Display initialization completed successfully");
    LOG_MEMORY_INFO(TAG_DISPLAY);
    
    LOG_FUNCTION_EXIT(TAG_DISPLAY);
    return true;
}

void Display::setupTouchscreen() {
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(0);
}

void Display::setupLVGL() {
    LOG_FUNCTION_ENTRY(TAG_DISPLAY);
    
    // Initialize LVGL
    lv_init();
    LOG_DISPLAY_I("LVGL core initialized");
    
    // Create display using LVGL 9.x API
    display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        LOG_DISPLAY_E("Failed to create LVGL display");
        return;
    }
    LOG_DISPLAY_I("LVGL display created: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Setup optimized display buffers using double buffering for better performance
    // Buffer size in pixels (LVGL will handle color depth internally)
    size_t buffer_size_pixels = BUFFER_SIZE;
    lv_display_set_buffers(display, draw_buf_1, draw_buf_2, buffer_size_pixels, LV_DISPLAY_RENDER_MODE_PARTIAL);
    LOG_DISPLAY_I("LVGL buffers configured: %d pixels per buffer (double buffered)", buffer_size_pixels);
    
    // Set display flush callback
    lv_display_set_flush_cb(display, disp_flush_cb);
    LOG_DISPLAY_D("LVGL flush callback registered");
    
    // Setup input device (touchscreen) using new API
    indev = lv_indev_create();
    if (!indev) {
        LOG_DISPLAY_E("Failed to create LVGL input device");
        return;
    }
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
    LOG_DISPLAY_I("LVGL input device configured successfully");
    
    LOG_FUNCTION_EXIT(TAG_DISPLAY);
}

void Display::flush(const lv_area_t *area, uint8_t *color_p) {
    if (!area || !color_p) {
        LOG_DISPLAY_E("Invalid flush parameters - area=%p, color_p=%p", area, color_p);
        lv_display_flush_ready(display);
        return;
    }
    
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    // Validate area bounds
    if (area->x1 < 0 || area->y1 < 0 || area->x2 >= SCREEN_WIDTH || area->y2 >= SCREEN_HEIGHT) {
        LOG_DISPLAY_W("Invalid display area: (%d,%d) to (%d,%d)", 
                     area->x1, area->y1, area->x2, area->y2);
        lv_display_flush_ready(display);
        return;
    }
    
    // LVGL flush callback - optimized drawing to TFT
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    
    // LVGL 9.x with 16-bit color depth sends RGB565 data
    // Cast uint8_t* to uint16_t* for RGB565 format
    uint16_t* pixel_data = (uint16_t*)color_p;
    tft.pushColors(pixel_data, w * h, true);
    
    tft.endWrite();
    
    // Signal to LVGL that flushing is complete
    lv_display_flush_ready(display);
}

void Display::touchRead(lv_indev_data_t *data) {
    if (!data) {
        LOG_DISPLAY_E("Invalid touch data pointer");
        return;
    }
    
    if (touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        
        // Map touch coordinates to screen coordinates with bounds checking
        int x = map(p.x, 200, 3700, 0, SCREEN_WIDTH - 1);
        int y = map(p.y, 240, 3800, 0, SCREEN_HEIGHT - 1);
        
        // Clamp values to screen bounds
        x = constrain(x, 0, SCREEN_WIDTH - 1);
        y = constrain(y, 0, SCREEN_HEIGHT - 1);
        
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x;
        data->point.y = y;
        
        // DEBUG: Log every touch detection
        LOG_DISPLAY_I("TOUCH DETECTED: Raw(%d,%d) -> Screen(%d,%d)", p.x, p.y, x, y);
    } else {
        data->state = LV_INDEV_STATE_REL;
        data->point.x = 0;
        data->point.y = 0;
    }
}

void Display::setBacklight(uint8_t brightness) {
    analogWrite(LCD_BACKLIGHT_PIN, brightness);
}

void Display::task() {
    // Handle LVGL timer tasks
    lv_timer_handler();
}

// Static callback functions
void Display::disp_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    if (instance) {
        instance->flush(area, color_p);
    } else {
        LOG_DISPLAY_E("Display instance not available for flush callback");
        // Still need to signal completion to avoid LVGL hanging
        lv_display_flush_ready(disp);
    }
}

void Display::touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    if (instance) {
        instance->touchRead(data);
    } else {
        LOG_DISPLAY_E("Display instance not available for touch callback");
        // Provide safe fallback data
        if (data) {
            data->state = LV_INDEV_STATE_REL;
            data->point.x = 0;
            data->point.y = 0;
        }
    }
} 