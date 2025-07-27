// Aura Weather Display - DEBUG: Testing Display + UI Components
// Adding UI component to test touch screen functionality

#include "src/config.h"
#include "src/components/logging/logging.h"
#include "src/components/display/display.h"
#include "src/components/ui/ui.h"

#include <lvgl.h>
#include <WiFi.h>

// Global component instances
Display display;
UI ui;

// Global variables (required by config.h extern declarations)
Language current_language = LANG_EN;
bool use_fahrenheit = false;
bool use_24_hour = true;
char latitude[16] = LATITUDE_DEFAULT;
char longitude[16] = LONGITUDE_DEFAULT;
String location = LOCATION_DEFAULT;

void setup() {
    Serial.begin(115200);
    delay(3000); // Longer delay for serial stability
    
    Serial.println("\n=== DEBUG: Testing Display + UI Components ===");
    Serial.flush();
    delay(100);
    
    // Test 1: Basic logging
    Serial.println("DEBUG: Step 1 - Initializing logging...");
    Serial.flush();
    logging_init();
    LOG_MAIN_I("DEBUG: Logging system working!");
    LOG_MEMORY_INFO(TAG_MAIN);
    
    Serial.println("DEBUG: Step 2 - Initializing display...");
    Serial.flush();
    
    // Test 2: Display initialization
    if (!display.init()) {
        LOG_MAIN_E("DEBUG: Display initialization failed!");
        Serial.println("DEBUG: DISPLAY INIT FAILED - Halting");
        while(1) delay(1000);
    } else {
        LOG_MAIN_I("DEBUG: Display initialized successfully!");
        Serial.println("DEBUG: Display init SUCCESS");
        Serial.flush();
    }
    
    LOG_MEMORY_INFO(TAG_MAIN);
    
    Serial.println("DEBUG: Step 3 - Initializing UI...");
    Serial.flush();
    
    // Test 3: UI initialization
    if (!ui.init(&display)) {
        LOG_MAIN_E("DEBUG: UI initialization failed!");
        Serial.println("DEBUG: UI INIT FAILED - Halting");
        while(1) delay(1000);
    } else {
        LOG_MAIN_I("DEBUG: UI initialized successfully!");
        Serial.println("DEBUG: UI init SUCCESS");
        Serial.flush();
    }
    
    LOG_MEMORY_INFO(TAG_MAIN);
    
    Serial.println("DEBUG: Step 4 - Creating main screen with touch handlers...");
    Serial.flush();
    
    // Test 4: Create main screen with touch event handlers
    ui.createMainScreen();
    LOG_MAIN_I("DEBUG: Main screen created with touch handlers!");
    Serial.println("DEBUG: TOUCH SCREEN SHOULD WORK NOW - Try touching!");
    Serial.flush();
    
    LOG_MEMORY_INFO(TAG_MAIN);
    
    Serial.println("DEBUG: Step 5 - Starting heartbeat test (TOUCH THE SCREEN!)");
    Serial.flush();
}

unsigned long lastHeartbeat = 0;
unsigned long lastLVGLTick = 0;
int heartbeatCount = 0;

void loop() {
    unsigned long now = millis();
    
    // Handle LVGL timer tasks (required for touch processing)
    if (now - lastLVGLTick >= 5) {
        lv_tick_inc(now - lastLVGLTick);
        lastLVGLTick = now;
    }
    lv_timer_handler();
    
    // Simple heartbeat every 3 seconds (longer to make touch testing easier)
    if (now - lastHeartbeat > 3000) {
        heartbeatCount++;
        Serial.printf("DEBUG: Heartbeat %d - TOUCH SCREEN TO TEST!\n", heartbeatCount);
        Serial.flush();
        
        LOG_MAIN_I("DEBUG: Heartbeat %d - Touch screen ready", heartbeatCount);
        LOG_MEMORY_INFO(TAG_MAIN);
        
        lastHeartbeat = now;
        
        // Remind user to test touch
        if (heartbeatCount % 5 == 0) {
            Serial.println("DEBUG: === TOUCH THE SCREEN TO TEST EVENT HANDLERS ===");
            Serial.flush();
        }
    }
    
    delay(5); // Very small delay for responsive touch
}
