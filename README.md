# Hackystone 

![hackystone_cover_image](frankfurt_rail_station.jpg)

Microcontroller code for indoor localization with BLE. Part of Capstone design 
project, winter 2019.

ESP32_BLE_beaconscan and ESP_32_Eddystone_TLM_nosleep are based on code from 
[pcbreflux](https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook)

# Structure

Anchors: stationary BLE-WiFi gateways which scan for advertisements from tags
and uploads pings received and RSSI values to server endpoint via HTTP POST.

Tags: mobile beacons which broadcast advertisements at fixed intervals using 
the Google Eddystone-TLM protocol. 

# What this does so far
 
`ESP32_Eddystone_TLM_nosleep` sends a BLE advertisement every 100 ms. 
The ping's data is formatted per Google's Eddystone protocol which 
allows us to transmit some identifying information with the ping. The blue LED 
on board blinks when transmitting. 

`ESP32_BLE_beaconscan` monitors for Bluetooth pings. Eddystone protocol pings 
are printed to either serial (remove `CONNECT_WIFI`) or sent as JSON data to 
server over HTTP. 

Upload `ESP32_Eddystone_TLM_nosleep` to tag board(s) and `ESP32_BLE_beaconscan` 
to anchor board(s). Remember to change all relevant device-specific 
preprocessor directives. 

Connect 2xAA battery pack to 3V3 and GND, *not VIN*. VIN goes through on-board 
regulator to provide stable 3.3V to chip but this requires at least around 4.5V 
input. 3V3 goes directly to chip - since actual input of chip is spec'ed at 
2.3 - 3.6V this works well with 2xAA. 


# Dev Environment Setup

1. [Install Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Install the ESP32 core for Arduino. See [instructions here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md). This essentially allows third party platforms such as ESP-32 to be supported in Arduino IDE. Our boards are "ESP32 Dev Module"


# Uploading to Board

## Tags 
Set preprocessor directive `TAG_ID` to an unique integer *in string format* ex. `"0"`

Upload to board and hard reset (click `EN`) the device.

Blue LED should flash on every transmit. 

## Anchor 
Set preprocessor directive `ANCHOR_ID` to an unique integer , ex `1`.
Configure WiFI SSID, password, and HTTP URL for API endpoint using the directives. 

Set partitioning scheme to `HUGE App (3MB no OTA)` (from Tools dropdown). 

Upload to board and hard reset (click `EN` button) the device.

Blue LED is on when WiFi uplink is established successfully and HTTP `200` 
responses are being received from server. 

# Output

JSON object with the following:
* `anchorId`: int
* `uptime`: int, milliseconds since anchor boot. ex. 2712
    * note: this time is relative and inaccurate - significant drift
* `data`: array of objects: 
    * `tagId`: int
    * `rssi`: int between -99 and 0 

See `sample_output.json`. 

# Todos

* make beacon code work on nRF51822 (stretch goal, PoC w/ ESP32) 

