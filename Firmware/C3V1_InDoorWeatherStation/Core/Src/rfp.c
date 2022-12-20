/*
 * rfp.c
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */

#include "rfp.h"

#include "HC12.h"
#include "crc.h"
#include "main.h"
#include "stdio.h"
uint8_t RfpDataRecive[100]   = { 0 };
uint8_t RfpDataTransmit[100] = { 0 };
static RFP_TypeDef *Rfp;
static void RFP_ChangeState(void);
static void RFP_InitializeFunction(void);
static void RFP_IdleFunction(void);
static void RFP_ParseFunction(void);
static void RFP_RunningFunction(void);
static void RFP_SendFunction(void);
static void RFP_WaitForResponseFunction(void);
static void RFP_ParseResponseFunction(void);
static void RFP_SendResponseFunction(RFPMessageType_TypeDef Response);
void RFP_SendData(RFPDeviceID_TypeDef Destination, RFPMessageType_TypeDef Type, uint8_t *Data, uint32_t Length);
static RFPTransitionTable_TypeDef RfpTransitionTable[] = { { RFP_STATE_INITIALIZE, RFP_STATE_IDLE, RFP_EVENT_END_INITIALIZE },
                                                           { RFP_STATE_IDLE, RFP_STATE_PARSE, RFP_EVENT_NEW_DATA },
                                                           { RFP_STATE_PARSE, RFP_STATE_IDLE, RFP_EVENT_ERROR },
                                                           { RFP_STATE_PARSE, RFP_STATE_RUNNING, RFP_EVENT_DATA_OK },
                                                           { RFP_STATE_RUNNING, RFP_STATE_IDLE, RFP_EVENT_NOTHING },
                                                           { RFP_STATE_IDLE, RFP_STATE_SEND, RFP_EVENT_SEND },
                                                           { RFP_STATE_SEND, RFP_STATE_WAIT_FOR_RESPONSE, RFP_EVENT_WAIT_FOR_RESPONSE },
                                                           { RFP_STATE_WAIT_FOR_RESPONSE, RFP_STATE_SEND, RFP_EVENT_ERROR },
                                                           { RFP_STATE_WAIT_FOR_RESPONSE, RFP_STATE_PARSE_RESPONSE, RFP_EVENT_NEW_DATA },
                                                           { RFP_STATE_PARSE_RESPONSE, RFP_STATE_SEND, RFP_EVENT_ERROR },
                                                           { RFP_STATE_PARSE_RESPONSE, RFP_STATE_IDLE, RFP_EVENT_DATA_OK },
                                                           { RFP_STATE_SEND, RFP_STATE_IDLE, RFP_EVENT_ERROR } };
RFPFunctions_TypeDef RfpFunction[]
    = { { RFP_InitializeFunction },      { RFP_IdleFunction },         { RFP_ParseFunction }, { RFP_RunningFunction }, { RFP_SendFunction },
        { RFP_WaitForResponseFunction }, { RFP_ParseResponseFunction } };
