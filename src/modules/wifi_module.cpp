#include "wifi_module.h"
#include <WiFi.h>

static const char* _ssid = nullptr;
static const char* _password = nullptr;
static TaskHandle_t _wifiTaskHandle = nullptr;

static const unsigned long RECONNECT_INTERVAL = 5000;

// FreeRTOS task running on Core 0
static void wifiTask(void* param) {
    unsigned long lastReconnectAttempt = 0;

    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            unsigned long now = millis();
            if (now - lastReconnectAttempt >= RECONNECT_INTERVAL) {
                lastReconnectAttempt = now;
                Serial.println("[WiFi] Reconnecting...");
                WiFi.disconnect();
                WiFi.begin(_ssid, _password);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

bool wifiInit(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;

    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection with timeout
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("[WiFi] Connected! IP: ");
        Serial.println(WiFi.localIP());

        // Start WiFi maintenance task on Core 0
        xTaskCreatePinnedToCore(
            wifiTask,
            "WiFiTask",
            4096,
            NULL,
            1,
            &_wifiTaskHandle,
            0  // Core 0
        );
        Serial.println("[WiFi] Task started on Core 0");

        return true;
    } else {
        Serial.println("[WiFi] Connection failed!");
        return false;
    }
}

void wifiUpdate() {
    // No longer needed - handled by FreeRTOS task on Core 0
}

bool wifiIsConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String wifiGetIP() {
    return WiFi.localIP().toString();
}

String wifiGetMAC() {
    return WiFi.macAddress();
}
