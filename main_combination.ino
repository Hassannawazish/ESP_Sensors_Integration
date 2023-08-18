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

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  int val = digitalRead(sensor_DO);
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
  if (val == 1) {
    Serial.println("Digital Output:   Status: Dry");
  } else {
    Serial.println("Digital Output:   Status: Wet");
  }

  delay(1000);
}