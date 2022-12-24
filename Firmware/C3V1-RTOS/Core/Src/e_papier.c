/*
 * e_papier.c
 *
 *  Created on: Apr 5, 2022
 *      Author: Marcin
 */
#include "main.h"

#include "e_papier.h"

#include "spi.h"

#include "string.h"

#include "cmsis_os.h"
SPI_HandleTypeDef *e_papier_spi;
static uint8_t BufferEpapier[EPD_WIDTH * EPD_HEIGHT / 8];

const unsigned char lut_vcom0[] = {
   0x00, 0x17, 0x00, 0x00, 0x00, 0x02, 0x00, 0x17, 0x17, 0x00, 0x00, 0x02, 0x00, 0x0A, 0x01, 0x00, 0x00, 0x01, 0x00, 0x0E, 0x0E, 0x00,
   0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_ww[] = {
   0x40, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x40, 0x0A, 0x01, 0x00, 0x00, 0x01, 0xA0, 0x0E, 0x0E,
   0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_bw[] = {
   0x40, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x40, 0x0A, 0x01, 0x00, 0x00, 0x01, 0xA0, 0x0E, 0x0E,
   0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_wb[] = {
   0x80, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x80, 0x0A, 0x01, 0x00, 0x00, 0x01, 0x50, 0x0E, 0x0E,
   0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_bb[] = {
   0x80, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x80, 0x0A, 0x01, 0x00, 0x00, 0x01, 0x50, 0x0E, 0x0E,
   0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
void e_papier_reset(void)
{
   HAL_GPIO_WritePin(EPAPIER_RST_GPIO_Port, EPAPIER_RST_Pin, 1);
   HAL_Delay(200);
   HAL_GPIO_WritePin(EPAPIER_RST_GPIO_Port, EPAPIER_RST_Pin, 0);
   HAL_Delay(200);
   HAL_GPIO_WritePin(EPAPIER_RST_GPIO_Port, EPAPIER_RST_Pin, 1);
   HAL_Delay(200);
}
void e_papier_send_command(uint8_t command)
{
   HAL_GPIO_WritePin(SCREENS_DC_GPIO_Port, SCREENS_DC_Pin, 0);
   HAL_GPIO_WritePin(EPAPIER_CS_GPIO_Port, EPAPIER_CS_Pin, 0);
   HAL_SPI_Transmit(e_papier_spi, &command, 1, 1000);
   HAL_GPIO_WritePin(EPAPIER_CS_GPIO_Port, EPAPIER_CS_Pin, 1);
}
void e_papier_send_data(uint8_t data)
{
   HAL_GPIO_WritePin(SCREENS_DC_GPIO_Port, SCREENS_DC_Pin, 1);
   HAL_GPIO_WritePin(EPAPIER_CS_GPIO_Port, EPAPIER_CS_Pin, 0);
   HAL_SPI_Transmit(e_papier_spi, &data, 1, 1000);
   HAL_GPIO_WritePin(EPAPIER_CS_GPIO_Port, EPAPIER_CS_Pin, 1);
}
void e_papier_wait_until_idle(void)
{
   while(HAL_GPIO_ReadPin(EPAPIER_BUSY_GPIO_Port, EPAPIER_BUSY_Pin) == 0)
   {
      osDelay(100);
   }
}
void e_papier_set_pixel(uint16_t x, uint16_t y, color_t color)
{
   if(x >= EPD_WIDTH || x < 0 || y >= EPD_HEIGHT || y < 0)
   {
      return;
   }
   if(color == WHITE)
   {
      BufferEpapier[x / 8 + y * EPD_WIDTH / 8] |= (0x80 > (x % 8));
   }
   else
   {
      BufferEpapier[x / 8 + y * EPD_WIDTH / 8] &= ~(0x80 >> (x % 8));
   }
}
void e_papier_set_lut(void)
{
   uint16_t count;
   e_papier_send_command(LUT_FOR_VCOM); // g vcom
   for(count = 0; count < 44; count++)
   {
      e_papier_send_data(lut_vcom0[count]);
   }

   e_papier_send_command(LUT_WHITE_TO_WHITE);
   for(count = 0; count < 42; count++)
   {
      e_papier_send_data(lut_ww[count]);
   }

   e_papier_send_command(LUT_BLACK_TO_WHITE);
   for(count = 0; count < 42; count++)
   {
      e_papier_send_data(lut_bw[count]);
   }

   e_papier_send_command(LUT_WHITE_TO_BLACK);
   for(count = 0; count < 42; count++)
   {
      e_papier_send_data(lut_wb[count]);
   }

   e_papier_send_command(LUT_BLACK_TO_BLACK);
   for(count = 0; count < 42; count++)
   {
      e_papier_send_data(lut_bb[count]);
   }
}
void e_papier_clear2(void)
{
   uint16_t Width, Height;
   Width  = EPD_WIDTH / 8;
   Height = EPD_HEIGHT;

   e_papier_send_command(DATA_START_TRANSMISSION_1);
   for(uint16_t j = 0; j < Height; j++)
   {
      for(uint16_t i = 0; i < Width; i++)
      {
         e_papier_send_data(0xFF);
      }
   }
   e_papier_send_command(DATA_START_TRANSMISSION_2);
   for(uint16_t j = 0; j < Height; j++)
   {
      for(uint16_t i = 0; i < Width; i++)
      {
         e_papier_send_data(0xFF);
      }
   }
   // e_papier_wait_until_idle();
}
void e_papier_turn_on_display(void)
{
   e_papier_send_command(DISPLAY_REFRESH);
   e_papier_wait_until_idle();
}
void e_papier_display(void)
{
   uint16_t Width, Height;
   Width  = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
   Height = EPD_HEIGHT;
   taskENTER_CRITICAL();
   e_papier_send_command(DATA_START_TRANSMISSION_1);
   for(uint16_t j = 0; j < Height; j++)
   {
      for(uint16_t i = 0; i < Width; i++)
      {
         e_papier_send_data(0XFF);
      }
   }
   e_papier_send_command(DATA_START_TRANSMISSION_2);
   for(uint16_t j = 0; j < Height; j++)
   {
      for(uint16_t i = 0; i < Width; i++)
      {
         e_papier_send_data(BufferEpapier[i + j * Width]);
      }
   }
   taskEXIT_CRITICAL();
   e_papier_turn_on_display();
   e_papier_wait_until_idle();
}
void e_papier_clear()
{
   memset(BufferEpapier, 0xff, EPD_HEIGHT * EPD_WIDTH / 8);
}
void e_papier_init(SPI_HandleTypeDef *spi)
{
   e_papier_spi = spi;
   e_papier_reset();

   e_papier_send_command(POWER_SETTING); // POWER SETTING
   e_papier_send_data(0x03);
   e_papier_send_data(0x00);
   e_papier_send_data(0x2b);
   e_papier_send_data(0x2b);

   e_papier_send_command(BOOSTER_SOFT_START); // boost soft start
   e_papier_send_data(0x17);                  // A
   e_papier_send_data(0x17);                  // B
   e_papier_send_data(0x17);                  // C

   e_papier_send_command(POWER_ON);
   e_papier_wait_until_idle();

   e_papier_send_command(PANEL_SETTING); // panel setting
   e_papier_send_data(0xbf);             // KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f
   e_papier_send_data(0x0d);

   e_papier_send_command(PLL_CONTROL); // PLL setting
   e_papier_send_data(0x3C);           // 3A 100HZ   29 150Hz 39 200HZ	31 171HZ

   e_papier_send_command(0x61); // resolution setting
   e_papier_send_data(0x01);
   e_papier_send_data(0x90); // 128
   e_papier_send_data(0x01); //
   e_papier_send_data(0x2c);

   e_papier_send_command(0x82); // vcom_DC setting
   e_papier_send_data(0x28);

   e_papier_send_command(0X50); // VCOM AND DATA INTERVAL SETTING
   e_papier_send_data(0x97);    // 97white border 77black border		VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7

   e_papier_set_lut();
   e_papier_clear();
   e_papier_display();
}
