/**
 * @file        circular_buffer.h
 * @authors   	Marc-Antoine Doyon
 * @copyright 	2023, Marc-Antoine Doyon @madoyon - GitHub
 * You may use, distribute and modify this code under the terms of the MIT license 
 * This code is strongly based upon embedded-resources (@embeddedartistry) example "circular_buffer"
 * @date		2023-07-26
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