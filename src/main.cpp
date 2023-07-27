/**
 * @file        main.cpp
 * @authors   	Marc-Antoine Doyon
 * @copyright 	2023, Marc-Antoine Doyon @madoyon - GitHub
 * You may use, distribute and modify this code under the terms of the MIT license 
 * @date		2023-07-26
 */

/* Includes -------------------------------------------------------*/
#include <Arduino.h>
#include "esp32-hal-ledc.h"
#include "main.h"
#include "light.h"
#include "soil.h"
#include "temperature.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "esp_rmaker_core.h"
#include "esp_rmaker_standard_devices.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_schedule.h"
#include "esp_rmaker_scenes.h"
#include "esp_rmaker_utils.h"
#include <nvs_flash.h>
#include <esp_log.h>
#include "app_wifi.h"
#include "app_insights.h"

#define DEBUG_ENABLE 0
#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10
#define CONFIG_ESP_RMAKER_MQTT_ENABLE_BUDGETING 0

//Sensors on Mosty-S
uint32_t soil_moisture;
soil_t soil_sensor;
LTR303_t light_sensor;
temperature_t temp_sensor;

//Average values of each sensor
static float avg_light = 0;
static float avg_soil = 0;
static float avg_temp = 0;

//ESP32 rainmaker devices 
static const char *TAG = "app_main";
esp_rmaker_device_t* temp_sensor_rmaker;
esp_rmaker_device_t* light_sensor_rmaker;
esp_rmaker_device_t* soil_sensor_rmaker;

//Scheduler variables
bool task_100_ms_flag = false;
bool task_1000_ms_flag = false;
bool task_2000_ms_flag = false;
bool task_5000_ms_flag = false;

void setup() {

  //Initialize sensors (light, moisture, temperature)
  light_sensor = init_task_light_sensor(PIN_LTR303_SDA, PIN_LTR303_SCL);
  soil_sensor = init_task_soil_sensor(PIN_SOIL_PWM, PIN_SOIL_ADC);
  temp_sensor = init_task_temperature_sensor(PIN_TEMP_AMBIANT);

#if DEBUG_ENABLE == 1
  Serial.begin(115200);
#endif

    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    //Initialize wifi settings
    app_wifi_init();

     /* Initialize the ESP RainMaker Agent.
     * Note that this should be called after app_wifi_init() but before app_wifi_start()
     * */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };

    err = nvs_flash_init_partition("nvs");
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS Flash init failed");
        ESP_ERROR_CHECK( err );

    }

    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "ESP RainMaker Device", "Temperature Sensor");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    /* Create a device and add the relevant parameters to it */
    temp_sensor_rmaker = esp_rmaker_temp_sensor_device_create("Temperature Sensor", NULL, avg_temp);
    light_sensor_rmaker = esp_rmaker_temp_sensor_device_create("Light Sensor", NULL, avg_light);
    soil_sensor_rmaker = esp_rmaker_temp_sensor_device_create("Soil Sensor", NULL, avg_soil);

    esp_rmaker_node_add_device(node, temp_sensor_rmaker);
    esp_rmaker_node_add_device(node, light_sensor_rmaker);
    esp_rmaker_node_add_device(node, soil_sensor_rmaker);

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable scheduling. */
    esp_rmaker_schedule_enable();

    /* Enable Scenes */
    esp_rmaker_scenes_enable();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();

    /* Start the Wi-Fi.
     * If the node is provisioned, it will start connection attempts,
     * else, it will start Wi-Fi provisioning. The function will return
     * after a connection has been successfully established
     */

    err = app_wifi_start(POP_TYPE_NONE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wifi. Aborting!!!");
        delay(1000);
        abort();
    }
}

void loop() {

  static uint32_t counter_100ms = millis();
  static uint32_t counter_5000ms = millis();

  if(task_100_ms_flag)
  {
    task_light_sensor(&light_sensor, &avg_light);
    task_soil_sensor(&soil_sensor, &avg_soil);
    task_temperature_sensor(&temp_sensor, &avg_temp);
    task_100_ms_flag = false;
  }
  
  if(task_5000_ms_flag)
  {
      esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(temp_sensor_rmaker, "esp.param.temperature"),
        esp_rmaker_float(avg_temp));
      
      
      esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(light_sensor_rmaker, "esp.param.temperature"),
        esp_rmaker_float(avg_light));


      esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(soil_sensor_rmaker, "esp.param.temperature"),
        esp_rmaker_float(avg_soil));
      task_5000_ms_flag = false;
#if DEBUG_ENABLE == 1
      Serial.println("5000ms");
      Serial.print("Avg light value: ");
      Serial.println(avg_light);
      Serial.print("Avg soil value: ");
      Serial.println(avg_soil);
      Serial.print("Avg temp value: ");
      Serial.println(avg_temp);
#endif 
  }

  if((millis()-counter_100ms) >= 100) 
  {
    task_100_ms_flag = true;
    counter_100ms = millis();
  }

  if((millis()-counter_5000ms) >= 5000)
  {
    task_5000_ms_flag = true;
    counter_5000ms = millis();
  }
}