RFPCommandFunctions_TypeDef RfpCommandFunction[RFP_COMMAND_CNT];
RFPDataFunctions_TypeDef RfpDataFunctions;
RFPMessageFunctions_TypeDef RfpMessageFunction[RFP_MESSAGE_CNT];
void RFP_Handle(void)
{
   RFP_ChangeState();
   if(RfpFunction[Rfp->State].RfpFunction != NULL)
   {
      RfpFunction[Rfp->State].RfpFunction();
   }
}
void RFP_Init(RFP_TypeDef *RadioProtocol, RFPDeviceID_TypeDef Device)
{
   Rfp             = RadioProtocol;
   Rfp->Initialize = RFP_INITIALIZE;
   Rfp->DeviceType = Device;
   for(int i = 0; i < RFP_COMMAND_CNT; i++)
   {
      RfpCommandFunction[i].RfpCommandFunction = NULL;
   }
   for(int i = 0; i < RFP_MESSAGE_CNT; i++)
   {
      RfpMessageFunction[i].RfpMessageFunction = NULL;
   }
   RfpDataFunctions.RfpDataFunction = NULL;
}
static void RFP_ChangeState(void)
{
   for(int i = 0; i < RFP_TRANSITION_TABLE_SIZE; i++)
   {
      if(Rfp->State == RfpTransitionTable[i].Source && Rfp->NewEvent == RfpTransitionTable[i].Event)
      {
         Rfp->State    = RfpTransitionTable[i].Destination;
         Rfp->NewEvent = RFP_EVENT_NOTHING;
         return;
      }
   }
   Rfp->NewEvent = RFP_EVENT_NOTHING;
}
void RFP_RegisterCommandFunction(RFPCommands_TypeDef Command, void (*Callback)(uint8_t *, uint32_t, uint32_t))
{
   RfpCommandFunction[Command - 1].RfpCommandFunction = Callback;
}
void RFP_RegisterDataFunction(void (*Callback)(uint8_t *, uint32_t, uint32_t))
{
   RfpDataFunctions.RfpDataFunction = Callback;
}
void RFP_RegisterMessageFunction(RFPMessage_TypeDef Message, void (*Callback)(uint8_t *, uint32_t, uint32_t))
{
   RfpMessageFunction[Message - 1].RfpMessageFunction = Callback;
}
void RFP_InterruptTask(void)
{
   Rfp->NewDataFlag = RFP_NEW_DATA_FLAG_SET;
}
void RFP_SendData(RFPDeviceID_TypeDef Destination, RFPMessageType_TypeDef Type, uint8_t *Data, uint32_t Length)
{
   RfpDataTransmit[0] = Destination;
   RfpDataTransmit[1] = Rfp->DeviceType;
   RfpDataTransmit[2] = Type;
   for(int i = 0; i < Length; i++)
   {
      RfpDataTransmit[i + 3] = Data[i];
   }
   uint32_t crc                    = Crc(CRC_INITIAL_VALUE, 3 + Length, RfpDataTransmit);
   RfpDataTransmit[3 + Length]     = ((crc >> 24) & 0xff);
   RfpDataTransmit[3 + Length + 1] = ((crc >> 16) & 0xff);
   RfpDataTransmit[3 + Length + 2] = ((crc >> 8) & 0xff);
   RfpDataTransmit[3 + Length + 3] = (crc & 0xff);
   Rfp->SendFlag                   = RFP_SEND_FLAG_SET;
   Rfp->DataSizeTransmit           = 3 + Length + 4;
}
static void RFP_SendResponseFunction(RFPMessageType_TypeDef Response)
{
   HAL_Delay(200);
   RfpDataTransmit[0] = Rfp->SourceMessage;
   RfpDataTransmit[1] = Rfp->DeviceType;
   RfpDataTransmit[2] = RFP_MESSAGE;
   RfpDataTransmit[3] = Response;
   uint32_t crc       = Crc(CRC_INITIAL_VALUE, 4, RfpDataTransmit);
   RfpDataTransmit[4] = ((crc >> 24) & 0xff);
   RfpDataTransmit[5] = ((crc >> 16) & 0xff);
   RfpDataTransmit[6] = ((crc >> 8) & 0xff);
   RfpDataTransmit[7] = (crc & 0xff);
   HC12_TransmitData(RfpDataTransmit, 8);
}
static void RFP_InitializeFunction(void)
{
   Rfp->NewEvent = RFP_EVENT_END_INITIALIZE;
   HC12_ExitCommandMode();
   HC12_Init(Rfp, RfpDataRecive);
   HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
}
static void RFP_IdleFunction(void)
{
   if(Rfp->NewDataFlag == RFP_NEW_DATA_FLAG_SET)
   {
      Rfp->NewDataFlag = RFP_NEW_DATA_FLAG_RESET;
      Rfp->NewEvent    = RFP_EVENT_NEW_DATA;
   }
   else if(Rfp->SendFlag == RFP_SEND_FLAG_SET)
   {
      Rfp->SendFlag = RFP_SEND_FLAG_RESET;
      Rfp->NewEvent = RFP_EVENT_SEND;
      HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
   }
}
static void RFP_ParseFunction(void)
{
   uint32_t MessageCRC = 0;
   Rfp->SourceMessage  = RfpDataRecive[1];
   if(RfpDataRecive[0] != Rfp->DeviceType)
   {
      Rfp->NewEvent = RFP_EVENT_ERROR;
      RFP_SendResponseFunction(RFP_ERROR);
      HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
   }
   else
   {
      if(Rfp->DataSize != 0)
      {
         MessageCRC = (((uint32_t)RfpDataRecive[Rfp->DataSize - 4] << 24) | ((uint32_t)RfpDataRecive[Rfp->DataSize - 3] << 16)
                       | ((uint32_t)RfpDataRecive[Rfp->DataSize - 2] << 8) | ((uint32_t)RfpDataRecive[Rfp->DataSize - 1]));
         if(MessageCRC == Crc(CRC_INITIAL_VALUE, Rfp->DataSize - 4, RfpDataRecive))
         {
            Rfp->NewEvent = RFP_EVENT_DATA_OK;
            RFP_SendResponseFunction(RFP_OK);
         }
         else
         {
            if(RepareMessage(MessageCRC, RfpDataRecive, Rfp->DataSize) == CRC_OK)
            {
               Rfp->NewEvent = RFP_EVENT_DATA_OK;
               RFP_SendResponseFunction(RFP_OK);
            }
            else
            {
               Rfp->NewEvent = RFP_EVENT_ERROR;
               RFP_SendResponseFunction(RFP_CRC_ERROR);
               HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
            }
         }
      }
   }
}
static void RFP_RunningFunction(void)
{
   switch(RfpDataRecive[2])
   {
      case RFP_COMMAND:
      {
         if(RfpCommandFunction[RfpDataRecive[3] - 1].RfpCommandFunction != NULL)
         {
            RfpCommandFunction[RfpDataRecive[3] - 1].RfpCommandFunction(RfpDataRecive, Rfp->DataSize, 4);
         }
         break;
      }
      case RFP_DATA:
      {
         if(RfpDataFunctions.RfpDataFunction != NULL)
         {
            RfpDataFunctions.RfpDataFunction(RfpDataRecive, Rfp->DataSize, 4);
         }
         break;
      }
      case RFP_MESSAGE:
      {
         if(RfpMessageFunction[RfpDataRecive[3] - 1].RfpMessageFunction != NULL)
         {
            RfpMessageFunction[RfpDataRecive[3] - 1].RfpMessageFunction(RfpDataRecive, Rfp->DataSize, 4);
         }
         break;
      }
      default:
      {
         break;
      }
   }
   HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
}
static void RFP_SendFunction(void)
{
   if(Rfp->Cnt < 5)
   {
      HAL_Delay(200);
      HC12_TransmitData(RfpDataTransmit, Rfp->DataSizeTransmit);
      Rfp->Cnt++;
      Rfp->NewEvent = RFP_EVENT_WAIT_FOR_RESPONSE;
      Rfp->LastTick = HAL_GetTick();
   }
   else
   {
      Rfp->Cnt      = 0;
      Rfp->NewEvent = RFP_EVENT_ERROR;
      HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
   }
}
static void RFP_WaitForResponseFunction(void)
{
   if(HAL_GetTick() - Rfp->LastTick > 2000)
   {
      Rfp->NewEvent = RFP_EVENT_ERROR;
   }
   else if(Rfp->NewDataFlag == RFP_NEW_DATA_FLAG_SET)
   {
      Rfp->NewDataFlag = RFP_NEW_DATA_FLAG_RESET;
      Rfp->NewEvent    = RFP_EVENT_NEW_DATA;
   }
}
static void RFP_ParseResponseFunction(void)
{
   uint32_t MessageCRC = (((uint32_t)RfpDataRecive[Rfp->DataSize - 4] << 24) | ((uint32_t)RfpDataRecive[Rfp->DataSize - 3] << 16)
                          | ((uint32_t)RfpDataRecive[Rfp->DataSize - 2] << 8) | ((uint32_t)RfpDataRecive[Rfp->DataSize - 1]));
   if(MessageCRC == Crc(CRC_INITIAL_VALUE, Rfp->DataSize - 4, RfpDataRecive))
   {
      if(RfpDataRecive[0] == Rfp->DeviceType && RfpDataRecive[2] == RFP_MESSAGE)
      {
         if(RfpDataRecive[3] == RFP_OK)
         {
            Rfp->NewEvent = RFP_EVENT_DATA_OK;
            Rfp->Cnt      = 0;
            HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
         }
         else
         {
            Rfp->NewEvent = RFP_EVENT_ERROR;
            HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
         }
      }
      else
      {
         Rfp->NewEvent = RFP_EVENT_ERROR;
         HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
      }
   }
   else if(RepareMessage(MessageCRC, RfpDataRecive, Rfp->DataSize) == CRC_OK)
   {
      if(RfpDataRecive[0] == Rfp->DeviceType && RfpDataRecive[2] == RFP_MESSAGE)
      {
         if(RfpDataRecive[3] == RFP_OK)
         {
            Rfp->NewEvent = RFP_EVENT_DATA_OK;
            Rfp->Cnt      = 0;
            HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
         }
         else
         {
            Rfp->NewEvent = RFP_EVENT_ERROR;
            HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
         }
      }
      else
      {
         Rfp->NewEvent = RFP_EVENT_ERROR;
         HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
      }
   }
   else
   {
      HC12_ReciveToIdleDMA(RfpDataRecive, sizeof(RfpDataRecive));
      Rfp->NewEvent = RFP_EVENT_ERROR;
   }
}
