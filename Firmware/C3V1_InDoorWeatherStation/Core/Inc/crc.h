/*
 * crc.h
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_
#define CRC_INITIAL_VALUE 0xffffffff
#include "main.h"
typedef enum
{
	CRC_ERROR,
	CRC_OK
}CRCStatus_TypeDef;
uint32_t Crc(uint32_t PreviousCRC, uint32_t DataLength, uint8_t *data);
CRCStatus_TypeDef RepareMessage(uint32_t OryginalCRC, uint8_t *data, uint32_t DataLength);
#endif /* INC_CRC_H_ */
