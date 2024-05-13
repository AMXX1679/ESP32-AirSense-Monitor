
# Simple Weather Station Readme

## Introduction

This Arduino code serves as the firmware for a simple weather station built on an ESP32 microcontroller. The weather station integrates sensors to measure temperature, humidity, and air quality (PPM). The collected data and transmitted to the Blynk cloud platform for remote monitoring.

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
#include <Adafruit_Sensor.h> //You need to add it by searching "Adafruit unified sensor" in libraries and install it
#include <Adafruit_BMP280.h> //You need to add it by searching "Adafruit BMP280" in libraries and install it
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <DHT.h> //You need to add it by searching "DHT sensor library" in libraries and install it
#include <Adafruit_GFX.h>
```

## Pin Configuration

- **Description:** Define the pin configurations for sensors and actuators.
- **Instructions:**
  - Modify the pin assignments based on your actual hardware setup.

```cpp
#define DHTPIN 26
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

# Sensor Setup

- **Description:** Initialize the DHT and MQ135 sensors.
- **Instructions:**
  - No additional setup is required.

```cpp
DHT dht(DHTPIN, DHTTYPE);
```

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

## LED Condition

```cpp
void setLEDColors(int val) {
  if (val < 1300 && val > 800) {
    // Green LED condition is met
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
  } else if (val > 1300) {
    // Red LED condition is met
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_BLUE, LOW);
  } else {
    // All values are good, turn on green LED
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, HIGH);
  }
}
```

## Setup

- **Description:** Initialize serial communication, Blynk, pins, and the DHT sensor.
- **Instructions:**
  - Ensure the correct pin assignments and set up the DHT sensor.

```cpp
    void setup()
{

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  setupDht();
  // Set up timer to run every 5 sec
  timer.setInterval(5000L, reandAndSendSensorsData);

  pinMode(DataPin, INPUT);

  delay(2000);
}
```

## Loop

- **Description:** Control RGB LEDs, read sensor values, and run Blynk timer.
- **Instructions:**
  - No user configuration needed.

```cpp
void loop() {

  pwmtime = pulseIn(DataPin, HIGH, 2000000) / 1000;
  float pulsepercent = pwmtime / 1004.0;
  PPM = ppmrange * pulsepercent;
  Serial.print("CO2 Konzentration in der Luft in PPM: ");
  Serial.println(PPM);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  setLEDColors(PPM);
  Serial.println(DHT_HUMIDITY);
  Serial.println(DHT_TEMPERATURE);

  Blynk.virtualWrite(MHZ_BLYNK_VPIN_PPM, PPM);

  delay(20);

  Blynk.run();  
  timer.run();
}
````

## Dependencies

- [Blynk Library](https://github.com/blynkkk/blynk-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [DHT Sensor Libry](https://github.com/adafruit/DHT-sensor-library)

## Usage

Follow the provided code documentation to understand and customize the weather station for your specific requirements. Use the Blynk app to remotely monitor and control the weather station.

Feel free to contribute, open issues, or provide feedback to enhance this weather station project!

---


![Logo](https://raw.githubusercontent.com/AMXX1679/ESP32_WeatherStation/main/Untitled%20Sketch_Steckplatine.jpg)
