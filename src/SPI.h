/*
 * SPI.h
 *
 *  Created on: Dec 2, 2017
 *      Author: roda8190
 */

#ifndef SOURCES_SPI_H_
#define SOURCES_SPI_H_

#include "stdint.h"

//Initialize SPI port for communication w/ Nordic chip
void SPI_init();

//read a single byte over the SPI interface
uint8_t SPI_read_byte();

//write a single byte over the SPI interface
void SPI_write_byte(uint8_t byte);

//send a packet of bytes over the SPI interface
void SPI_send_packet(uint8_t* p, uint8_t length);

// Blocks until SPI transmit buffer has completed transmitting
void SPI_flush();



#endif /* SOURCES_SPI_H_ */
