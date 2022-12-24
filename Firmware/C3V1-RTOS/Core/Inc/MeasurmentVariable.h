/*
 * MeasurmentVariable.h
 *
 *  Created on: Dec 24, 2022
 *      Author: M
 */

#ifndef INC_MEASURMENTVARIABLE_H_
#define INC_MEASURMENTVARIABLE_H_

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

#endif /* INC_MEASURMENTVARIABLE_H_ */
