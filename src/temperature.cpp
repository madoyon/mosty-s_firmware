/**
 * @file        ${file_base}.c
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Private Includes -------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "temperature.h"
#include "Arduino.h"
#include "math.h"
#include "esp_adc_cal.h"


/* Private Defines --------------------------------------------------------*/
#define DEBUG_MODE  1
#define RESOLUTION_12_BIT   4095
#define VOLTAGE_REFERENCE_MV    3300
/* Private Macros ---------------------------------------------------------*/
/* Private Enumerations ---------------------------------------------------*/
/* Private Structures -----------------------------------------------------*/

/* Private Variables ------------------------------------------------------*/

const double BALANCE_RESISTOR   = 10000.0;

const double MAX_ADC            = 4095.0;

const double BETA               = 3434.0;

const double ROOM_TEMP          = 298.15; // room temperature in Kelvin

const double RESISTOR_ROOM_TEMP = 10000.0;

esp_adc_cal_characteristics_t esp_cal;

/* Private Declarations Functions -----------------------------------------*/
/* Private Definitions Functions ------------------------------------------*/
static double convert_adc_to_temp(uint32_t* adc_value);

/* Public Definitions Functions -------------------------------------------*/

uint32_t buffer_temperature[CBUFFER_LENGTH] = {0};

temperature_t init_task_temperature_sensor(uint8_t pin_adc)
{
    temperature_t temp_sensor; 
    static esp_err_t cal_status = ESP_FAIL;
    
    //Initialize temperature sensor struct and buffer
    temp_sensor.cbuf = init_circular_buffer(buffer_temperature, CBUFFER_LENGTH);
    circular_buffer_set_n_avg(temp_sensor.cbuf, CBUFFER_AVG_EVERY_N);

    temp_sensor.pin_adc = pin_adc;

    //Configuration of ADC1 (CH1-CH5)
    adc1_config_width(ADC_WIDTH_BIT_12);

    //Set attenunation to 11dB for range 0-2500mV
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);

    cal_status = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP); 
    if(cal_status == ESP_OK)
    {
      Serial.println(" The calibration mode is supported in eFuse");
    }
    else if(cal_status == ESP_ERR_NOT_SUPPORTED)
    {
      Serial.println("Error, eFuse values are not burned");
    }
    ESP_ERROR_CHECK( cal_status );

    //Calibrate ADC
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_12Bit, 2500, &esp_cal);
    
    return temp_sensor;
}

void task_temperature_sensor(temperature_t* temp_sensor, float* avg_temp)
{
    static int adc_raw = 0;
    static int adc_mV = 0;
    static uint32_t avg_adc_mV = 0;
    static double temp_value_C = 0;

    //Read adc value for the soil
    adc_raw = analogRead(temp_sensor->pin_adc);
    adc_mV = esp_adc_cal_raw_to_voltage(adc_raw, &esp_cal);

    //Add data to circular buffer and convert avg to temperature
    circular_buffer_add(temp_sensor->cbuf, (uint32_t)adc_mV);
    avg_adc_mV = circular_buffer_get_avg(temp_sensor->cbuf);
    temp_value_C = convert_adc_to_temp(&avg_adc_mV);

    *avg_temp = (float)temp_value_C;
    
#if DEBUG_MODE == 1
    //Get the average from the circular buffer
    // Serial.print("Sum: ");
    // Serial.println(adc_value);

    //Get the average from the circular buffer
    // Serial.print("Average: ");
    // Serial.println(circular_buffer_get_avg(temp_sensor->cbuf));
#endif
}

static double convert_adc_to_temp(uint32_t* adc_mV)
{
    double rThermistor = 0;            // Holds thermistor resistance value

    double tKelvin     = 0;            // Holds calculated temperature
    double tCelsius    = 0;            // Hold temperature in celsius

    rThermistor = BALANCE_RESISTOR * ((3295.0/(double)(*adc_mV))-1);

    tKelvin = (BETA * ROOM_TEMP) / 
    (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));

    tCelsius = tKelvin - 273.15;  // convert kelvin to celsius 

    return tCelsius;    // Return the temperature in Celsius
}

/* External Definitions Functions -----------------------------------------*/
/* Callback Definitions Functions -----------------------------------------*/