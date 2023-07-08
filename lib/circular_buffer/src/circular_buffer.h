/**
 * @file      	${file_base}.h
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Header Guard -----------------------------------------------------------*/

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

/* Public Includes --------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

/* Public Defines ---------------------------------------------------------*/
/* Public Macros ----------------------------------------------------------*/
/* Public Enumerations ----------------------------------------------------*/
/* Public Structures ------------------------------------------------------*/


struct circular_buffer_t {
    size_t head;
    size_t tail;
    size_t max; //size
    ssize_t sum;

    float average;
    size_t avg_every_n;
    size_t avg_cntr;

    uint8_t* buffer;
   
    bool isFull;
};


/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/

circular_buffer_t* init_circular_buffer(uint8_t* buffer, size_t size);
/* External Declarations functions ----------------------------------------*/

#endif	/* CIRCULAR_BUFFER_H_ */