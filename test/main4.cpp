#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Replace these with your WiFi credentials
const char* ssid = /*"TT_23C0";*/ "TP-Link_AP_70CA";
const char* password = /*"i6q4k3b21r";*/ "97800903";

// Replace this with your WeTransfer direct link
const char* firmware_url = "https://download.wetransfer.com/eugv/52ea9cf007565ef95e7e307ba7266ad820241227102745/69bbeed8e8faf4cb83b389e0f432f3bf756f7fb4/firmware.bin?cf=y&token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImRlZmF1bHQifQ.eyJleHAiOjE3MzUyOTY0NjEsImlhdCI6MTczNTI5NTg2MSwiZG93bmxvYWRfaWQiOiI0MDJjYTI1Yy1lZjNhLTQzMzAtYjZiYS0yNmQwMTgwNmNkMjkiLCJzdG9yYWdlX3NlcnZpY2UiOiJzdG9ybSJ9.40anaCTtbmI4_OGxuYI-AXTY0nfefIdndAwr2SkbiQg";
void performOTA() ;
void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    // Start OTA update
    performOTA();
}

void performOTA() {
    HTTPClient http;
    http.begin(firmware_url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        bool canBegin = Update.begin(contentLength);

        if (canBegin) {
            Serial.println("Starting OTA...");
            WiFiClient& client = http.getStream();

            size_t written = Update.writeStream(client);
            if (written == contentLength) {
                Serial.println("OTA written successfully. Rebooting...");
                if (Update.end()) {
                    if (Update.isFinished()) {
                        Serial.println("Update successfully completed.");
                        ESP.restart();
                    } else {
                        Serial.println("Update not finished. Something went wrong.");
                    }
                } else {
                    Serial.printf("Update failed. Error #: %d\n", Update.getError());
                }
            } else {
                Serial.printf("OTA write error. Written %d/%d bytes\n", written, contentLength);
            }
        } else {
            Serial.println("Not enough space for OTA update");
        }
    } else {
        Serial.printf("Failed to download firmware. HTTP Code: %d\n", httpCode);
    }

    http.end();
}

void loop() {
    // Nothing here
}
