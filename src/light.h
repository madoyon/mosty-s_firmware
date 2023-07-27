/**
 * @file        light.h
 * @authors   	Marc-Antoine Doyon
 * @copyright 	2023, Marc-Antoine Doyon @madoyon - GitHub
 * You may use, distribute and modify this code under the terms of the MIT license 
 * @date		2023-07-26
 */

/* Header Guard -----------------------------------------------------------*/

#ifndef LIGHT_H_
#define LIGHT_H_

/* Public Includes --------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "Adafruit_LTR329_LTR303.h"
#include "circular_buffer.h"
#include <Wire.h>

/* Public Defines ---------------------------------------------------------*/
#define CBUFFER_LENGTH  10
#define CBUFFER_AVG_EVERY_N   3 //Average every 10 values 

/* Public Macros ----------------------------------------------------------*/
/* Public Enumerations ----------------------------------------------------*/
/* Public Structures ------------------------------------------------------*/

typedef struct {
  Adafruit_LTR303 ltr303;
  uint16_t visible_ir_lux = 0;
  uint16_t ir_lux = 0;
  circular_buffer_t* cbuf;
  bool isInitialized = false;
} LTR303_t;

// LTR303_t light_sensor;

/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/
LTR303_t init_task_light_sensor(uint8_t sda, uint8_t scl);

void task_light_sensor(LTR303_t* light_sensor, float* avg_light);

/* External Declarations functions ----------------------------------------*/

#endif	/* LIGHT_H_ */