/*
 * gpio.c
 *
 *  Created on: Dec 2, 2017
 *      Author: roda8190
 */
#include "MKL25Z4.h"

void GPIO_nrf_init()
{


	PORTC->PCR[5] = PORT_PCR_MUX(2);  // enable SPI0 SCK MUX (MUX 2)
	PORTC->PCR[6] = PORT_PCR_MUX(2);  // enable SPI0 MOSI MUX (MUX 2)
	PORTC->PCR[7] = PORT_PCR_MUX(2);  // enable SPI0 MISO MUX (MUX 2)

	PORTC->PCR[9] = PORT_PCR_MUX(1);  // enable PTC pin 9 MUX (MUX 1) in order to use a regular GPIO pin as the PCS line

	PTC_BASE_PTR->PDDR |= 1<<9;  // set port C pin 4 as an output (PCS)


	PTC_BASE_PTR->PSOR |= 1<<9; //set CS pin to high

}

