
// *** MAIN SETTINGS ***
// Replace this block with correct template settings.
// You can find it for every template here:
//
//   https://blynk.cloud/dashboard/templates

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

// Your WiFi credentials.
// Set password to "" for open networks.
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_WIFI_PASS"
#define BLYNK_PRINT Serial  // Comment this out to disable serial monitor prints

// Adding the required libraries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h> //You need to add it by searching "Adafruit unified sensor" in libraries and install it
#include <Adafruit_BMP280.h> //You need to add it by searching "Adafruit BMP280" in libraries and install it
#include <BlynkSimpleEsp32.h> //You need to add it by searching "Blynk" in libraries and install it
#include <DHT.h> //You need to add it by searching "DHT sensor library" in libraries and install it
#include <Adafruit_GFX.h>

#define RZERO 225000

// DHT sensor settings and configuration
#define DHT_BLYNK_VPIN_TEMPERATURE V0 //Virtual pin on Blynk side
#define DHT_BLYNK_VPIN_HUMIDITY V1 //Virtual pin on Blynk side
#define MHZ_BLYNK_VPIN_PPM V3

#define DHTPIN 26
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#define ALTITUDE_0 (1013.25)

#define PIN_RED 4
#define PIN_GREEN 5
#define PIN_BLUE 2

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
int DataPin = 13;
int ppmrange = 5000;
unsigned long pwmtime;

int PPM = 0;
float pulsepercent=0;
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

void reandAndSendSensorsData() {
  readAndSendDhtData();
  Serial.println("Sending data from sensors");
}

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

/*void displayDataOnScreen() {

  pwmtime = pulseIn(DataPin, HIGH, 2000000) / 1000;
  float pulsepercent = pwmtime / 1004.0;
  PPM = ppmrange * pulsepercent;

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  
}
*/

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
