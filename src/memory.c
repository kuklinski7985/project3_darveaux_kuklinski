/**
* @file memory.c
* @brief This file is used to manipulate data in memory and allocate 
* space in memory
* @author Andrew Kuklinski and Mason Darveaux
* @date 9/30/2017
**/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"
#include "MKL25Z4.h"

uint8_t transfer_cnt = 0;

int8_t * my_memset(uint8_t * src, size_t length, uint8_t value)
{

  //checking for invalid NULL pointers
  if(src == NULL)
    {
      return (void*)NULL;
    }
  //sets a number of bytes (defined by length) in memory equal to value, starting
  //the src address 
  for(int i=0; i<length; i++)
  {
    *src = value;
    src++;
  }
  // resets src pointer to original value
  return ((int8_t*) src-length);
}


uint8_t * my_memzero(uint8_t * src, size_t length)
{
  //checking for invalid NULL pointers
  if(src == NULL)
    {
      return (void*)NULL;
    }
  //sets a number of bytes (defined by length) in memory equal to zero, starting
  //at the src address 
  for(int i=0; i<length; i++)
  {
    *src = 0;
    src++;
  }
  // returns src pointer to original value
  return ((uint8_t*) src-length);
}


uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
  //checking for invalid NULL pointers
  if(src == NULL || dst == NULL)
    {
      return (void*)NULL;
    }

  //defines an array to hold the src data in order to prevent corruption
  uint8_t holdArray[length];
  uint8_t *holdArrayPtr = holdArray;
 
  //copies src data into the hold array
  for(int i=0; i<length; i++)
  {
    *holdArrayPtr = *src;
    src++;
    holdArrayPtr++;
  }
  
  //resets holdArrayPtr to original value
  holdArrayPtr = holdArray;

  //copies data from hold array into destination
  for(int j=0; j<length; j++)
  {
    *dst = *holdArrayPtr;
    dst++;
    holdArrayPtr++;
  }

  //returns destination pointer to original value
  return ((uint8_t*) dst-length);
}

uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length)
{

  //copies data directly from source location to destination
  for(int i=0; i<length; i++)
  {
    *dst = *src;
    src++;
    dst++;
  }

  //resets dst pointer to original value
  return (dst - length);
}


uint8_t * my_reverse(uint8_t * src, size_t length)
{

  if(src == NULL)
  {
    return (void*)NULL;
  }
  //defines pointers to the beginning and end of array of data
  uint8_t *ptrHead = src;    
  uint8_t *ptrTail = src + (length-1); 

  //defines intermediate variable to store data when swapping bytes  
  uint8_t intermediate = 0;     


  if(ptrHead == (ptrTail-1))
      {
        intermediate = *ptrHead;
        *ptrHead = *ptrTail;
        *ptrTail = intermediate;
      }

  //will continue to loop until head and tail pointers are equal, 
  //or they are adjacent

  while(!((ptrHead == ptrTail) || (ptrHead == (ptrTail -1))))
  {
    //swaps the bytes of the array
    intermediate = *ptrHead;
    *ptrHead = *ptrTail;
    *ptrTail = intermediate;
    ptrHead++;
    ptrTail--;
	
    // if head and tail pointers are adjacent, perform one additional swap before 
    // exiting the loop
    if(ptrHead == (ptrTail-1))
    {
      intermediate = *ptrHead;
      *ptrHead = *ptrTail;
      *ptrTail = intermediate; 
    }
  }
  return src;
}

int32_t * reserve_words(size_t length)
{
  //defines a pointer to be used to return the address of the allocated memory
  int32_t * reservePtr;

  //allocates a number of words in memory defined by the length value
  reservePtr = (int32_t *) malloc((sizeof(size_t)*length));

  //malloc returns a 0 if it fails
  if(reservePtr == 0)
  {
    return NULL;
  }
  return reservePtr;
}

void free_words(uint32_t * src)
{
  //frees memory previously allocated using malloc
  free(src);
}

void memmove_dma(uint8_t * src, uint8_t * dst, size_t length)
{
	uint32_t source_dummy = (uint32_t)src;
	uint32_t destination_dummy =(uint32_t)dst;
	DMA_SAR0 |= source_dummy;   // set the DMA0 SAR to hold the src address
	DMA_DAR0 |= destination_dummy;   // set the DMA0 DAR to hold the dst address

	DMA_DSR_BCR0 |= length; // set the DMA0 BCR register to hole the number of bytes to be transferred


	DMA_DCR0 |= DMA_DCR_START_MASK; // start the DMA transfer

}

void memset_dma(uint8_t * src, size_t length, uint8_t value, uint8_t * dummy_address)
{
	uint16_t i;

	for (i=0; i<length; i++)
	{
		*(dummy_address+i)=value;
	}

	uint32_t source_dummy = (uint32_t)dummy_address;
	uint32_t destination_dummy = (uint32_t)src;

	DMA_SAR0 |= source_dummy;   // set the DMA0 SAR to hold the address of the array of bytes to be written to the target locations
	DMA_DAR0 |= destination_dummy;   // set the DMA0 DAR to hold the src address to be written to

	DMA_DSR_BCR0 |= length; // set the DMA0 BCR register to hole the number of bytes to be transferred

	DMA_DCR0 |= DMA_DCR_START_MASK; // start the DMA transfer

}

void DMA_init()
{
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;  //enable clock signal to DMAMUX module

	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK; //enable clock signal to DMA module

	DMAMUX0_CHCFG0 &= ~(DMAMUX_CHCFG_ENBL_MASK); //disable DMA MUX channel 0 in order to configure the DMA MUX channel
	DMAMUX0_CHCFG0 &= ~(DMAMUX_CHCFG_TRIG_MASK); //disable the triggering function of DMA channel 0
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_SOURCE_MASK;  //configure DMA MUX source to be always enabled and use the DMA channel MUX to reactivate the channel after a transfer
	DMAMUX0_CHCFG0 |= ~(DMAMUX_CHCFG_ENBL_MASK); //enable DMA MUX channel 0

	DMA_DCR0 |= 0x20000;  // sets bit 17 of DCR equal to 1 in order to enable 8 bit destination transfers
	DMA_DCR0 |= DMA_DCR_DINC_MASK;  // sets bit 19 of DCR equal to 1 in order to increment the DAR after a successful transfer
	DMA_DCR0 |= 0x100000;  //sets bit 20 of the DCR equal to 1 in order to enable 8 bit source transfers
	DMA_DCR0 |= DMA_DCR_SINC_MASK; // sets bit 22 of the DCR equal to 1 in order to increment the SAR after a successful transfer
	//DMA_DCR0 |= DMA_DCR_EINT_MASK; // sets bit 31 of the DCR equal to 1 in order to generate an interrupt after a complete transfer



}

void DMA0_IRQHandler()
{
	transfer_cnt++;
}

