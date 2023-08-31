#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DHTPIN 16      // GPIO16 (D0)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define sensor_DO 5

#define Threshold 400
#define MQ2pin A0
float sensorValue;

const int digitalPin_door = 34;

int measurePin = A3;
int ledPower = 12;
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void taskTemperatureHumidity(void *pvParameters) {
  for (;;) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void taskDigitalOutput(void *pvParameters) {
  for (;;) {
    int val = digitalRead(sensor_DO);
    if (val == HIGH) {
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
    if (sensorValue > Threshold) {
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

void taskAdditionalSensor(void *pvParameters) {
  for (;;) {
    digitalWrite(ledPower, LOW);
    delayMicroseconds(samplingTime);
    voMeasured = analogRead(measurePin);
    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH);
    delayMicroseconds(sleepTime);
    voMeasured = map(voMeasured, 0, 4095, 0, 1023);
    calcVoltage = voMeasured * (5.0 / 1024);
    dustDensity = 0.17 * calcVoltage - 0.1;

    if (dustDensity < 0) {
      dustDensity = 0.00;
    }

    Serial.print("Raw Signal Value (0-1023):  ");
    Serial.print(voMeasured);
    Serial.print("     Voltage:   ");
    Serial.print(calcVoltage);
    Serial.print("     Dust Density:   ");
    Serial.println(dustDensity);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup() {
  pinMode(digitalPin_door, INPUT);
  pinMode(sensor_DO, INPUT);
  Serial.begin(9600);

  dht.begin();

  xTaskCreate(taskTemperatureHumidity, "TempHumTask", 1000, NULL, 2, NULL);
  xTaskCreate(taskDigitalOutput, "DigitalOutTask", 1000, NULL, 1, NULL);
  xTaskCreate(taskSmokeDetection, "SmokeDetectTask", 1000, NULL, 1, NULL);
  xTaskCreate(taskDoorStatus, "DoorStatusTask", 1000, NULL, 1, NULL);
  xTaskCreate(taskAdditionalSensor, "AdditionalSensorTask", 1000, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop() {
}
