/**
 * @file        light.cpp
 * @authors   	Marc-Antoine Doyon
 * @copyright 	2023, Marc-Antoine Doyon @madoyon - GitHub
 * You may use, distribute and modify this code under the terms of the MIT license 
 * @date		2023-07-26
 */

/* Private Includes -------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "light.h"

/* Private Defines --------------------------------------------------------*/
/* Private Macros ---------------------------------------------------------*/
/* Private Enumerations ---------------------------------------------------*/
/* Private Structures -----------------------------------------------------*/

/* Private Variables ------------------------------------------------------*/
/* Private Declarations Functions -----------------------------------------*/
/* Private Definitions Functions ------------------------------------------*/
/* Public Definitions Functions -------------------------------------------*/
uint32_t buffer_visible_ir[CBUFFER_LENGTH] = {0};

LTR303_t init_task_light_sensor(uint8_t sda, uint8_t scl)
{
    LTR303_t light_sensor;
    light_sensor.ltr303 = Adafruit_LTR303();
    light_sensor.ir_lux = 0;
    light_sensor.visible_ir_lux = 0;
    light_sensor.isInitialized = false;

    //Set I2C pins 
    Wire.setPins(sda, scl);

    light_sensor.cbuf = init_circular_buffer(buffer_visible_ir, CBUFFER_LENGTH);

    circular_buffer_set_n_avg(light_sensor.cbuf, CBUFFER_AVG_EVERY_N);

    light_sensor.isInitialized = light_sensor.ltr303.begin(&Wire);
    
    //Set gain, integration time and measurement rate
    light_sensor.ltr303.setGain(LTR3XX_GAIN_8);
    light_sensor.ltr303.setIntegrationTime(LTR3XX_INTEGTIME_200);
    light_sensor.ltr303.setMeasurementRate(LTR3XX_MEASRATE_1000);  

    return light_sensor;
}

void task_light_sensor(LTR303_t* light_sensor, float* avg_light)
{
    static uint16_t ch0, ch1;
    //Read both channels (visible + ir & ir)
    light_sensor->ltr303.readBothChannels(ch0, ch1);

    light_sensor->visible_ir_lux = ch0;
    light_sensor->ir_lux = ch1;
    
    //Add data to circular buffer
    circular_buffer_add(light_sensor->cbuf, ch0);

    *avg_light = circular_buffer_get_avg(light_sensor->cbuf);
}

/* External Definitions Functions -----------------------------------------*/
/* Callback Definitions Functions -----------------------------------------*/