/*
 * ssd1306_spi.h
 *
 *  Created on: Mar 18, 2022
 *      Author: Marcin
 */

#ifndef INC_SSD1306_SPI_H_
#define INC_SSD1306_SPI_H_
#include "main.h"
#define ssd1306_dc_GPIO_Port  SCREENS_DC_GPIO_Port
#define ssd1306_dc_Pin        SCREENS_DC_Pin
#define ssd1306_cs_GPIO_Port  SSD1306_CS_GPIO_Port
#define ssd1306_cs_Pin        SSD1306_CS_Pin
#define ssd1306_res_GPIO_Port SSD1306_RST_GPIO_Port
#define ssd1306_res_Pin       SSD1306_RST_Pin
//
// Resolution
//
#define SSD1306_LCDWIDTH  128
#define SSD1306_LCDHEIGHT 64
//
// Commands
//
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_EXTERNALVCC         0x1
#define SSD1306_SWITCHCAPVCC        0x2
//
// Colors
//
#define INVERSE 2
void ssd1306_init(SPI_HandleTypeDef *spi);
void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_display();
void ssd1306_clear();
#endif /* INC_SSD1306_SPI_H_ */
