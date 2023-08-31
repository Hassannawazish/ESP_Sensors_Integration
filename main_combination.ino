#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>

#define RX_PIN 16  // GPIO16 (D0)
#define TX_PIN 17  // GPIO17 (D1)
SoftwareSerial dhtSerial(RX_PIN, TX_PIN);  // RX, TX
#define DHTTYPE    DHT11
DHT_Unified dht(RX_PIN, DHTTYPE);
uint32_t delayMS;

#define sensor_DO 5

#define Threshold 400
#define MQ2pin A0
float gasSensorValue; 

const int digitalPin_door_sensor = 34;

int measurePin = A3;
int ledPower = 17;
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  pinMode(digitalPin_door_sensor, INPUT);
  pinMode(sensor_DO, INPUT);
  pinMode(ledPower, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  delay(2000);
}

void loop() {
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);
  voMeasured = map(voMeasured, 0, 4095, 0, 1023);
  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;
  if ( dustDensity < 0) {
    dustDensity = 0.00;
  }
  
  int water_sensor_val = digitalRead(sensor_DO);
  gasSensorValue = analogRead(MQ2pin); 
  int pinState = digitalRead(digitalPin_door_sensor);
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

  Serial.print("Raw Signal Value (0-1023):  ");
  Serial.print(voMeasured);
  Serial.print("     Voltage:   ");
  Serial.print(calcVoltage);
  Serial.print("     Dust Density:   ");
  Serial.println(dustDensity);
  
  if (water_sensor_val == 1) {
    Serial.println("Digital Output:   Status: Dry");
  } else {
    Serial.println("Digital Output:   Status: Wet");
  }
  if(gasSensorValue > Threshold)
  {
    Serial.print("Gas Sensor Value: ");
    Serial.print(gasSensorValue);
    Serial.println("       | Smoke detected!");
  }
  else
  {
    Serial.print("Gas Sensor Value: ");
    Serial.print(gasSensorValue);
    Serial.println("       | Smoke Not detected!");
  }
  Serial.println("");
   if (pinState == HIGH) {
    Serial.println("Pin is HIGH (Door Opened)");
  } else {
    Serial.println("Pin is LOW  (Door Closed");
  }
  delay(2000);
}