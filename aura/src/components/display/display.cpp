#include "display.h"

// Static instance for callbacks
Display* Display::instance = nullptr;

Display::Display() : 
    touchscreenSPI(VSPI),
    touchscreen(XPT2046_CS, XPT2046_IRQ) {
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
    
    // Setup display buffer
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, draw_buf, NULL, DRAW_BUF_SIZE / 4);
    
    // Setup display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Setup input device driver (touchscreen)
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_cb;
    lv_indev_drv_register(&indev_drv);
}

void Display::flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    tft.endWrite();
    
    lv_disp_flush_ready(disp_drv);
}

void Display::touchRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
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
    lv_task_handler();
}

// Static callback functions
void Display::disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    if (instance) {
        instance->flush(disp_drv, area, color_p);
    }
}

void Display::touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    if (instance) {
        instance->touchRead(indev_drv, data);
    }
} 