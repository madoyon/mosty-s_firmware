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

    uint32_t* buffer;
   
    bool isFull;
};


/* Global Variables -------------------------------------------------------*/
/* Public Declarations functions ------------------------------------------*/

circular_buffer_t* init_circular_buffer(uint32_t* buffer, size_t size);

void circular_buffer_set_n_avg(circular_buffer_t* me, size_t avg_every_n);

void circular_buffer_free(circular_buffer_t* me);

void circular_buffer_add(circular_buffer_t* me, size_t data);

void circular_buffer_reset(circular_buffer_t* me);

size_t circular_buffer_size(circular_buffer_t* me);

size_t circular_buffer_capacity(circular_buffer_t* me);

int circular_buffer_get(circular_buffer_t* me, uint32_t* data);

int circular_buffer_get(circular_buffer_t* me, uint32_t* data);

float circular_buffer_get_avg(circular_buffer_t* me);

/* External Declarations functions ----------------------------------------*/

#endif	/* CIRCULAR_BUFFER_H_ */