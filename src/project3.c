/**
* @file project3.c
* @brief 
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "circbuff.h"
#include "uart.h"
#include "debug.h"
#include "project3.h"
//#include "MKL25Z4.h"
#include "conversion.h"
#include "memory.h"
#include <time.h>


void project3(void)
{
  profileAll();

}

void profileAll()
{

  uint8_t *destPtr;
  uint8_t *srcPtr;
  uint16_t n = 5000;
  
  destPtr = (uint8_t*) malloc((sizeof(size_t))*n);
  if (destPtr != NULL)
    {
      printf("dest is good.\n");
    }
  
  srcPtr = (uint8_t*) malloc((sizeof(size_t))*n);
    if (srcPtr != NULL)
    {
      printf("src is good.\n");
    }


  clock_t start, end;
  uint16_t totalSum = 0;
  uint8_t exeTime;

  for(int i=0; i<=99; i++)
    {
      start = clock();
      my_memmove(destPtr,srcPtr,n);
      end = clock();
      exeTime = end - start;
      totalSum += exeTime;
      printf("clock ticks to complete: %d\n",exeTime);
    }
  printf("average: %d\n",(totalSum/100));

  free(destPtr);
  free(srcPtr);

  /*clock_t ticks1, ticks2;
  uint32_t diff;
  ticks1 = clock();
  printf("ticks1: %ld\n\n",(ticks1/CLOCKS_PER_SEC));
  ticks2=ticks1;
  while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC)<30)
    {
      ticks2=clock();
    }

  printf("Took %ld ticks to wait one second.\n",ticks2-ticks1);
  printf("This value should be the same as CLOCKS_PER_SEC which is %ld.\n", CLOCKS_PER_SEC);

  diff = difftime(ticks2,ticks1);
  printf("diff: %ld\n",(diff/(CLOCKS_PER_SEC/1000)));*/

  return;
}
