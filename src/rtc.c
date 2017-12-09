/*
 * rtc.c
 *
 *  Created on: Dec 6, 2017
 *      Author: roda8190
 */

#include "rtc.h"
#include <stdint.h>
#include "MKL25Z4.h"
#include "binaryLogger.h"
#include "project3.h"

int RTC_cnt =0;

void rtc_init()
{
	  // enable internal reference clock, MCGIRCLK is active
	  MCG_C1 |= MCG_C1_IRCLKEN_MASK;

	  // select the slow internal reference clock source
	  MCG_C2 &= ~(MCG_C2_IRCS_MASK);

	  //Set PTC1 as RTC_CLKIN and select 32 KHz clock source for the RTC module.
	  SIM_SOPT1  &= SIM_SOPT1_OSC32KSEL(0b00);
	  SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b10);
	  PORTC->PCR[1] = PORT_PCR_MUX(1);

	  //Set PTC3 as CLKOUT pin and selects the MCGIRCLK clock to output on the CLKOUT pin.
	  SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(0b100);
	  PORTC->PCR[3] = PORT_PCR_MUX(5);


	  //Enable software access and interrupts to the RTC module.
	  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;


	  // Clear all RTC registers
	  RTC_CR = RTC_CR_SWR_MASK;
	  RTC_CR &= ~RTC_CR_SWR_MASK;

	  if (RTC_SR & RTC_SR_TIF_MASK){
	       RTC_TSR = 0x00000000;
	  }


	  //Set RTC compensation interval to be _ seconds
	  RTC_TCR = RTC_TCR_CIR(1) | RTC_TCR_TCR(0xFF);

	  RTC_IER = 0x00;  // Clear IER register

	  //Enable seconds interrupt for RTC module and enable the irq


	  RTC_IER |= RTC_IER_TSIE_MASK;

	  // Set Time Seconds Register
	  RTC_TSR = 0x01;


	  //Enable time counter
	  RTC_SR |= RTC_SR_TCE_MASK;





}

void RTC_Seconds_IRQHandler()
{

	uint8_t payloadStr[] = "Heartbeat";

	logOutputData(heartbeat_ptr, payloadStr , HEARTBEAT);

	log_item(heartbeat_ptr,loggerBuffer);

	RTC_cnt++;



}

