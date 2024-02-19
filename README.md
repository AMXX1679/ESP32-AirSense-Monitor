# Simple Weather Station Readme

## Introduction

This Arduino code serves as the firmware for a simple weather station built on an ESP32 microcontroller. The weather station integrates sensors to measure temperature, humidity, and air quality (PPM). The collected data is displayed on an OLED screen and transmitted to the Blynk cloud platform for remote monitoring.

## Getting Started

### 1. Blynk Template Settings

- **Description:** Replace the template settings with the information specific to your Blynk project.
- **Instructions:**
  - Visit [Blynk Cloud Templates](https://blynk.cloud/dashboard/templates).
  - Replace the placeholder values with your Blynk template ID, template name, and authentication token.

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Simple Weather Station"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"
```

### 2. WiFi Credentials

- **Description:** Set the WiFi credentials to connect the ESP32 to your local network.
- **Instructions:**
  - Replace "Your_WiFi_SSID" and "Your_WiFi_Password" with your WiFi network's SSID and password.

```cpp
#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASS "Your_WiFi_Password"
```

## Libraries

- **Description:** Ensure the necessary libraries are installed using the Arduino Library Manager.
- **Instructions:**
  - Install the following libraries through the Arduino IDE's Library Manager.

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "MQ135.h"
#include <U8g2lib.h>
```

## Pin Configuration

- **Description:** Define the pin configurations for sensors and actuators.
- **Instructions:**
  - Modify the pin assignments based on your actual hardware setup.

```cpp
#define DHTPIN 26
#define MQ135_SENSOR_PIN 32
#define PIN_RED 4
#define PIN_GREEN 5
#define PIN_BLUE 2
```

## Blynk Setup

- **Description:** Initialize the Blynk connection with the provided authentication token and WiFi credentials.
- **Instructions:**
  - Insert your Blynk authentication token, WiFi SSID, and password.

```cpp
Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
```

## Display Setup

- **Description:** Configure the OLED display using the U8g2 library.
- **Instructions:**
  - No additional setup is needed, but ensure the library is installed.

```cpp
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
```

## Sensor Setup

- **Description:** Initialize the DHT and MQ135 sensors.
- **Instructions:**
  - No additional setup is required.

```cpp
DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor(MQ135_SENSOR_PIN);
```

## Timer and State Variables

- **Description:** Declare timer and state variables for data processing and LED control.
- **Instructions:**
  - No user configuration needed.

```cpp
BlynkTimer timer;
int val;
int sensorValue = 0;
int DHT_ENABLED = 0;
float DHT_HUMIDITY;
float DHT_HUMIDITY_IGNORED_DELTA = 0.01;
float DHT_TEMPERATURE;
float DHT_TEMPERATURE_IGNORED_DELTA = 0.01;
int RUN = 0;
```

## Setup DHT

- **Description:** Set up the DHT sensor.
- **Instructions:**
  - No user configuration needed.

```cpp
void setupDht() {
  Serial.println("DHT startup!");
  dht.begin();
  DHT_ENABLED = 1;
}
```

## Send DHT Data

- **Description:** Send DHT sensor data to the Blynk server.
- **Instructions:**
  - No user configuration needed.

```cpp
void sendDhtData() {
  Serial.println("Sending DHT data");
  Blynk.virtualWrite(DHT_BLYNK_VPIN_TEMPERATURE, DHT_TEMPERATURE);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_HUMIDITY, DHT_HUMIDITY);
}
```

## Read and Send DHT Data

- **Description:** Read DHT sensor data and send it to Blynk if changes are detected.
- **Instructions:**
  - No user configuration needed.

```cpp
void readAndSendDhtData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // ...
  // (Continued on next comment due to character limit)
```

## Read and Send Sensors Data

- **Description:** Read and send DHT sensor data every 5 seconds.
- **Instructions:**
  - No user configuration needed.

```cpp
void reandAndSendSensorsData() {
  readAndSendDhtData();
  Serial.println("Sending data from sensors");
}
```

## Red LED Condition

- **Description:** Determine if the conditions for activating the red LED are met.
- **Instructions:**
  - No user configuration needed.

```cpp
bool isRedLEDCondition(float humidity, float temperature, int val) {
  // ...
```

## Green LED Condition

- **Description:** Determine if the conditions for activating the green LED are met.
- **Instructions:**
  - No user configuration needed.

```cpp
bool isGreenLEDCondition(float humidity, float temperature, int val) {
  // ...
```

## Set LED Colors

- **Description:** Control RGB LEDs based on sensor data.
- **Instructions:**
  - No user configuration needed.

```cpp
void setLEDColors(float humidity, float temperature, int val) {
  // ...
```

## Display Data on Screen

- **Description:** Display sensor data on the OLED screen.
- **Instructions:**
  - No user configuration needed.

```cpp
void displayDataOnScreen() {
  // ...
```

## Setup

- **Description:** Initialize serial communication, Blynk, pins, and the DHT sensor.
- **Instructions:**
  - Ensure the correct pin assignments and set up the DHT sensor.

```cpp
void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  pinMode(MQ135_SENSOR_PIN, INPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  setupDht();

  timer.setInterval(5000L, reandAndSendSensorsData);

  u8g2.begin();
}
```

## Loop

- **Description:** Control RGB LEDs, read sensor values, display data, and run Blynk timer.
- **Instructions:**
  - No user configuration needed.

```cpp
void loop() {
  setLEDColors(DHT_HUMIDITY, DHT_TEMPERATURE, val);

  val =

 analogRead(MQ135_SENSOR_PIN);
  Serial.print("raw = ");
  Serial.println(val);

  Blynk.virtualWrite(MQ135_BLYNK_VPIN_PPM, val);

  displayDataOnScreen();

  delay(2000);

  Blynk.run();
  timer.run();
}
````
## Features

- **Multi-Sensor Integration:** Incorporates DHT11 and MQ135 sensors for accurate temperature, humidity, and air quality readings.
- **Blynk Integration:** Utilizes the Blynk cloud platform for remote monitoring and control of the weather station.
- **OLED Display:** Displays real-time sensor data on an OLED screen for local visualization.
- **LED Indicators:** Provides visual feedback through RGB LEDs based on environmental conditions.
- **Customizable:** Easily adaptable and extendable for different sensor configurations and use cases.
## Dependencies

- [Blynk Library](https://github.com/blynkkk/blynk-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280)
- [DHT Sensor Libry](https://github.com/adafruit/DHT-sensor-library)
- [MQ135 Library](https://github.com/GeorgK/MQ135)

## Usage

Follow the provided code documentation to understand and customize the weather station for your specific requirements. Use the Blynk app to remotely monitor and control the weather station.

Feel free to contribute, open issues, or provide feedback to enhance this weather station project!

---
