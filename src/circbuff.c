/**
* @file circbuff.c
* @brief function definitions for interfacing and controlling a circular buffer
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "circbuff.h"


CB_status CB_buffer_add_item(CB_t * buff, uint8_t data)
{
  if(CB_is_full(buff) == buffer_full)  //checks to see if buffer is full
  {
    return buffer_full;
  }
  else
  {
    *buff->headptr = data;
    buff->count++;
  }
  //*buff->headptr = data;
  //buff->count++;

  /*checks coniditions for wrap around, if true then sets back to start
  * else increases headptr to next location*/
  
  if(((buff->headptr)) == ((buff->circbuff) + (buff->buffLength)))
  {
    buff->headptr = buff->circbuff;
  }
  else
  {
    buff->headptr++;
  }
  
  return no_error;
}



CB_status CB_buffer_remove_item(CB_t * buff, uint8_t * removedData)
{
  //check to see if buffer is empty
  if(CB_is_empty(buff) == buffer_empty)
    {
      return buffer_empty;
    }
  else  //if not empty the read data and decrement count
    {
      *removedData = *buff->tailptr;
      buff->count--;
    }
  

  if(((buff->tailptr)) == ((buff->circbuff) + (buff->buffLength)))
    {
      buff->tailptr = buff->circbuff;
    }
  else
    {
      buff->tailptr++;
    }
  return no_error;
}

CB_status CB_is_full(CB_t * buff)
{
  if((buff->count) == (buff->buffLength-1))
  {
    return buffer_full;
  }
  else
  {
    return no_error;
  }
}

CB_status CB_is_empty(CB_t * buff)
{
  if((buff->headptr) == (buff->tailptr))
    {
      return buffer_empty;
    }
  else
    {
      return no_error;
    }
}

CB_status CB_peek(CB_t * buff, uint32_t positionFromHead, uint8_t *readValue)
{
  uint32_t dist2End = 0;
  uint32_t newPos = 0;

  if(((buff->headptr)+positionFromHead) >= ((buff->circbuff) + (buff->buffLength)))
    {
      dist2End = (buff->buffLength) - (((buff->circbuff)+(buff->buffLength))-(buff->headptr))+1;
      //printf("dist2end: %d\n\n",dist2End);
      newPos = positionFromHead - dist2End - 1;
      //printf("newPos: %d\n",newPos);
      *readValue = *((buff->circbuff) + newPos);
      //printf("readValue Add: %p\n",((buff->circbuff) + newPos));
    }
    else
    {
      *readValue = *((buff->headptr) + positionFromHead);
      //printf("readValue Add: %p\n",readValue);
    }
  return no_error;
}

CB_status CB_init(CB_t * buff, uint32_t length){

  buff->circbuff  = (uint8_t*)malloc((sizeof(size_t))*length);

  
  if (buff->circbuff == NULL)
  {
    return null_error;
  }
  else
  {
    buff->headptr = buff->circbuff;
    buff->tailptr = buff->circbuff;
    buff->poppedData = buff->circbuff;

    buff->buffLength = length;   //setting the length of the buffer
    buff->count = 0;             //set current item count of buffer to zero
    return no_error;
  }
}

CB_status CB_destroy(CB_t * buff){
  if(buff == NULL)
  {
    return null_error;
  }
  else
  {
    free(buff);
    return no_error;
  }
}
