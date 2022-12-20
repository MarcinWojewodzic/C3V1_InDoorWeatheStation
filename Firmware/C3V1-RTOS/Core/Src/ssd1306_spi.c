/*
 * ssd1306_spi.c
 *
 *  Created on: Mar 18, 2022
 *      Author: Marcin
 */
#include "ssd1306_spi.h"
#include "main.h"
#include "string.h"
SPI_HandleTypeDef *ssd1306_spi;
static uint8_t buffer[(SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT) / 8];
static void ssd1306_command(uint8_t command)
{
   HAL_GPIO_WritePin(ssd1306_dc_GPIO_Port, ssd1306_dc_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_RESET);
   HAL_SPI_Transmit(ssd1306_spi, &command, 1, 10);
   HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_SET);
}
// static void ssd1306_data(uint8_t data)
//{
//    HAL_GPIO_WritePin(ssd1306_dc_GPIO_Port, ssd1306_dc_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(ssd1306_spi, &data, 1, 10);
//    HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_SET);
// }
void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t color)
{
   if((x < 0) || (x >= SSD1306_LCDWIDTH) || (y < 0) || (y >= SSD1306_LCDHEIGHT))
   {
      return;
   }
   if(color != 0 && color != 1)
   {
      return;
   }
   if(color == 0)
   {
      buffer[x + (y / 8) * SSD1306_LCDWIDTH] &= ~(1 << (y & 7));
   }
   else
   {
      buffer[x + (y / 8) * SSD1306_LCDWIDTH] |= (1 << (y & 7));
   }
}
void ssd1306_display()
{
   ssd1306_command(0x22);
   ssd1306_command(0x00);
   ssd1306_command(0x07);
   HAL_GPIO_WritePin(ssd1306_dc_GPIO_Port, ssd1306_dc_Pin, GPIO_PIN_SET);
   HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_RESET);
   HAL_SPI_Transmit(ssd1306_spi, (uint8_t *)&buffer, (SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8), 100);
   HAL_GPIO_WritePin(ssd1306_cs_GPIO_Port, ssd1306_cs_Pin, GPIO_PIN_SET);
}
void ssd1306_clear()
{
   memset(buffer, 0x00, (SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8));
}
void ssd1306_init(SPI_HandleTypeDef *spi)
{
   HAL_GPIO_WritePin(ssd1306_res_GPIO_Port, ssd1306_res_Pin, GPIO_PIN_RESET);
   HAL_Delay(150);
   HAL_GPIO_WritePin(ssd1306_res_GPIO_Port, ssd1306_res_Pin, GPIO_PIN_SET);
   HAL_Delay(100);
   ssd1306_spi = spi;
   ssd1306_command(0xAE); // Display Off

   ssd1306_command(0x00);
   ssd1306_command(0x10);
   ssd1306_command(0x40);
   ssd1306_command(0x20); // Set addressing mode
   ssd1306_command(0x00); // Horizontal Addressing Mode

   ssd1306_command(0x81); // Set Contrast Control
   ssd1306_command(0xff);
   ssd1306_command(0xA0 | (0x01 & 1));
   ssd1306_command(0xC0 | (0x08 & (1 << 3)));

   ssd1306_command(0xA6); // Set Normal Display

   ssd1306_command(0xA8); // Select Multiplex Ratio
   ssd1306_command(0x3F); // Default => 0x3F (1/64 Duty)	0x1F(1/32 Duty)

   ssd1306_command(0xD3); // Setting Display Offset
   ssd1306_command(0x00); // 00H Reset

   ssd1306_command(0xD5); // SET DISPLAY CLOCK
   ssd1306_command(0x80); // 105HZ

   ssd1306_command(0xD9); // Set Pre-Charge period
   ssd1306_command(0x22);

   ssd1306_command(0xDA); // Set COM Hardware Configuration
   ssd1306_command(0x12); // Alternative COM Pin---See IC Spec page 34
                          // (0x02)=> A4=0;Sequential COM pin configuration;A5=0;Disable COM Left/Right remap

   ssd1306_command(0xDB); // Set Deselect Vcomh level
   ssd1306_command(0x40);

   ssd1306_command(0x8D); // Set Charge Pump
   // ssd1306_command(0x10);  // Disable Charge Pump
   ssd1306_command(0x14); // Endable Charge Pump

   ssd1306_command(0xA4); // Entire Display ON
   ssd1306_command(SSD1306_DISPLAYON);
}
