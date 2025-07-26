#ifndef WIFI_H
#define WIFI_H

#include "../../config.h"
#include <WiFi.h>
#include <WiFiManager.h>

class WiFiComponent {
public:
    WiFiComponent();
    ~WiFiComponent();
    
    // Initialization
    bool init();
    
    // WiFi management
    bool connect();
    bool isConnected() const;
    String getLocalIP() const;
    String getMACAddress() const;
    
    // Configuration
    void resetSettings();
    bool autoConnect(const char* ap_name = DEFAULT_CAPTIVE_SSID);
    
    // Hostname management
    String generateUniqueHostname() const;
    void setHostname(const String& hostname);
    
    // WiFi events
    static void onAPModeStarted(WiFiManager* wifiManager);
    
private:
    WiFiManager wifi_manager_;
    bool initialized_;
    String device_hostname_;
    
    // Helper methods
    void setupWiFiManager();
    uint32_t calculateMACHash() const;
};

#endif // WIFI_H 