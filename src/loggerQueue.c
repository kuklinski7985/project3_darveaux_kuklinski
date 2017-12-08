/**
* @file loggerQueue.c
* @brief function definitions logger buffer, project3
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "conversion.h"
#include "profilingFxn.h"
#include "circbuff.h"
#include "binaryLogger.h"
#include "loggerQueue.h"

CB_status loggerEventToBuffer(CB_t * buff, uint8_t data)
{
	CB_status internalStatus;
	internalStatus = CB_buffer_add_item(buff,data);
	return internalStatus;
}

CB_status loggerRemoveFromBuffer(CB_t * buff, uint8_t * removedData)
{
	CB_status internalStatus;
	internalStatus = CB_buffer_remove_item(buff,removedData);
	return internalStatus;
}

CB_status loggingBuffer_full(CB_t * buff)
{
	CB_status internalStatus;
	internalStatus = CB_is_full(buff);
	return internalStatus;
}

CB_status loggingBuffer_empty(CB_t * buff)
{
	CB_status internalStatus;
	internalStatus = CB_is_empty(buff);
	return internalStatus;
}

CB_status loggingBuffer_peek(CB_t * buff, uint32_t positionFromHead, uint8_t *readValue)
{
	CB_status internalStatus;
	internalStatus = CB_peek(buff, positionFromHead,readValue);
	return internalStatus;
}

CB_status loggingBuffer_init(CB_t * buff, uint8_t length)
{
	CB_status internalStatus;
	internalStatus = CB_init(buff, length);
	return internalStatus;
}

CB_status loggingBuffer_destroy(CB_t * buff)
{
	CB_status internalStatus;
	internalStatus = CB_destroy(buff);
	return internalStatus;
}
