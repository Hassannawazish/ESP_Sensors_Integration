#include <Arduino_FreeRTOS.h>
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
float sensorValue; 

const int digitalPin_door = 34;

void taskTemperatureHumidity(void *pvParameters) {
  for (;;) {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
    vTaskDelay(pdMS_TO_TICKS(delayMS));
  }
}

void taskDigitalOutput(void *pvParameters) {
  for (;;) {
    int val = digitalRead(sensor_DO);
    if (val == 1) {
      Serial.println("Digital Output:   Status: Dry");
    } else {
      Serial.println("Digital Output:   Status: Wet");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void taskSmokeDetection(void *pvParameters) {
  for (;;) {
    sensorValue = analogRead(MQ2pin); 
    if(sensorValue > Threshold) {
      Serial.print("Sensor Value: ");
      Serial.print(sensorValue);
      Serial.println("       | Smoke detected!");
    } else {
      Serial.print("Sensor Value: ");
      Serial.print(sensorValue);
      Serial.println("       | Smoke Not detected!");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void taskDoorStatus(void *pvParameters) {
  for (;;) {
    int pinState = digitalRead(digitalPin_door);
    if (pinState == HIGH) {
      Serial.println("Pin is HIGH (Door Opened)");
    } else {
      Serial.println("Pin is LOW  (Door Closed)");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup() {
  pinMode(digitalPin_door, INPUT);
  pinMode(sensor_DO, INPUT);
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  xTaskCreate(taskTemperatureHumidity, "TempHumTask", 1000, NULL, 1, NULL);
  xTaskCreate(taskDigitalOutput, "DigitalOutTask", 1000, NULL, 2, NULL);
  xTaskCreate(taskSmokeDetection, "SmokeDetectTask", 1000, NULL, 3, NULL);
  xTaskCreate(taskDoorStatus, "DoorStatusTask", 1000, NULL, 4, NULL);
  
  vTaskStartScheduler();
}

void loop() {
}
