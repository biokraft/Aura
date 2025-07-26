#include "wifi.h"
#include <Arduino.h>

WiFiComponent::WiFiComponent() : initialized_(false) {
    Serial.println("WiFi component created");
}

WiFiComponent::~WiFiComponent() {
    Serial.println("WiFi component destroyed");
}

bool WiFiComponent::init() {
    if (initialized_) {
        Serial.println("WiFi component already initialized");
        return true;
    }
    
    Serial.println("Initializing WiFi component...");
    
    // Generate unique hostname
    device_hostname_ = generateUniqueHostname();
    WiFi.setHostname(device_hostname_.c_str());
    Serial.printf("Device hostname set to: %s\n", device_hostname_.c_str());
    
    // Setup WiFiManager
    setupWiFiManager();
    
    initialized_ = true;
    Serial.println("WiFi component initialized successfully");
    return true;
}

bool WiFiComponent::connect() {
    if (!initialized_) {
        Serial.println("ERROR: WiFi component not initialized");
        return false;
    }
    
    Serial.println("Attempting WiFi connection...");
    
    // Try to connect, will create AP if no saved credentials
    if (!wifi_manager_.autoConnect(DEFAULT_CAPTIVE_SSID)) {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
    
    Serial.println("WiFi connected successfully");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
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
    Serial.println("Resetting WiFi settings...");
    wifi_manager_.resetSettings();
    Serial.println("WiFi settings reset completed");
}

bool WiFiComponent::autoConnect(const char* ap_name) {
    if (!initialized_) {
        Serial.println("ERROR: WiFi component not initialized");
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
    Serial.printf("Hostname updated to: %s\n", hostname.c_str());
}

void WiFiComponent::onAPModeStarted(WiFiManager* wifiManager) {
    Serial.println("WiFi AP Mode started");
    Serial.printf("AP Name: %s\n", DEFAULT_CAPTIVE_SSID);
    Serial.println("Connect to configure WiFi settings");
    
    // Note: In the original code, this triggered a UI update
    // That functionality should be handled by the main application
    // through a callback or event system
}

void WiFiComponent::setupWiFiManager() {
    // Set the AP mode callback
    wifi_manager_.setAPCallback(onAPModeStarted);
    
    // Additional WiFiManager configuration can be added here
    // Such as timeout settings, custom parameters, etc.
    Serial.println("WiFiManager configured");
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