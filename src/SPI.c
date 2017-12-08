/*
 * SPI.c
 *
 *  Created on: Dec 2, 2017
 *      Author: roda8190
 */

#include "SPI.h"
#include <stdint.h>
#include "MKL25Z4.h"

void SPI_init()
{
	//Enable clock signal to SPI0
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;

	//configures SPI0 as a master, enables the SPI system, sets CPHA =0 and CPOL =0
	SPI0->C1 = 0x50; // sets bits 4 and 6 equal to 1, leaves the rest equal to 0

	SPI0->C2 = 0x10; //sets bit 4, MODFEN =1 SS acts as a slave select output

	SPI0->BR = 0x01; //sets SPI BAUD Rate divisor to 32, CLK input is 24MHZ, SPICLK should be 750 KHz




}


uint8_t SPI_read_byte()
{
	uint8_t byte =0;

	while (!(SPI0->S && 0x80)); // wait for SPRF flag to be set indicating there is data to be read

	byte = SPI0_D;

	return byte;


}

void SPI_write_byte(uint8_t byte)
{

	SPI_flush();

	SPI0_D = byte;


}

void SPI_send_packet(uint8_t* p, uint8_t length)
{
	int16_t i;

	for (i=0; i<length; i++)
	{
		while (!(SPI0->S && 0x20)); // wait for SPTEF flag to be set indicating the transmit buffer is empty
		SPI0_D = *(p+i);
	}


}

void SPI_flush()
{
	while (!(SPI0->S && 0x20)); // wait for SPTEF flag to be set indicating the transmit buffer is empty

}

