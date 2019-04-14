# Hackystone 

Capstone hardware prototyping for indoor localization with Bluetooth Low Energy. 

ESP32_BLE_beaconscan and ESP_32_Eddystone_TLM_nosleep are based on code from 
[pcbreflux](https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook)

# What this does so far
 
`ESP32_Eddystone_TLM_nosleep` is a simple sketch that uploads a BLE ping every 
second. The ping's data is formatted per Google's Eddystone protocol which 
allows us to transmit some identifying information with the ping. The blue LED 
on board blinks when transmitting also. 

`ESP32_BLE_beaconscan` monitors for Bluetooth pings. Eddystone protocol pings 
are printed to serial (should be sent via WiFi). Serial monitor in Arduino: 
Ctrl+Shift+M, set baud rate to 115200. 

Upload `ESP32_Eddystone_TLM_nosleep` to tag board(s) and `ESP32_BLE_beaconscan` 
to anchor board(s). For now, anchor boards only print to serial so need to be 
tethered until WiFi is implemented. 

Connect 2xAA battery pack to 3V3 and GND, *not VIN*. VIN goes through on-board 
regulator to provide stable 3.3V to chip but this requires at least around 4.5V 
input. 3V3 goes directly to chip - since actual input of chip is spec'ed at 
2.3 - 3.6V this works well with 2xAA. If we experience any problems with this 
setup, we can use VIN with 4xAA power packs.


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

* Test WiFi support/HTTP upload of data
* make beacon code work on nRF51822 (stretch goal, PoC w/ ESP32) 

