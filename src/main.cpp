#include <Arduino.h>
#include "esp32-hal-ledc.h"
#include "main.h"
#include "light.h"
#include "soil.h"
#include "temperature.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"




uint32_t soil_moisture;
soil_t soil_sensor;
LTR303_t light_sensor;
temperature_t temp_sensor;
static float avg_light;
static float avg_soil;
static float avg_temp;

esp_adc_cal_characteristics_t characteristics;
esp_err_t test = ESP_FAIL;

void setup() {

  //Initialize sensors (light, moisture, temperature)
  light_sensor = init_task_light_sensor(PIN_LTR303_SDA, PIN_LTR303_SCL);

  soil_sensor = init_task_soil_sensor(PIN_SOIL_PWM, PIN_SOIL_ADC);

  temp_sensor = init_task_temperature_sensor(PIN_TEMP_AMBIANT);

  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {

  static uint32_t soil_moisture = 0;

  task_light_sensor(&light_sensor, &avg_light);

  task_soil_sensor(&soil_sensor, &avg_soil);

  soil_moisture = adc1_get_raw(ADC1_CHANNEL_3);


  task_temperature_sensor(&temp_sensor, &avg_temp);

  delay(100);
}

