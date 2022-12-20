/*
 * HC12.h
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */

#ifndef INC_HC12_H_
#define INC_HC12_H_
#include "main.h"
#include "rfp.h"
#define HC12_MAX_TIMEOUT 1000
typedef enum
{
   CH0 = 0,
   CH6,
   CH12,
   CH18,
   CH21,
   CH30,
   CH36,
   CH42,
   CH48,
   CH55,
   CH60,
   CH70,
   CH75,
   CH80,
   CH90,
   CH99
} HC12Channel_TypeDef;
typedef struct
{
   char *Command;
   uint32_t SizeOfCommand;
   char *Response;
   uint32_t SizeOfResponse;
} HC12Command_TypeDef;
void HC12_EnterCommandMode(void);
void HC12_ExitCommandMode(void);
void HC12_TransmitData(uint8_t *Data, uint32_t Length);
void HC12_ReciveToIdleDMA(uint8_t *Data, uint32_t Length);
void HC12_Init(RFP_TypeDef *TempRfp, uint8_t *Buffer);
void HC12_SetChannel(HC12Channel_TypeDef Channel);
#endif /* INC_HC12_H_ */
