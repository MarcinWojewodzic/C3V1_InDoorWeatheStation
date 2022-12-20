/*
 * fram.c
 *
 *  Created on: May 13, 2022
 *      Author: Marcin
 */
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "fram.h"
void fram_CsLow(fram_t *fram)
{
	HAL_GPIO_WritePin(fram->cs_port, fram->cs_pin, 0);
}
void fram_CsHigh(fram_t *fram)
{
	HAL_GPIO_WritePin(fram->cs_port, fram->cs_pin, 1);
}
void fram_SetWELBit(fram_t *fram)
{
	uint8_t com=WREN;
	fram_CsLow(fram);
	HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
	fram_CsHigh(fram);
}
void fram_SetProtectedBlocs(fram_t *fram,uint8_t blocs)
{
	if(blocs>3)
	{
		blocs=3;
	}
	uint8_t data[2];
	data[0]=WRSR;
	data[1]=(blocs<<2);
	fram_SetWELBit(fram);
	fram_CsLow(fram);
	HAL_SPI_Transmit(fram->fram_spi, data, 2, 1000);
	fram_CsHigh(fram);
}
uint8_t fram_ReadSreg(fram_t *fram)
{
	fram_CsLow(fram);
	uint8_t _sreg;
	uint8_t com=RDSR;
	HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
	HAL_SPI_Receive(fram->fram_spi, &_sreg, 1, 1000);
	fram_CsHigh(fram);
	return _sreg;
}
void fram_Write(fram_t *fram,uint16_t addr,uint8_t *buff, uint16_t length)
{
	fram_SetWELBit(fram);
	uint8_t data[3];
	data[0]=WRITE;
	data[1]=(addr&0xf0)>>8;
	data[2]=addr&0x0f;
	fram_CsLow(fram);
	HAL_SPI_Transmit(fram->fram_spi, data, 3, 1000);
	HAL_SPI_Transmit(fram->fram_spi, buff, length, 1000);
	fram_CsHigh(fram);
}
void fram_Read(fram_t *fram,uint16_t addr, uint8_t *buff, uint16_t length)
{
	fram_SetWELBit(fram);
	uint8_t data[3];
	data[0]=READ;
	data[1]=(addr&0xf0)>>8;
	data[2]=addr&0x0f;
	fram_CsLow(fram);
	HAL_SPI_Transmit(fram->fram_spi, data, 3, 1000);
	HAL_SPI_Receive(fram->fram_spi, buff, length, 1000);
	fram_CsHigh(fram);
}
uint32_t fram_ReadID(fram_t *fram)
{
	uint8_t com,id[4];
	com=RDID;
	fram_CsLow(fram);
	HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
	HAL_SPI_Receive(fram->fram_spi, id, 4, 1000);
	fram_CsHigh(fram);
	return ((id[0]<<24)|(id[1]<<16)|(id[2]<<8)|id[3]);
}
void fram_Init(fram_t *fram,SPI_HandleTypeDef *spi,GPIO_TypeDef *_hold_port,GPIO_TypeDef *_cs_port,GPIO_TypeDef *_wp_port,uint16_t _hold_pin,uint16_t _cs_pin,uint16_t _wp_pin)
{
	fram->fram_spi=spi;
	fram->hold_port=_hold_port;
	fram->hold_pin=_hold_pin;
	fram->cs_port=_cs_port;
	fram->cs_pin=_cs_pin;
	fram->wp_port=_wp_port;
	fram->wp_pin=_wp_pin;
	HAL_GPIO_WritePin(fram->hold_port, fram->hold_pin, 1);
	HAL_GPIO_WritePin(fram->wp_port, fram->wp_pin, 1);
	fram_SetProtectedBlocs(fram, 0);
	fram->sreg=fram_ReadSreg(fram);
	fram->framID=fram_ReadID(fram);
}

