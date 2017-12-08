/*
 * nordic.c
 *
 *  Created on: Dec 2, 2017
 *      Author: roda8190
 */

#include "nordic.h"
#include <stdint.h>
#include "SPI.h"

uint8_t nrf_read_register(uint8_t reg_address)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t return_byte =0;

	reg_address &= 0x1F; //mask out lower 5 bits of register address parameter
	cmd_byte = NORDIC_RD_CMD | reg_address;

	NRF_CHIP_ENABLE;


	SPI_write_byte(cmd_byte);


	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the specified register

	return_byte = SPI_read_byte();  // returns the contents of the specified register
	return_byte = SPI_read_byte();  // returns the contents of the specified register

	NRF_CHIP_DISABLE;

	return return_byte;


}

void nrf_write_register(uint8_t reg_address, uint8_t value)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t dummy =0;

	reg_address &= 0x1F; //mask out lower 5 bits of register address parameter

	cmd_byte = NORDIC_WR_CMD | reg_address;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();

	SPI_write_byte(value);

	dummy = SPI_read_byte(); // read garbage data out of SPI data buffer after a value has been written to NRF register

	NRF_CHIP_DISABLE;




}

uint8_t nrf_read_status()
{
	uint8_t return_byte =0;

	NRF_CHIP_ENABLE;

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the status register

	return_byte =SPI_read_byte(); // return status register byte which should have been shifted into SPI data register

	NRF_CHIP_DISABLE;

	return return_byte;

}

void nrf_write_config(uint8_t config)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t dummy =0;

	cmd_byte = NORDIC_WR_CMD | NORDIC_CONFIG_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();

	SPI_write_byte(config);

	dummy = SPI_read_byte(); // read garbage data out of SPI data buffer after a value has been written to NRF config register

	NRF_CHIP_DISABLE;


}

uint8_t nrf_read_config()
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t return_byte =0;

	cmd_byte = NORDIC_RD_CMD | NORDIC_CONFIG_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the config register

	return_byte = SPI_read_byte();  // returns the contents of the config register
	return_byte = SPI_read_byte();  // returns the contents of the config register

	NRF_CHIP_DISABLE;

	return return_byte;

}

uint8_t nrf_read_rf_setup()
{


	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t return_byte =0;

	cmd_byte = NORDIC_RD_CMD | NORDIC_RF_SETUP_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the rf_setup register

	return_byte = SPI_read_byte();  // returns the contents of the rf_setup register
	return_byte = SPI_read_byte();  // returns the contents of the rf_setup register

	NRF_CHIP_DISABLE;

	return return_byte;

}

void nrf_write_rf_setup(uint8_t config)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t dummy =0;

	cmd_byte = NORDIC_WR_CMD | NORDIC_RF_SETUP_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();

	SPI_write_byte(config);

	dummy = SPI_read_byte(); // read garbage data out of SPI data buffer after a value has been written to NRF rf_setup register

	NRF_CHIP_DISABLE;



}

uint8_t nrf_read_rf_ch()
{
	uint32_t i;
	uint32_t j;
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t return_byte =0;

	SPI0->S &= ~(0x80);

	cmd_byte = NORDIC_RD_CMD | NORDIC_RF_CH_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the rf_ch register

	return_byte = SPI_read_byte();  // returns the contents of the rf_ch register

	return_byte = SPI_read_byte();  // returns the contents of the rf_ch register

	NRF_CHIP_DISABLE;

	return return_byte;

}

void nrf_write_rf_ch(uint8_t channel)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t dummy =0;

	cmd_byte = NORDIC_WR_CMD | NORDIC_RF_CH_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();

	SPI_write_byte(channel);

	dummy = SPI_read_byte(); // read garbage data out of SPI data buffer after a value has been written to NRF rf_ch register

	NRF_CHIP_DISABLE;


}

void nrf_read_TX_ADDR(uint8_t * address)
{
	uint8_t read_array[5]={0};
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;

	cmd_byte = NORDIC_RD_CMD | NORDIC_TX_ADDR_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	//read all 5 bytes of TX_ADDR register
	for (int i=0; i<5; i++)
	{
		SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out a single byte of the tx_address  register

		*(address+i) = SPI_read_byte();  // read a single byte of the tx_address register and store it in the address array
		*(address+i) = SPI_read_byte();  // read a single byte of the tx_address register and store it in the address array
		read_array[i] = *(address+i);

	}

	NRF_CHIP_DISABLE;



}

void nrf_write_TX_ADDR(uint8_t * tx_addr)
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t dummy =0;

	cmd_byte = NORDIC_WR_CMD | NORDIC_TX_ADDR_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted


	//write all 5 bytes of the TX_ADDR register
	for (int i=0; i<5; i++)
	{
		SPI_write_byte(*(tx_addr+i));  // writes a single byte to the tx_addr register

		dummy = SPI_read_byte();  // read garbage data out of SPI data buffer after a value has been written to NRF tx_addr register

	}

	NRF_CHIP_DISABLE;


}

uint8_t nrf_read_fifo_status()
{
	uint8_t cmd_byte =0;
	uint8_t nrf_status =0;
	uint8_t return_byte =0;

	cmd_byte = NORDIC_RD_CMD | NORDIC_FIFO_STATUS_REG;

	NRF_CHIP_ENABLE;

	SPI_write_byte(cmd_byte);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	SPI_write_byte(NORDIC_NOP);  // sends NOP in order to shift out contents of the rf_ch register

	return_byte = SPI_read_byte();  // returns the contents of the fifo status register
	return_byte = SPI_read_byte();  // returns the contents of the fifo status register

	NRF_CHIP_DISABLE;

	return return_byte;

}

void nrf_flush_tx_fifo()
{
	uint8_t nrf_status =0;

	NRF_CHIP_ENABLE;

	SPI_write_byte(NORDIC_FLUSH_TX_CMD);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	NRF_CHIP_DISABLE;

}

void nrf_flush_rx_fifo()
{
	uint8_t nrf_status =0;

	NRF_CHIP_ENABLE;

	SPI_write_byte(NORDIC_FLUSH_RX_CMD);

	nrf_status = SPI_read_byte();  // status register bytes are shifted out when a command is transmitted

	NRF_CHIP_DISABLE;


}

