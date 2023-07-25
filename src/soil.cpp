/**
 * @file        ${file_base}.c
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Private Includes -------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "soil.h"
#include "esp32-hal-ledc.h"

/* Private Defines --------------------------------------------------------*/
#define LEDC_CHANNEL_0  0
#define LEDC_FREQUENCY_HZ 100000    //100kHz
#define LEDC_TIMER_2_BIT  2         //2 bits duty cyle: 25%, 50%, 75%, 100%

#define DEBUG_MODE  0
/* Private Macros ---------------------------------------------------------*/
/* Private Enumerations ---------------------------------------------------*/
/* Private Structures -----------------------------------------------------*/

/* Private Variables ------------------------------------------------------*/
/* Private Declarations Functions -----------------------------------------*/
/* Private Definitions Functions ------------------------------------------*/
/* Public Definitions Functions -------------------------------------------*/

uint32_t buffer_soil[CBUFFER_LENGTH] = {0};

soil_t init_task_soil_sensor(uint8_t pin_pwm, uint8_t pin_adc)
{
    soil_t soil_sensor; 

    soil_sensor.pin_adc = pin_adc;

    ledcSetup(LEDC_CHANNEL_0, LEDC_FREQUENCY_HZ, LEDC_TIMER_2_BIT);
    ledcAttachPin(pin_pwm, LEDC_CHANNEL_0);

    soil_sensor.cbuf = init_circular_buffer(buffer_soil, CBUFFER_LENGTH);

    circular_buffer_set_n_avg(soil_sensor.cbuf, CBUFFER_AVG_EVERY_N);

    return soil_sensor;
}

void task_soil_sensor(soil_t* soil_sensor, float* avg_soil)
{
    if(!soil_sensor->isInitialized)
    {     
        //Start duty cycle and pwm
        ledcWrite(LEDC_CHANNEL_0, 1);
        soil_sensor->isInitialized = true;
    }

    static uint16_t soil_value = 0;
    //Read adc value for the soil
    soil_value = analogRead(soil_sensor->pin_adc);

    //Add data to circular buffer
    circular_buffer_add(soil_sensor->cbuf, soil_value);

    *avg_soil = (float)circular_buffer_get_avg(soil_sensor->cbuf);

#if DEBUG_MODE == 1
    //Get the average from the circular buffer
    Serial.print("Sum: ");
    Serial.println(soil_value);

    //Get the average from the circular buffer
    Serial.print("Average: ");
    Serial.println(circular_buffer_get_avg(soil_sensor->cbuf));
#endif
}


/* External Definitions Functions -----------------------------------------*/
/* Callback Definitions Functions -----------------------------------------*/