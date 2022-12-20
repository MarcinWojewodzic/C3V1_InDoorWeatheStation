/*
 * rfp.h
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */

#ifndef INC_RFP_H_
#define INC_RFP_H_
#include "main.h"
#define RFP_TRANSITION_TABLE_SIZE sizeof(RfpTransitionTable) / sizeof(RFPTransitionTable_TypeDef)
#define RFP_COMMAND_CNT           6
#define RFP_MESSAGE_CNT           5
typedef enum
{
   RFP_BS   = 0x01,
   RFP_ODWS = 0x02,
   RFP_IDWS = 0x03
} RFPDeviceID_TypeDef;
typedef enum
{
   RFP_TEMPERATURE_AHT_15          = 0x01,
   RFP_HUMIDITI_AHT_15             = 0x02,
   RFP_TEMPERATURE_DS18B20         = 0x03,
   RFP_TEMPERATURE_INTERNAL        = 0x04,
   RFP_BATTERY_LEVEL               = 0x05,
   RFP_BRIGHTNESS                  = 0x06,
   RFP_EXTERNAL_MODULE_TEMPERATURE = 0x07,
   RFP_BATTERY_STATE               = 0x08,
   RFP_AIR_POLLUTION_PM1           = 0x09,
   RFP_AIR_POLLUTION_PM25          = 0x0a,
   RFP_AIR_POLLUTION_PM10          = 0x0b
} RFPParameters_TypeDef;
typedef enum
{
   RFP_TEMPERATURE_AHT_15_SIZE          = 0x04,
   RFP_HUMIDITI_AHT_15_SIZE             = 0x04,
   RFP_TEMPERATURE_DS18B20_SIZE         = 0x04,
   RFP_TEMPERATURE_INTERNAL_SIZE        = 0x04,
   RFP_BATTERY_LEVEL_SIZE               = 0x04,
   RFP_BRIGHTNESS_SIZE                  = 0x02,
   RFP_EXTERNAL_MODULE_TEMPERATURE_SIZE = 0x04,
   RFP_BATTERY_STATE_SIZE               = 0x01,
   RFP_AIR_POLLUTION_PM1_SIZE           = 0x02,
   RFP_AIR_POLLUTION_PM25_SIZE          = 0x02,
   RFP_AIR_POLLUTION_PM10_SIZE          = 0x02,
} RFPSizeParameters_TypeDef;
typedef enum
{
   RFP_TEST             = 0x01,
   RFP_START_MEASURMENT = 0x02,
   RFP_GO_TO_DEEP_SLEEP = 0x03,
   RFP_RESET            = 0x04,
   RFP_ENTER_DEBUG_MODE = 0x05,
   RFP_EXIT_DEBUG_MODE  = 0x06
} RFPCommands_TypeDef;
typedef enum
{
   RFP_ERROR           = 0x01,
   RFP_CRC_ERROR       = 0x02,
   RFP_LOW_BATTERY     = 0x03,
   RFP_OK              = 0x04,
   RFP_EXIT_DEEP_SLEEP = 0x05
} RFPMessage_TypeDef;
typedef enum
{
   RFP_COMMAND = 0x01,
   RFP_DATA    = 0x02,
   RFP_MESSAGE = 0x03
} RFPMessageType_TypeDef;
typedef enum
{
   RFP_ERROR_SIZE           = 0x00,
   RFP_CRC_ERROR_SIZE       = 0x00,
   RFP_LOW_BATTERY_SIZE     = 0x04,
   RFP_OK_SIZE              = 0x00,
   RFP_EXIT_DEEP_SLEEP_SIZE = 0x00

} RFPMessageDataSize_TypeDef;
typedef enum
{
   RFP_TEST_SIZE             = 0x00,
   RFP_START_MEASURMENT_SIZE = 0x00,
   RFP_GO_TO_DEEP_SLEEP_SIZE = 0x02,
   RFP_RESET_SIZE            = 0x00,
   RFP_ENTER_DEBUG_MODE_SIZE = 0x00,
   RFP_EXIT_DEBUG_MODE_SIZE  = 0x00
} RFPCommandDataSize_TypeDef;
typedef enum
{
   RFP_STATE_INITIALIZE = 0x00,
   RFP_STATE_IDLE,
   RFP_STATE_PARSE,
   RFP_STATE_RUNNING,
   RFP_STATE_SEND,
   RFP_STATE_WAIT_FOR_RESPONSE,
   RFP_STATE_PARSE_RESPONSE
} RFPState_TypeDef;
typedef enum
{
   RFP_EVENT_NOTHING = 0x00,
   RFP_EVENT_NEW_DATA,
   RFP_EVENT_END_INITIALIZE,
   RFP_EVENT_ERROR,
   RFP_EVENT_DATA_OK,
   RFP_EVENT_WAIT_FOR_RESPONSE,
   RFP_EVENT_SEND
} RFPEvent_TypeDef;
typedef enum
{
   RFP_NOT_INITIALIZE = 0x00,
   RFP_INITIALIZE
} RFPInitialize_TypeDef;
typedef enum
{
   RFP_NEW_DATA_FLAG_RESET = 0x00,
   RFP_NEW_DATA_FLAG_SET
} RFPNewDataFlag_TypeDef;
typedef enum
{
   RFP_SEND_FLAG_RESET,
   RFP_SEND_FLAG_SET
} RFPSendFlag_TypeDef;
typedef struct
{
   RFPState_TypeDef Source;
   RFPState_TypeDef Destination;
   RFPEvent_TypeDef Event;
} RFPTransitionTable_TypeDef;
typedef struct
{
   RFPState_TypeDef State;
   RFPEvent_TypeDef NewEvent;
   RFPState_TypeDef OldSate;
   RFPInitialize_TypeDef Initialize;
   volatile RFPNewDataFlag_TypeDef NewDataFlag;
   RFPDeviceID_TypeDef DeviceType;
   uint32_t DataSize;
   uint32_t DataSizeTransmit;
   RFPDeviceID_TypeDef SourceMessage;
   RFPSendFlag_TypeDef SendFlag;
   uint32_t Cnt;
   uint32_t LastTick;
} RFP_TypeDef;
typedef struct
{
   void (*RfpFunction)(void);
} RFPFunctions_TypeDef;
typedef struct
{
   void (*RfpCommandFunction)(uint8_t *, uint32_t, uint32_t);
} RFPCommandFunctions_TypeDef;
typedef struct
{
   void (*RfpDataFunction)(uint8_t *, uint32_t, uint32_t);
} RFPDataFunctions_TypeDef;
typedef struct
{
   void (*RfpMessageFunction)(uint8_t *, uint32_t, uint32_t);
} RFPMessageFunctions_TypeDef;
void RFP_Handle(void);
void RFP_Init(RFP_TypeDef *RadioProtocol, RFPDeviceID_TypeDef Device);
void RFP_RegisterCommandFunction(RFPCommands_TypeDef Command, void (*Callback)(uint8_t *, uint32_t, uint32_t));
void RFP_RegisterDataFunction(void (*Callback)(uint8_t *, uint32_t, uint32_t));
void RFP_RegisterMessageFunction(RFPMessage_TypeDef Message, void (*Callback)(uint8_t *, uint32_t, uint32_t));
void RFP_InterruptTask(void);
void RFP_SendData(RFPDeviceID_TypeDef Destination, RFPMessageType_TypeDef Type, uint8_t *Data, uint32_t Length);
#endif /* INC_RFP_H_ */
