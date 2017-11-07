/**
* @file circbuff.h
* @brief Holds public interfaces to a circular buffer
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/


#include <stdio.h>
#include <stdint.h>

#ifndef circularBuffer_h_
#define circularBuffer_h_

/*struct and emun are used to interact with and control the circular buffer*/
typedef struct {
  uint8_t * circbuff;           //allocated memory for the buffer elements
  uint8_t * headptr;            //pointer to head or newest item
  uint8_t * tailptr;            //pointer to the tail or oldest item
  uint8_t buffLength;         //number of items allocated to the buffer
  uint8_t count;              //current item count in the buffer
  uint8_t * poppedData;         //place to store data removed from buffer
}CB_t;


typedef enum {
  buffer_full, buffer_empty, no_error, null_error
}CB_status;


/**
 *@brief Adds item to the circular buffer given a pointer
 *
 *@param "buff" pointer to circular buffer
 *@param "data" data to be added to the buffer
 *
 *@return enumeration that specifies the success/failure/ect of the fxn call

 */
CB_status CB_buffer_add_item(CB_t * buff, uint8_t data);


/**
 *@brief remove item from the buffer
 *
 *@param "buff" pointer to buffer from which item is to be removed
 *@param "removedData" data that was removed and returned
 *
 *@return enumeration that specifies the success/failure/ect of the fxn call

 */
CB_status CB_buffer_remove_item(CB_t * buff, uint8_t * removedData);


/**
 *@brief checks to see if buffer is full
 *
 *@param "buff" buffer to check
 *
 *@return status of buffer or an error code

 */
CB_status CB_is_full(CB_t * buff);


/**
 *@brief checks to see if buffer is empty
 *
 *@param "buff" buffer to check
 *
 *@return the status of the buffer or an error code

 */
CB_status CB_is_empty(CB_t * buff);


/**
 *@brief allows you to check the value of any position of the buffer
 *
 *@param "buff" buffer to look into
 *@param "buffValue" value of buffer position
 *
 *@return value of the position given in the buffer

 */
CB_status CB_peek(CB_t * buff, uint32_t positionFromHead, uint8_t *readValue);

/**
 *@brief initializes a buffer
 *
 *@param "buff"  buffer to be initialized
 *@param "length" number of bytes to reserve
 *
 *@return

 */
CB_status CB_init(CB_t * buff, uint32_t length);

/**
 *@brief  destroys current buffer and makes memory available
 *
 *@param "buff" buffer to distroy
 *
 *@return

 */
CB_status CB_destroy(CB_t * buff);

#endif /*__circularBuffer_h__*/
