/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEBeacon.h"
#include "BLEEddystoneTLM.h"
#include "BLEEddystoneURL.h"

#define ANCHOR_ID 2
//#define CONNECT_WIFI // remove this directive if debugging with Serial
#define WIFI_SSID "your_ssid"
#define WIFI_PSK "wifi_psk"
#define ENDPOINT "http://railfan.net/api/v1.0"
#define LED_PIN 2

BLEScan* pBLEScan;
int scanTime = 1; //In seconds
uint16_t beconUUID = 0xFEAA;
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))

int tag_ids[100];
int rssi_vals[100];
int rssi_ptr;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.getServiceDataUUID().equals(BLEUUID(beconUUID))==true) {  // found Eddystone UUID
            //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
            std::string strServiceData = advertisedDevice.getServiceData();
            uint8_t cServiceData[100];
            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
            if (cServiceData[0]==0x20) { // unencrypted Eddystone-TLM
                BLEEddystoneTLM oBeacon = BLEEddystoneTLM();
                oBeacon.setData(strServiceData);
                Serial.printf("tagId: %d, RSSI: %d\n", atoi(advertisedDevice.getName().c_str()), advertisedDevice.getRSSI());
                tag_ids[rssi_ptr] = atoi(advertisedDevice.getName().c_str());
                rssi_vals[rssi_ptr] = (signed int) advertisedDevice.getRSSI();
                rssi_ptr++;
            }
        }
    }
};


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster

#ifdef CONNECT_WIFI
    WiFi.begin(WIFI_SSID, WIFI_PSK);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("waiting for WiFi connection");
    }
    digitalWrite(LED_PIN, HIGH);
#endif
}

void loop() {
    rssi_ptr = 0;
    std::fill_n(rssi_vals, 100, 0);
    std::fill_n(tag_ids, 100, 0);

    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json["anchorId"] = ANCHOR_ID;
    json["uptime"] = millis();
    JsonArray &scanData = json.createNestedArray("data");

    BLEScanResults foundDevices = pBLEScan->start(scanTime);
    for (int i=0; i<rssi_ptr; i++){
        JsonObject &thisScan = scanData.createNestedObject();
        thisScan["tagId"] = tag_ids[i];
        thisScan["rssi"] = rssi_vals[i];
    }
    json.prettyPrintTo(Serial);

#ifdef CONNECT_WIFI
    char json_buffer[1024];
    json.prettyPrintTo(json_buffer, sizeof(json_buffer));

    HTTPClient http;
    http.begin(ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json_buffer);
    http.end();
    Serial.printf("HTTP Response: %d\n", httpCode);
    digitalWrite(LED_PIN, httpCode == HTTP_SUCCESS);
#endif
    jsonBuffer.clear();
}
