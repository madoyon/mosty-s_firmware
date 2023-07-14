/**
 * @file        ${file_base}.c
 * @authors   	${user}
 * @copyright 	Evident CopyRight.
 * @date		${date}
 */

/* Private Includes -------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "Arduino.h"

#include "circular_buffer.h"
/* Private Defines --------------------------------------------------------*/
/* Private Macros ---------------------------------------------------------*/
/* Private Enumerations ---------------------------------------------------*/
/* Private Structures -----------------------------------------------------*/

/* Private Variables ------------------------------------------------------*/
/* Private Declarations Functions -----------------------------------------*/

static inline size_t moveup_headtail_value(size_t value, size_t max)
{
    return (value + 1) % max;
}

static inline void moveup_headtail_pointer(circular_buffer_t* me)
{
    static uint32_t buffer_size;
    buffer_size = circular_buffer_size(me);

    if(me->isFull == true)
    {
        me->tail = moveup_headtail_value(me->tail, me->max);
        me->sum -= me->buffer[me->tail];
        me->sum += me->buffer[me->head];
    }
    else 
    {
        me->sum += me->buffer[me->head];
    }

    if(me->avg_every_n > 0) 
    {
        me->avg_cntr++;     
        if(me->avg_cntr >= me->avg_every_n && me->sum > 0)
        {
            me->avg_cntr = 0;
            me->average = (float)me->sum/(float)buffer_size;
        }
    }

    me->head = moveup_headtail_value(me->head, me->max);
    me->isFull = (me->head == me->tail);
}

/* Private Definitions Functions ------------------------------------------*/
/* Public Definitions Functions -------------------------------------------*/

circular_buffer_t* init_circular_buffer(uint32_t* buffer, size_t size) 
{
    circular_buffer_t* cbuf = (circular_buffer_t*)malloc(sizeof(circular_buffer_t));

    cbuf->average = 0;
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->max = size;
    cbuf->sum = 0;
    cbuf->buffer = buffer;

    cbuf->average = 0.0;
    cbuf->avg_every_n = 0;
    cbuf->avg_cntr = 0;

    return cbuf;
}

void circular_buffer_set_n_avg(circular_buffer_t* me, size_t avg_every_n)
{
    me->avg_every_n = avg_every_n;
}

void circular_buffer_free(circular_buffer_t* me)
{
    free(me);
}

void circular_buffer_add(circular_buffer_t* me, size_t data)
{
    me->buffer[me->head] = data;
    moveup_headtail_pointer(me); 
}

void circular_buffer_reset(circular_buffer_t* me)
{
    me->head = 0;
    me->tail = 0; 
    me->isFull = false;
}

size_t circular_buffer_size(circular_buffer_t* me)
{
    size_t size = me->max;

    if(me->isFull == false)
    {
        if(me->head >= me->tail)
        {
            size = me->head - me->tail;
        }
        else 
        {
            size = me->max + me->head - me->tail;
        }

    }
    return size;
}

size_t circular_buffer_capacity(circular_buffer_t* me)
{
    return me->max;
}

int circular_buffer_get(circular_buffer_t* me, uint32_t* data)
{
    int r = -1;

    if(me->isFull == false)
    {
        *data = me->buffer[me->tail];
        me->tail = moveup_headtail_value(me->tail, me->max);
        me->isFull = false;
        r = 0;
    }

    return r;
}

bool circular_buffer_full(circular_buffer_t* me)
{
	return me->isFull;
}

float circular_buffer_get_avg(circular_buffer_t* me)
{
    return me->average;
}

/* External Definitions Functions -----------------------------------------*/
/* Callback Definitions Functions -----------------------------------------*/