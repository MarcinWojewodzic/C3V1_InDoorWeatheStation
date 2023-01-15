/*
 * MeasurmentVariable.h
 *
 *  Created on: Dec 24, 2022
 *      Author: M
 */

#ifndef INC_MEASURMENTVARIABLE_H_
#define INC_MEASURMENTVARIABLE_H_
#include "rfp.h"
typedef enum
{
   INTERNAL_PM1,
   INTERNAL_PM25,
   INTERNAL_PM10,
   EXTERNAL_PM1,
   EXTERNAL_PM25,
   EXTERNAL_PM10,
   PRESSURE,
   EXTERNAL_TEMPERATURE,
   EXTERNAL_HUMIDITY
} ChartType_TypeDef;
typedef struct
{
   uint8_t Reserved;
   uint8_t Second;
   uint8_t Minute;
   uint8_t Hour;
   uint16_t InternalPM1;
   uint16_t InternalPM25;
   uint16_t InternalPM10;
   uint16_t ExternalPM1;
   uint16_t ExternalPM25;
   uint16_t ExternalPM10;
   float Pressure;
   float ExternalTemperature;
   float ExternalHumidity;
} MemoryVariable_TypeDef;
typedef struct
{
   uint8_t Date;
   uint8_t Month;
   uint8_t Year;
   uint32_t StartFlashPage;
   uint8_t Length;
   uint32_t Crc;
} FramDateChart_TypeDef;
typedef struct
{
   MemoryVariable_TypeDef Record[9];
   uint32_t PageCRC;
} PageVariable_TypeDef;
typedef struct
{
   float ExtTemperature;
   float ExtHumidity;
   float InternalTemperature;
   float InternalHumidity;
   float Pressure;
   uint16_t ExtPM1;
   uint16_t ExtPM25;
   uint16_t ExtPM10;
   uint16_t InternalPM1;
   uint16_t InternalPM25;
   uint16_t InternalPM10;
   float C1V1InternalTemperature;
   uint16_t Brightness;
   float BatteryVoltage;
   uint8_t BatteryState;
} MV_TypeDef;
typedef struct
{
   uint16_t Year;
   uint8_t Month;
   uint8_t Date;
   ChartType_TypeDef ChartType;
} ChartDateAndType_TypeDef;
typedef struct
{
   RFPMessageType_TypeDef MessageType;
   uint8_t Data;
   uint16_t AdditionalData;
} RfpMessage_TypeDef;
typedef union
{
   uint16_t IntegerType[800];
   float FloatingPointType[800];
} ChartTypeVariable_TypeDef;
typedef struct
{
   double Sunrise;
   double Sunset;
   double Sunup;
} Sunrise_TypeDef;
#endif /* INC_MEASURMENTVARIABLE_H_ */
