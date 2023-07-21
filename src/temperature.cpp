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

/* Private Defines --------------------------------------------------------*/
#define DEBUG_MODE  1
#define RESOLUTION_12_BIT   4095
#define VOLTAGE_REFERENCE_MV    3300
/* Private Macros ---------------------------------------------------------*/
/* Private Enumerations ---------------------------------------------------*/
/* Private Structures -----------------------------------------------------*/

/* Private Variables ------------------------------------------------------*/

/* In order to use the Beta equation, we must know our other resistor
  within our resistor divider. If you are using something with large tolerance,
  like at 5% or even 1%, measure it and place your result here in ohms. */
const double BALANCE_RESISTOR   = 10000.0;

// This helps calculate the thermistor's resistance (check article for details).
const double MAX_ADC            = 4095.0;

/* This is thermistor dependent and it should be in the datasheet, or refer to the
  article for how to calculate it using the Beta equation.
  I had to do this, but I would try to get a thermistor with a known
  beta if you want to avoid empirical calculations. */
const double BETA               = 3434.0;

/* This is also needed for the conversion equation as "typical" room temperature
  is needed as an input. */
const double ROOM_TEMP          = 298.15;   // room temperature in Kelvin

/* Thermistors will have a typical resistance at room temperature so write this 
  down here. Again, needed for conversion equations. */
const double RESISTOR_ROOM_TEMP = 10000.0;

/* Private Declarations Functions -----------------------------------------*/
/* Private Definitions Functions ------------------------------------------*/
static double convert_adc_to_temp(int* adc_value);

/* Public Definitions Functions -------------------------------------------*/

uint32_t buffer_temperature[CBUFFER_LENGTH] = {0};

temperature_t init_task_temperature_sensor(uint8_t pin_adc)
{
    temperature_t temp_sensor; 

    temp_sensor.pin_adc = pin_adc;

    analogReadResolution(12);

    analogSetAttenuation(ADC_11db);

    temp_sensor.cbuf = init_circular_buffer(buffer_temperature, CBUFFER_LENGTH);

    circular_buffer_set_n_avg(temp_sensor.cbuf, CBUFFER_AVG_EVERY_N);

    return temp_sensor;
}

void task_temperature_sensor(temperature_t* temp_sensor, float* avg_temp)
{
    static int adc_value = 0;
    static double temp_value_C = 0;

    //Read adc value for the soil
    adc_value = analogRead(temp_sensor->pin_adc);

    temp_value_C = convert_adc_to_temp(&adc_value);
    
    //Add data to circular buffer
    circular_buffer_add(temp_sensor->cbuf, (uint32_t)temp_value_C);

#if DEBUG_MODE == 1
    //Get the average from the circular buffer
    // Serial.print("Sum: ");
    // Serial.println(adc_value);

    //Get the average from the circular buffer
    // Serial.print("Average: ");
    // Serial.println(circular_buffer_get_avg(temp_sensor->cbuf));
#endif
}

static double convert_adc_to_temp(int* adc_value)
{
    double rThermistor = 0;            // Holds thermistor resistance value

    double tKelvin     = 0;            // Holds calculated temperature
    double tCelsius    = 0;            // Hold temperature in celsius

    rThermistor = BALANCE_RESISTOR * (3.295/((*adc_value)*2.8/4095.0)-1);

    tKelvin = (BETA * ROOM_TEMP) / 
    (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));

    tCelsius = tKelvin - 273.15;  // convert kelvin to celsius 

    return tCelsius;    // Return the temperature in Celsius
}

/* External Definitions Functions -----------------------------------------*/
/* Callback Definitions Functions -----------------------------------------*/