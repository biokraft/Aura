#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../config.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>

class Display {
public:
    Display();
    ~Display();
    
    // Initialization
    bool init();
    void setupTouchscreen();
    void setupLVGL();
    
    // Display management
    void flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    void touchRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    void setBacklight(uint8_t brightness);
    
    // Task handling
    void task();
    
    // Getters
    TFT_eSPI& getTFT() { return tft; }
    XPT2046_Touchscreen& getTouchscreen() { return touchscreen; }
    
private:
    TFT_eSPI tft;
    SPIClass touchscreenSPI;
    XPT2046_Touchscreen touchscreen;
    uint32_t draw_buf[DRAW_BUF_SIZE / 4];
    
    // Static callback functions for LVGL
    static void disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static void touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    
    // Static instance pointer for callbacks
    static Display* instance;
};

#endif // DISPLAY_H 