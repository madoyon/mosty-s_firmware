/**
 * @file      	${file_base}.h
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Header Guard -----------------------------------------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

/* Public Includes --------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

/* Public Defines ---------------------------------------------------------*/

//RBG LED
#define PIN_LED_GREEN       1U
#define PIN_LED_RED         5U
#define PIN_LED_BLUE        7U

//Sensors
#define PIN_SOIL_ADC        0U
#define PIN_SOIL_PWM        6U

#define PIN_BAT_ADC         4U

#define PIN_LTR303_INT      10U      
#define PIN_LTR303_SDA      20U
#define PIN_LTR303_SCL      21U

#define PIN_TEMP_AMBIANT    3U

//Push buttons
#define PIN_BOOT_PB         9U

/* Public Macros ----------------------------------------------------------*/
/* Public Enumerations ----------------------------------------------------*/
/* Public Structures ------------------------------------------------------*/
/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/
/* External Declarations functions ----------------------------------------*/

#endif	/* MAIN_H_ */