#include "wifi.h"
#include "../logging/logging.h"
#include <Arduino.h>

WiFiComponent::WiFiComponent() : initialized_(false) {
    LOG_WIFI_D("WiFi component created");
}

WiFiComponent::~WiFiComponent() {
    LOG_WIFI_D("WiFi component destroyed");
}

bool WiFiComponent::init() {
    if (initialized_) {
        LOG_WIFI_W("WiFi component already initialized");
        return true;
    }
    
    LOG_WIFI_I("Initializing WiFi component...");
    
    // Generate unique hostname
    device_hostname_ = generateUniqueHostname();
    WiFi.setHostname(device_hostname_.c_str());
    LOG_WIFI_I("Device hostname set to: %s", device_hostname_.c_str());
    
    // Setup WiFiManager
    setupWiFiManager();
    
    initialized_ = true;
    LOG_WIFI_I("WiFi component initialized successfully");
    return true;
}

bool WiFiComponent::connect() {
    if (!initialized_) {
        LOG_WIFI_E("WiFi component not initialized");
        return false;
    }
    
    LOG_WIFI_I("Attempting WiFi connection...");
    
    // Try to connect, will create AP if no saved credentials
    if (!wifi_manager_.autoConnect(DEFAULT_CAPTIVE_SSID)) {
        LOG_WIFI_E("Failed to connect to WiFi");
        return false;
    }
    
    LOG_WIFI_I("WiFi connected successfully");
    LOG_WIFI_I("IP address: %s", WiFi.localIP().toString().c_str());
    return true;
}

bool WiFiComponent::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiComponent::getLocalIP() const {
    return WiFi.localIP().toString();
}

String WiFiComponent::getMACAddress() const {
    return WiFi.macAddress();
}

void WiFiComponent::resetSettings() {
    LOG_WIFI_I("Resetting WiFi settings...");
    wifi_manager_.resetSettings();
    LOG_WIFI_I("WiFi settings reset completed");
}

bool WiFiComponent::autoConnect(const char* ap_name) {
    if (!initialized_) {
        LOG_WIFI_E("WiFi component not initialized");
        return false;
    }
    
    return wifi_manager_.autoConnect(ap_name);
}

String WiFiComponent::generateUniqueHostname() const {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    
    // Create a simple hash from MAC address
    uint32_t hash = calculateMACHash();
    
    // Convert to 5-character alphanumeric string
    String hashStr = "";
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 5; i++) {
        hashStr += chars[hash % 36];
        hash /= 36;
    }
    
    return String(DEVICE_HOSTNAME) + "-" + hashStr;
}

void WiFiComponent::setHostname(const String& hostname) {
    device_hostname_ = hostname;
    WiFi.setHostname(hostname.c_str());
    LOG_WIFI_I("Hostname updated to: %s", hostname.c_str());
}

void WiFiComponent::onAPModeStarted(WiFiManager* wifiManager) {
    LOG_WIFI_I("WiFi AP Mode started");
    LOG_WIFI_I("AP Name: %s", DEFAULT_CAPTIVE_SSID);
    LOG_WIFI_I("Connect to configure WiFi settings");
    
    // Note: In the original code, this triggered a UI update
    // That functionality should be handled by the main application
    // through a callback or event system
}

void WiFiComponent::setupWiFiManager() {
    // Set the AP mode callback
    wifi_manager_.setAPCallback(onAPModeStarted);
    
    // Additional WiFiManager configuration can be added here
    // Such as timeout settings, custom parameters, etc.
    LOG_WIFI_D("WiFiManager configured");
}

uint32_t WiFiComponent::calculateMACHash() const {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    
    uint32_t hash = 0;
    for (int i = 0; i < 6; i++) {
        hash = hash * 31 + mac[i];
    }
    
    return hash;
} 