/*
 * nordic.h
 *
 *  Created on: Dec 2, 2017
 *      Author: roda8190
 */

#ifndef SOURCES_NORDIC_H_
#define SOURCES_NORDIC_H_

#include "MKL25Z4.h"

#define NRF_CHIP_ENABLE (PTC_BASE_PTR->PCOR |= 1<<9) //set CS pin low to enable SPI transfer
#define NRF_CHIP_DISABLE (PTC_BASE_PTR->PSOR |= 1<<9) //set CS pin to high to end SPI transfer


//nordic read and write commands
#define NORDIC_WR_CMD (0x20)
#define NORDIC_RD_CMD (0x00)
#define NORDIC_FLUSH_TX_CMD (0xE1)
#define NORDIC_FLUSH_RX_CMD (0xE2)


//nordic register addresses
#define NORDIC_CONFIG_REG (0x00)
#define NORDIC_RF_CH_REG (0x05)
#define NORDIC_TX_ADDR_REG (0x10)
#define NORDIC_STATUS_REG (0x07)
#define NORDIC_RF_SETUP_REG (0x06)
#define NORDIC_FIFO_STATUS_REG (0x17)

//nordic maskes

#define NORDIC_POWER_UP (1)
#define NORDIC_POWER_DOWN (0)
#define NORDIC_NOP (0xFF)
#define NORDIC_POWER_UP_MASK (0x02)



//reads a register in the nordic chip and returns the value stored in the register
uint8_t nrf_read_register(uint8_t reg_address);

//writes to a register in the nordic chip
void nrf_write_register(uint8_t reg_address, uint8_t value);

//reads the status register of the nordic chip
uint8_t nrf_read_status();

//writes to the config register of the nordic chip
void nrf_write_config(uint8_t config);

//reads the configuration register of the nordic chip
uint8_t nrf_read_config();

//reads the rf setup register of the nordic chip
uint8_t nrf_read_rf_setup();

//writes to the rf setup register of the nordic chip
void nrf_write_rf_setup(uint8_t config);

//reads the rf CH register of the nordic chip
uint8_t nrf_read_rf_ch();

//write to the rf CH register of the nordic chip
void nrf_write_rf_ch(uint8_t channel);

//reads the 5 bytes of the TX Address register
void nrf_read_TX_ADDR(uint8_t * address);

//writes the 5 bytes of the TX Address register
void nrf_write_TX_ADDR(uint8_t * tx_addr);

//reads the fifo status register of the nordic chip
uint8_t nrf_read_fifo_status();

//sends the command flush TX to the nordic chip
void nrf_flush_tx_fifo();

//sends the command flush RX to the nordic chip
void nrf_flush_rx_fifo();




#endif /* SOURCES_NORDIC_H_ */
