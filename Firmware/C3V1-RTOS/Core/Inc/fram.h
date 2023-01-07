/*
 * fram.h
 *
 *  Created on: May 13, 2022
 *      Author: Marcin
 */

#ifndef INC_FRAM_H_
#define INC_FRAM_H_
#define RDSR  0b00000101
#define WREN  0x06
#define RDID  0x9f
#define WRITE 0x02
#define READ  0x03
#define WRSR  0x01
typedef enum
{
   FRAM_ERROR,
   FRAM_OK
} FramStatus_TypeDef;
typedef struct
{
   SPI_HandleTypeDef *fram_spi;
   GPIO_TypeDef *hold_port, *cs_port, *wp_port;
   uint16_t hold_pin, cs_pin, wp_pin;
   uint8_t sreg;
   uint32_t framID;
} fram_t;
void fram_Init(fram_t *fram, SPI_HandleTypeDef *spi, GPIO_TypeDef *_hold_port, GPIO_TypeDef *_cs_port, GPIO_TypeDef *_wp_port, uint16_t _hold_pin,
               uint16_t _cs_pin, uint16_t _wp_pin);
void fram_Write(fram_t *fram, uint16_t addr, uint8_t *buff, uint16_t length);
void fram_Read(fram_t *fram, uint16_t addr, uint8_t *buff, uint16_t length);
uint32_t fram_Read32(fram_t *fram, uint32_t Address);
void fram_Write32(fram_t *fram, uint32_t Address, uint32_t Data);
FramStatus_TypeDef fram_Increment32(fram_t *fram, uint32_t Address);
#endif /* INC_FRAM_H_ */
