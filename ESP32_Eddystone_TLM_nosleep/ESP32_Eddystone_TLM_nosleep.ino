/* Hackystone tag code for broadcasting Google Eddystone-TLM formatted
 * BLE advertisements at fixed intervals with ESP32 development board.
 *
 * Ruohan Xiong, 2019.
 * Code is based on work from pcbreflux
 * https://github.com/pcbreflux/espressif
*/

#include "sys/time.h"
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "esp_sleep.h"

#define LED_PIN 2
#define TAG_ID "0" // string

RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
BLEAdvertising *pAdvertising;
struct timeval now;
  
void setBeacon() {
    char beacon_data[22];
    uint16_t beconUUID = 0xFEAA;
    uint16_t volt = 3300; // 3300mV = 3.3V
    uint16_t temp = (uint16_t)((float)23.00);
    uint32_t tmil = now.tv_sec*10;
    uint8_t temp_farenheit;
    float temp_celsius;

    temp_farenheit= temprature_sens_read();
    temp_celsius = ( temp_farenheit - 32 ) / 1.8;
    temp = (uint16_t)(temp_celsius);

    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

    oAdvertisementData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
    oAdvertisementData.setCompleteServices(BLEUUID(beconUUID));
    oAdvertisementData.setName(TAG_ID);

    beacon_data[0] = 0x20;  // Eddystone Frame Type (Unencrypted Eddystone-TLM)
    beacon_data[1] = 0x00;  // TLM version
    beacon_data[2] = (volt>>8);  // Battery voltage, 1 mV/bit i.e. 0xCE4 = 3300mV = 3.3V
    beacon_data[3] = (volt&0xFF);  //
    beacon_data[4] = (temp&0xFF);  // Beacon temperature
    beacon_data[5] = (temp>>8);    //
    beacon_data[6] = ((bootcount & 0xFF000000) >> 24);  // Advertising PDU count
    beacon_data[7] = ((bootcount & 0xFF0000) >> 16);  //
    beacon_data[8] = ((bootcount & 0xFF00) >> 8);  //
    beacon_data[9] = (bootcount&0xFF);  //
    beacon_data[10] = ((tmil & 0xFF000000) >> 24);  // Time since power-on or reboot
    beacon_data[11] = ((tmil & 0xFF0000) >> 16);  //
    beacon_data[12] = ((tmil & 0xFF00) >> 8);  //
    beacon_data[13] = (tmil&0xFF);  //

    oAdvertisementData.setServiceData(BLEUUID(beconUUID), std::string(beacon_data, 14));
    pAdvertising->setScanResponseData(oAdvertisementData);
}

void setup() {    
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

}

void loop() {
    gettimeofday(&now, NULL);
    Serial.printf("start ESP32 %d\n",bootcount++);
    Serial.printf("(%lds since last broadcast, %lds since boot)\n",now.tv_sec,now.tv_sec-last);
    last = now.tv_sec;
    // Create the BLE Device
    BLEDevice::init("ESP32");
    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pAdvertising = pServer->getAdvertising();
    setBeacon();
     // Start advertising
    digitalWrite(LED_PIN, HIGH);
    pAdvertising->start();
    Serial.println("Advertising started...");
    delay(50);
    pAdvertising->stop();
    digitalWrite(LED_PIN, LOW);

    Serial.printf("sleeping... \n");
    delay(50);
}
