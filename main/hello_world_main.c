#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "dht.h"

#define DHT_PIN GPIO_NUM_16
#define DHT_TYPE DHT_TYPE_DHT11

#define SENSOR_DO_PIN GPIO_NUM_5
#define DO_THRESHOLD 400
#define MQ2_PIN ADC1_CHANNEL_0

#define DOOR_PIN GPIO_NUM_34

static const char *TAG = "main";

static void taskTemperatureHumidity(void *pvParameters) {
    float temperature, humidity;

    for (;;) {
        if (dht_read_data(DHT_TYPE, DHT_PIN, &humidity, &temperature) == ESP_OK) {
            ESP_LOGI(TAG, "Temperature: %.2f°C", temperature);
            ESP_LOGI(TAG, "Humidity: %.2f%%", humidity);
        } else {
            ESP_LOGE(TAG, "Failed to read data from DHT sensor");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void taskDigitalOutput(void *pvParameters) {
    for (;;) {
        int val = gpio_get_level(SENSOR_DO_PIN);
        if (val == 1) {
            ESP_LOGI(TAG, "Digital Output: Status: Dry");
        } else {
            ESP_LOGI(TAG, "Digital Output: Status: Wet");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void taskSmokeDetection(void *pvParameters) {
    uint32_t sensorValue;

    for (;;) {
        esp_adc1_config_width(ADC_WIDTH_BIT_12);
        esp_adc1_config_channel_atten(MQ2_PIN, ADC_ATTEN_DB_11);
        sensorValue = adc1_get_raw(MQ2_PIN);

        if (sensorValue > DO_THRESHOLD) {
            ESP_LOGI(TAG, "Sensor Value: %d | Smoke detected!", sensorValue);
        } else {
            ESP_LOGI(TAG, "Sensor Value: %d | Smoke Not detected!", sensorValue);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void taskDoorStatus(void *pvParameters) {
    for (;;) {
        int pinState = gpio_get_level(DOOR_PIN);
        if (pinState == 1) {
            ESP_LOGI(TAG, "Pin is HIGH (Door Opened)");
        } else {
            ESP_LOGI(TAG, "Pin is LOW (Door Closed)");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main() {
    gpio_config_t io_conf;

    // Configure DHT pin as an input
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << DHT_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Configure digital output sensor pin as an input
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << SENSOR_DO_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Configure door status pin as an input
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << DOOR_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Initialize DHT sensor
    if (dht_init(DHT_PIN, DHT_TYPE) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize DHT sensor");
    }

    // Create tasks
    xTaskCreate(taskTemperatureHumidity, "TempHumTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskDigitalOutput, "DigitalOutTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskSmokeDetection, "SmokeDetectTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskDoorStatus, "DoorStatusTask", 2048, NULL, 1, NULL);
}
