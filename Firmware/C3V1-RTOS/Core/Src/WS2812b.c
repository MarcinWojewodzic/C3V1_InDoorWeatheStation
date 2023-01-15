/*
 * WS2812b.c
 *
 *  Created on: Jan 15, 2023
 *      Author: M
 */

#include "main.h"

#include "WS2812b.h"
#include "tim.h"
uint16_t WsBuffer[11 * 24] = { 0 };
void WS2812_SetPixel(uint8_t r, uint8_t g, uint8_t b)
{

   for(int8_t k = 7, i = 0; k >= 0; k--, i++)
   {

      if((r & (1 << k)) == 0)
         WsBuffer[10 * 24 + i] = zero;
      else
         WsBuffer[10 * 24 + i] = one;
   }
   for(int8_t k = 7, i = 0; k >= 0; k--, i++)
   {

      if((g & (1 << k)) == 0)
         WsBuffer[10 * 24 + i + 8] = zero;
      else
         WsBuffer[10 * 24 + i + 8] = one;
   }
   for(int8_t k = 7, i = 0; k >= 0; k--, i++)
   {

      if((b & (1 << k)) == 0)
         WsBuffer[10 * 24 + i + 16] = zero;
      else
         WsBuffer[10 * 24 + i + 16] = one;
   }
   for(int i = 0; i < 240; i++)
   {
      WsBuffer[i] = 0;
   }
   HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, WsBuffer, 11 * 24);
   while(HAL_DMA_STATE_READY != HAL_DMA_GetState(htim1.hdma[1]))
   {
   }
   HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
}
