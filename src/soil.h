/**
 * @file      	${file_base}.h
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Header Guard -----------------------------------------------------------*/

#ifndef SOIL_H_
#define SOIL_H_

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
  bool isInitialized = false;
} soil_t;


/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/
soil_t init_task_soil_sensor(uint8_t pin_pwm, uint8_t pin_adc);

void task_soil_sensor(soil_t* soil, float* avg_soil);

/* External Declarations functions ----------------------------------------*/

#endif	/* LIGHT_H_ */