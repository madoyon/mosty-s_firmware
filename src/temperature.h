/**
 * @file        temperature.h
 * @authors   	Marc-Antoine Doyon
 * @copyright 	2023, Marc-Antoine Doyon @madoyon - GitHub
 * You may use, distribute and modify this code under the terms of the MIT license 
 * @date		2023-07-26
 */

/* Header Guard -----------------------------------------------------------*/

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

/* Public Includes --------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "Arduino.h"

/* Public Defines ---------------------------------------------------------*/
#define CBUFFER_LENGTH  10
#define CBUFFER_AVG_EVERY_N   3 //Average every 10 values 

/* Public Macros ----------------------------------------------------------*/
/* Public Enumerations ----------------------------------------------------*/
/* Public Structures ------------------------------------------------------*/

typedef struct {
  circular_buffer_t* cbuf;
  uint8_t pin_adc;
} temperature_t;

/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/
temperature_t init_task_temperature_sensor(uint8_t pin_adc);

void task_temperature_sensor(temperature_t* soil, float* avg_temp);

/* External Declarations functions ----------------------------------------*/

#endif	/* TEMPERATURE_H_ */