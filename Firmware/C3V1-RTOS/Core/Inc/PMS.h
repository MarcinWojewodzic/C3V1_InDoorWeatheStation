/*
 * PMS.h
 *
 *  Created on: Nov 6, 2022
 *      Author: M
 */

#ifndef INC_PMS_H_
#define INC_PMS_H_
#include "main.h"
void PMS_Reset(void);
void PMS_ReceiveToIdle(uint8_t *Data, uint32_t Length);
void PMS_EnterSleepMode(void);
void PMS_ExitSleepMode(void);
#endif /* INC_PMS_H_ */
