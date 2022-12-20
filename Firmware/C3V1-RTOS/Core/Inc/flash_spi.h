/*
 * flash_spi.h
 *
 *  Created on: May 14, 2022
 *      Author: Marcin
 */

#ifndef INC_FLASH_SPI_H_
#define INC_FLASH_SPI_H_
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define READ_STATUS_REG1 0x05
#define READ_STATUS_REG2 0x35
#define WRITE_STATUS_REG 0x01
#define READ_DATA 0x03
#define PAGE_PROGRAM 0x02
#define SECTOR_ERASE 0x20
#define CHIP_ERASE 0xC7
#define ENABLE_RESET 0x66
#define RESET 0x99
#define JEDEC_ID 0x9f
#define READ_SR1 0x05
#define READ_SR2 0x35
#define BUSY_FLAG 0x01
#define WEL_BIT 0x02
typedef struct
{
	SPI_HandleTypeDef *flash_spi;
	uint32_t id;
	uint16_t flash_CsPin;
	GPIO_TypeDef *flash_CsPort;
} flash_t;
void flash_Init(flash_t *flash,SPI_HandleTypeDef *spi, GPIO_TypeDef* cs_port,uint16_t cs_pin);
void flash_ReadDataBytes(flash_t *flash,uint32_t addr,uint8_t *data,uint16_t size);
void flash_WritePage(flash_t *flash,uint32_t page_addr,uint8_t *data,uint16_t size);
void flash_ClearSector(flash_t *flash,uint16_t addr);
#endif /* INC_FLASH_SPI_H_ */
