/*
 * fram.c
 *
 *  Created on: May 13, 2022
 *      Author: Marcin
 */
#include "main.h"

#include "cmsis_os.h"
#include "fram.h"
#include "gpio.h"
#include "iwdg.h"
#include "spi.h"
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
   uint8_t com = WREN;
   fram_CsLow(fram);
   HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
   fram_CsHigh(fram);
   HAL_IWDG_Refresh(&hiwdg);
   osDelay(100);
}
void fram_SetProtectedBlocs(fram_t *fram, uint8_t blocs)
{
   if(blocs > 3)
   {
      blocs = 3;
   }
   uint8_t data[2];
   data[0] = WRSR;
   data[1] = (blocs << 2);
   fram_SetWELBit(fram);
   fram_CsLow(fram);
   HAL_SPI_Transmit(fram->fram_spi, data, 2, 1000);
   fram_CsHigh(fram);
}
uint32_t fram_Read32(fram_t *fram, uint32_t Address)
{
   uint8_t Temp[4];
   fram_Read(fram, Address, Temp, 4);
   return ((Temp[0] << 24) | (Temp[1] << 16) | (Temp[2] << 8) | Temp[3]);
}
void fram_Write32(fram_t *fram, uint32_t Address, uint32_t Data)
{
   uint8_t Temp[4];
   Temp[0] = ((Data >> 24) & 0xff);
   Temp[1] = ((Data >> 16) & 0xff);
   Temp[2] = ((Data >> 8) & 0xff);
   Temp[3] = ((Data >> 0) & 0xff);
   fram_Write(fram, Address, Temp, 4);
}
void fram_ChipErase(fram_t *fram)
{
   uint8_t Temp = 0;
   for(int i = 0; i < 8 * 1024; i++)
   {
      fram_Write(fram, i, &Temp, 1);
   }
}
FramStatus_TypeDef fram_Increment32(fram_t *fram, uint32_t Address)
{
   uint32_t Temp = 0, Confirm = 0;
   Temp = fram_Read32(fram, Address);
   Temp++;
   fram_Write32(fram, Address, Temp);
   Confirm = fram_Read32(fram, Address);
   if(Temp == Confirm)
   {
      return FRAM_OK;
   }
   else
   {
      return FRAM_ERROR;
   }
}
uint8_t fram_ReadSreg(fram_t *fram)
{
   fram_CsLow(fram);
   uint8_t _sreg;
   uint8_t com = RDSR;
   HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
   HAL_SPI_Receive(fram->fram_spi, &_sreg, 1, 1000);
   fram_CsHigh(fram);
   return _sreg;
}
void fram_Write(fram_t *fram, uint16_t addr, uint8_t *buff, uint16_t length)
{
   fram_SetWELBit(fram);
   uint8_t data[3];
   data[0] = WRITE;
   data[1] = (addr & 0xff) >> 8;
   data[2] = addr & 0xff;
   fram_CsLow(fram);
   HAL_SPI_Transmit(fram->fram_spi, data, 3, 1000);
   HAL_SPI_Transmit(fram->fram_spi, buff, length, 1000);
   fram_CsHigh(fram);
}
void fram_Read(fram_t *fram, uint16_t addr, uint8_t *buff, uint16_t length)
{
   fram_SetWELBit(fram);
   uint8_t data[3];
   data[0] = READ;
   data[1] = (addr & 0xff) >> 8;
   data[2] = addr & 0xff;
   fram_CsLow(fram);
   HAL_SPI_Transmit(fram->fram_spi, data, 3, 1000);
   HAL_SPI_Receive(fram->fram_spi, buff, length, 1000);
   fram_CsHigh(fram);
}
uint32_t fram_ReadID(fram_t *fram)
{
   uint8_t com, id[4];
   com = RDID;
   fram_CsLow(fram);
   HAL_SPI_Transmit(fram->fram_spi, &com, 1, 1000);
   HAL_SPI_Receive(fram->fram_spi, id, 4, 1000);
   fram_CsHigh(fram);
   return ((id[0] << 24) | (id[1] << 16) | (id[2] << 8) | id[3]);
}
void fram_Init(fram_t *fram, SPI_HandleTypeDef *spi, GPIO_TypeDef *_hold_port, GPIO_TypeDef *_cs_port, GPIO_TypeDef *_wp_port, uint16_t _hold_pin,
               uint16_t _cs_pin, uint16_t _wp_pin)
{
   fram->fram_spi  = spi;
   fram->hold_port = _hold_port;
   fram->hold_pin  = _hold_pin;
   fram->cs_port   = _cs_port;
   fram->cs_pin    = _cs_pin;
   fram->wp_port   = _wp_port;
   fram->wp_pin    = _wp_pin;
   HAL_GPIO_WritePin(fram->hold_port, fram->hold_pin, 1);
   HAL_GPIO_WritePin(fram->wp_port, fram->wp_pin, 1);
   fram_SetProtectedBlocs(fram, 0);
   fram->sreg   = fram_ReadSreg(fram);
   fram->framID = fram_ReadID(fram);
}
