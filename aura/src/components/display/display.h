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
    void flush(const lv_area_t *area, uint8_t *color_p);
    void touchRead(lv_indev_data_t *data);
    void setBacklight(uint8_t brightness);
    
    // Task handling
    void task();
    
    // Getters
    TFT_eSPI& getTFT() { return tft; }
    XPT2046_Touchscreen& getTouchscreen() { return touchscreen; }
    lv_display_t* getDisplay() { return display; }
    
private:
    TFT_eSPI tft;
    SPIClass touchscreenSPI;
    XPT2046_Touchscreen touchscreen;
    
    // Optimized buffer configuration for ESP32 performance
    // Use smaller buffers in fast internal SRAM (30 lines worth)
    static constexpr int BUFFER_LINES = 30;
    static constexpr int BUFFER_SIZE = SCREEN_WIDTH * BUFFER_LINES;
    
    // Define DMA_ATTR if not already defined
    #ifndef DMA_ATTR
    #define DMA_ATTR DRAM_ATTR
    #endif
    
    static lv_color_t draw_buf_1[BUFFER_SIZE] DMA_ATTR;
    static lv_color_t draw_buf_2[BUFFER_SIZE] DMA_ATTR;  // Double buffer for better performance
    
    lv_display_t* display;
    lv_indev_t* indev;
    
    // Static callback functions for LVGL
    static void disp_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p);
    static void touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data);
    
    // Static instance pointer for callbacks
    static Display* instance;
};

#endif // DISPLAY_H 