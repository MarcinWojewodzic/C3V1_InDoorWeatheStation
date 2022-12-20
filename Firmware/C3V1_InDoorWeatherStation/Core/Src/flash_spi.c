/*
 * flash_spi.c
 *
 *  Created on: May 14, 2022
 *      Author: Marcin
 */
#include"main.h"
#include"flash_spi.h"
void flash_CsLow(flash_t *flash)
{
	HAL_GPIO_WritePin(flash->flash_CsPort, flash->flash_CsPin, 0);
}
void flash_CsHigh(flash_t *flash)
{
	HAL_GPIO_WritePin(flash->flash_CsPort, flash->flash_CsPin, 1);
}
void flash_WriteCommand(flash_t *flash,uint8_t *command,uint8_t size)
{
	HAL_SPI_Transmit(flash->flash_spi, command, size, 1000);
}
void flash_ReadCommand(flash_t *flash,uint8_t *command,uint8_t size)
{
	HAL_SPI_Receive(flash->flash_spi, command, size, 1000);
}
void flash_WriteData(flash_t *flash,uint8_t *data,uint32_t size)
{
	HAL_SPI_Transmit(flash->flash_spi, data, size, 1000);
}
void flash_ReadData(flash_t *flash,uint8_t *data,uint32_t size)
{
	HAL_SPI_Receive(flash->flash_spi, data, size, 1000);
}
uint8_t flash_ReadSR1(flash_t *flash)
{
	flash_CsLow(flash);
	uint8_t command=READ_SR1;
	flash_WriteCommand(flash, &command, 1);
	flash_ReadCommand(flash, &command, 1);
	flash_CsHigh(flash);
	return command;
}
uint8_t flash_ReadSR2(flash_t *flash)
{
	flash_CsLow(flash);
	uint8_t command=READ_SR2;
	flash_WriteCommand(flash, &command, 1);
	flash_ReadCommand(flash, &command, 1);
	flash_CsHigh(flash);
	return command;
}
void flash_Reset(flash_t *flash)
{
	uint8_t command=ENABLE_RESET;
	flash_CsLow(flash);
	flash_WriteCommand(flash, &command, 1);
	flash_CsHigh(flash);
	command=RESET;
	flash_CsLow(flash);
	flash_WriteCommand(flash, &command, 1);
	flash_CsHigh(flash);

}
void flash_SetWELBit(flash_t *flash)
{
	flash_CsLow(flash);
	uint8_t command=WRITE_ENABLE;
	flash_WriteCommand(flash, &command, 1);
	flash_CsHigh(flash);
}
void flash_ReadID(flash_t *flash)
{
	uint8_t id[3],command=JEDEC_ID;
	flash_CsLow(flash);
	flash_WriteCommand(flash, &command, 1);
	flash_ReadCommand(flash, id, 3);
	flash_CsHigh(flash);
	flash->id=((id[0]<<16)|(id[1]<<8)|id[2]);
}
void flash_WaitForEndProcess(flash_t *flash)
{
	while((flash_ReadSR1(flash)&BUSY_FLAG)){}
}
void flash_SetWELBit_and_WaitForEndProcess(flash_t *flash)
{
	while(!(flash_ReadSR1(flash)&WEL_BIT))
	{
		flash_SetWELBit(flash);
	}
}
void flash_ReadDataBytes(flash_t *flash,uint32_t addr,uint8_t *data,uint16_t size)
{
	uint8_t dataToSend[4];
	flash_WaitForEndProcess(flash);
	flash_SetWELBit_and_WaitForEndProcess(flash);
	dataToSend[0] = READ_DATA;
	dataToSend[1] = (addr >> 16) & 0xff;
	dataToSend[2] = (addr >> 8) & 0xff;
	dataToSend[3] = addr & 0xff;
	flash_CsLow(flash);
	flash_WriteCommand(flash, dataToSend, 4);
	flash_ReadData(flash, data, size);
	flash_CsHigh(flash);
	flash_WaitForEndProcess(flash);
	flash_WaitForEndProcess(flash);
}
void flash_WritePage(flash_t *flash,uint32_t page_addr,uint8_t *data,uint16_t size)
{
	uint8_t dataToSend[4];
	flash_WaitForEndProcess(flash);
	flash_SetWELBit_and_WaitForEndProcess(flash);
	dataToSend[0] = PAGE_PROGRAM;
	dataToSend[1] = (page_addr >> 16) & 0xff;
	dataToSend[2] = (page_addr >> 8) & 0xff;
	dataToSend[3] = page_addr & 0xff;
	flash_CsLow(flash);
	flash_WriteCommand(flash, dataToSend, 4);
	flash_WriteData(flash, data, size);
	flash_CsHigh(flash);
	flash_WaitForEndProcess(flash);
}
void flash_ClearSector(flash_t *flash,uint16_t addr)
{
	uint32_t address=addr*4096;
	uint8_t dataToSend[4];
	dataToSend[0]=SECTOR_ERASE;
	dataToSend[1]=(address>>16)&0xff;
	dataToSend[2]=(address>>8)&0xff;
	dataToSend[3]=(address)&0xff;
	flash_WaitForEndProcess(flash);
	flash_SetWELBit_and_WaitForEndProcess(flash);
	flash_CsLow(flash);
	flash_WriteCommand(flash, dataToSend, 4);
	flash_CsHigh(flash);
	flash_WaitForEndProcess(flash);
}
void flash_Init(flash_t *flash,SPI_HandleTypeDef *spi, GPIO_TypeDef* cs_port,uint16_t cs_pin)
{
	flash->flash_spi=spi;
	flash->flash_CsPort=cs_port;
	flash->flash_CsPin=cs_pin;
	flash_Reset(flash);
	flash_ReadID(flash);
}
