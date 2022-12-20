/*
 * HC12.c
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */
#include "HC12.h"
#include "main.h"
#include "rfp.h"
#include "usart.h"
static RFP_TypeDef *rfp;
static uint8_t *HC12Data;
HC12Command_TypeDef ChannelCommand[] = {
   { "AT+C000", sizeof("AT+C000") - 1, "COK+C000", sizeof("COK+C000") - 1 }, { "AT+C006", sizeof("AT+C006") - 1, "COK+C006", sizeof("COK+C006") - 1 },
   { "AT+C012", sizeof("AT+C012") - 1, "COK+C012", sizeof("COK+C012") - 1 }, { "AT+C018", sizeof("AT+C018") - 1, "COK+C018", sizeof("COK+C018") - 1 },
   { "AT+C021", sizeof("AT+C021") - 1, "COK+C021", sizeof("COK+C021") - 1 }, { "AT+C030", sizeof("AT+C030") - 1, "COK+C030", sizeof("COK+C030") - 1 },
   { "AT+C036", sizeof("AT+C036") - 1, "COK+C036", sizeof("COK+C036") - 1 }, { "AT+C042", sizeof("AT+C042") - 1, "COK+C042", sizeof("COK+C042") - 1 },
   { "AT+C048", sizeof("AT+C048") - 1, "COK+C048", sizeof("COK+C048") - 1 }, { "AT+C055", sizeof("AT+C055") - 1, "COK+C055", sizeof("COK+C055") - 1 },
   { "AT+C060", sizeof("AT+C060") - 1, "COK+C060", sizeof("COK+C060") - 1 }, { "AT+C070", sizeof("AT+C070") - 1, "COK+C070", sizeof("COK+C070") - 1 },
   { "AT+C075", sizeof("AT+C075") - 1, "COK+C075", sizeof("COK+C075") - 1 }, { "AT+C080", sizeof("AT+C080") - 1, "COK+C080", sizeof("COK+C080") - 1 },
   { "AT+C090", sizeof("AT+C090") - 1, "COK+C090", sizeof("COK+C090") - 1 }, { "AT+C099", sizeof("AT+C099") - 1, "COK+C099", sizeof("COK+C099") - 1 }
};
void HC12_EnterCommandMode(void)
{
   HAL_GPIO_WritePin(HC12_SET_GPIO_Port, HC12_SET_Pin, 0);
}
void HC12_ExitCommandMode(void)
{
   HAL_GPIO_WritePin(HC12_SET_GPIO_Port, HC12_SET_Pin, 1);
}
void HC12_TransmitData(uint8_t *Data, uint32_t Length)
{
   HAL_UART_Transmit(&huart1, Data, Length, 1000);
}
void HC12_ReciveToIdleDMA(uint8_t *Data, uint32_t Length)
{
   HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Data, Length);
}
void HC12_Init(RFP_TypeDef *TempRfp, uint8_t *Buffer)
{
   rfp      = TempRfp;
   HC12Data = Buffer;
}
void HC12_SetChannel(HC12Channel_TypeDef Channel)
{
   HC12_EnterCommandMode();
   HAL_Delay(100);
   rfp->NewDataFlag = RFP_NEW_DATA_FLAG_RESET;
   HC12_TransmitData((uint8_t *)ChannelCommand[Channel].Command, ChannelCommand[Channel].SizeOfCommand);
   uint32_t TickStart = HAL_GetTick();
   while(rfp->NewDataFlag != RFP_NEW_DATA_FLAG_SET)
   {
      if(HAL_GetTick() - TickStart > HC12_MAX_TIMEOUT)
      {
         break;
      }
   }
   HC12_ExitCommandMode();
   HAL_Delay(200);
}
