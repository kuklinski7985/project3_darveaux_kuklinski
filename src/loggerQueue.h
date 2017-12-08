/**
* @file loggerQueue.h
* @brief
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef loggerQueue_h_
#define loggerQueue_h_

CB_status loggerEventToBuffer(CB_t * buff, uint8_t data);
CB_status loggerRemoveFromBuffer(CB_t * buff, uint8_t * removedData);
CB_status loggingBuffer_full(CB_t * buff);
CB_status loggingBuffer_empty(CB_t * buff);
CB_status loggingBuffer_peek(CB_t * buff, uint32_t positionFromHead, uint8_t *readValue);
CB_status loggingBuffer_init(CB_t * buff, uint8_t length);
CB_status loggingBuffer_destroy(CB_t * buff);

#endif /*loggerQueue_h_*/
