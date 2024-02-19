// *** MAIN SETTINGS ***
// Replace this block with correct template settings.
// You can find it for every template here:
//
//   https://blynk.cloud/dashboard/templates

#define BLYNK_TEMPLATE_ID "TMPL4ah-fKabl"
#define BLYNK_TEMPLATE_NAME "Simple Weather Station"
#define BLYNK_AUTH_TOKEN "ytRnzjY2VVAeSwuRFK6-qhRg-wX2TPRu"

// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID "Raum 306"
#define WIFI_PASS "schokimagessen306"
#define BLYNK_PRINT Serial  // Comment this out to disable serial monitor prints

// Adding the required libraries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h> //You need to add it by searching "Adafruit unified sensor" in libraries and install it
#include <Adafruit_BMP280.h> //You need to add it by searching "Adafruit BMP280" in libraries and install it
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <DHT.h> //You need to add it by searching "DHT sensor library" in libraries and install it
#include "MQ135.h"
#include <U8g2lib.h>


#define RZERO 225000

// DHT sensor settings and configuration
#define DHT_BLYNK_VPIN_TEMPERATURE V0 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_HUMIDITY V1 //Virtual pin on Blynk side
#define MQ135_BLYNK_VPIN_PPM V3

#define DHTPIN 26
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define ALTITUDE_0 (1013.25)

#include "MQ135.h"
#define RZERO 225000


#define PIN_RED 4
#define PIN_GREEN 5
#define PIN_BLUE 2

MQ135 gasSensor = MQ135(32);
int val;
int sensorPin = 32;
int sensorValue = 0;

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer; 

// DHT related variables
int   DHT_ENABLED = 0;
float DHT_HUMIDITY;
float DHT_HUMIDITY_IGNORED_DELTA = 0.01;
float DHT_TEMPERATURE;
float DHT_TEMPERATURE_IGNORED_DELTA = 0.01;

int RUN = 0;

// SETUP BLOCK

// DHT setup
void setupDht() {
  Serial.println("DHT startup!");
  dht.begin();
  DHT_ENABLED = 1;
}
// BMP setup

// Sending data from DHT sensor to Blynk
void sendDhtData() {
  Serial.println("Sending DHT data");
  Blynk.virtualWrite(DHT_BLYNK_VPIN_TEMPERATURE, DHT_TEMPERATURE);
  Blynk.virtualWrite(DHT_BLYNK_VPIN_HUMIDITY, DHT_HUMIDITY);
}

// DATA PROCESSING BLOCK
// Reading DHT data
void readAndSendDhtData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT");
  } else {
    float humidityDelta = abs(humidity - DHT_HUMIDITY) - DHT_HUMIDITY_IGNORED_DELTA;
    float temperatureDelta = abs(temperature - DHT_TEMPERATURE) - DHT_HUMIDITY_IGNORED_DELTA;
    if (humidityDelta > 0 || temperatureDelta > 0) {
      DHT_HUMIDITY = humidity;
      DHT_TEMPERATURE = temperature;
      Serial.printf("Humidity: %f%%. Temperature: %f*C.\n", humidity, temperature);
      sendDhtData();
    }
  }
}

// Reading BMP data

void reandAndSendSensorsData() {
  readAndSendDhtData();
  Serial.println("Sending data from sensors");
}

bool isRedLEDCondition(float humidity, float temperature, int val) {
  bool isHumidityBad = (humidity < 30 || humidity > 70);
  bool isTemperatureBad = (temperature < 18 || temperature > 27);
  bool isValBadCondition = (val > 1400);
  return isHumidityBad || isTemperatureBad || isValBadCondition;
}

bool isGreenLEDCondition(float humidity, float temperature, int val) {
  bool isHumidityGood = (humidity >= 30 && humidity <= 40) || (humidity >= 50 && humidity <= 70);
  bool isTemperatureGood = (temperature >= 18 && temperature <= 21) || (temperature >= 24 && temperature <= 26);
  bool isValGoodCondition = (val >= 800 && val <= 1400);
  return isHumidityGood || isTemperatureGood || isValGoodCondition;
}

void setLEDColors(float humidity, float temperature, int val) {
  if (isGreenLEDCondition(humidity, temperature, val)) {
    // Green LED condition is met
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
  } else if (isRedLEDCondition(humidity, temperature, val)) {
    // Red LED condition is met
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_BLUE, LOW);
  } else {
    // All values are good, turn on green LED
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
  }
}

void displayDataOnScreen() {
  u8g2.clearBuffer();          // Clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
  u8g2.setCursor(2, 10);
  u8g2.print("PPM: ");
  u8g2.print(val);

  u8g2.setCursor(2, 30);
  u8g2.print("Temperature: ");
  u8g2.print(DHT_TEMPERATURE);
  u8g2.print(" *C");

  u8g2.setCursor(2, 50);
  u8g2.print("Humidity: ");
  u8g2.print(DHT_HUMIDITY);
  u8g2.print(" %");

  u8g2.sendBuffer();          // Transfer internal memory to the display
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  pinMode(sensorPin, INPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  setupDht();
  // Set up timer to run every 5 sec
  timer.setInterval(5000L, reandAndSendSensorsData);

  u8g2.begin();

}

void loop() {

  setLEDColors(DHT_HUMIDITY, DHT_TEMPERATURE, val);

  val = analogRead(32);
  Serial.print("raw = ");
  Serial.println(val);

  Blynk.virtualWrite(MQ135_BLYNK_VPIN_PPM, val);

  displayDataOnScreen();

  delay(2000);

  Blynk.run();  
  timer.run();
}
