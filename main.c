#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "dht.h"

#define DHT_PIN 16
#define DHT_TYPE DHT_TYPE_DHT11
#define SOIL_SENSOR_PIN 5
#define MQ2_PIN ADC1_CHANNEL_0
#define DOOR_PIN 34
#define THRESHOLD 400

static const char *TAG = "MultiSensor";

void temp_hum_task(void *pvParameters) {
    while (1) {
        float temperature = 0.0, humidity = 0.0;
        if (dht_read_float_data(DHT_TYPE, DHT_PIN, &humidity, &temperature) == ESP_OK) {
            ESP_LOGI(TAG, "Temperature: %.1f°C", temperature);
            ESP_LOGI(TAG, "Humidity: %.1f%%", humidity);
        } else {
            ESP_LOGW(TAG, "Failed to read from DHT sensor");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void soil_sensor_task(void *pvParameters) {
    gpio_set_direction(SOIL_SENSOR_PIN, GPIO_MODE_INPUT);
    while (1) {
        int val = gpio_get_level(SOIL_SENSOR_PIN);
        if (val == 1) {
            ESP_LOGI(TAG, "Soil Status: Dry");
        } else {
            ESP_LOGI(TAG, "Soil Status: Wet");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void smoke_detect_task(void *pvParameters) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(MQ2_PIN, ADC_ATTEN_DB_11);
    while (1) {
        int value = adc1_get_raw(MQ2_PIN);
        if (value > THRESHOLD) {
            ESP_LOGW(TAG, "Smoke detected! Value: %d", value);
        } else {
            ESP_LOGI(TAG, "No smoke detected. Value: %d", value);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void door_status_task(void *pvParameters) {
    gpio_set_direction(DOOR_PIN, GPIO_MODE_INPUT);
    while (1) {
        int state = gpio_get_level(DOOR_PIN);
        if (state == 1) {
            ESP_LOGI(TAG, "Door Opened");
        } else {
            ESP_LOGI(TAG, "Door Closed");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main() {
    xTaskCreate(temp_hum_task, "temp_hum_task", 2048, NULL, 1, NULL);
    xTaskCreate(soil_sensor_task, "soil_sensor_task", 2048, NULL, 1, NULL);
    xTaskCreate(smoke_detect_task, "smoke_detect_task", 2048, NULL, 1, NULL);
    xTaskCreate(door_status_task, "door_status_task", 2048, NULL, 1, NULL);
}
