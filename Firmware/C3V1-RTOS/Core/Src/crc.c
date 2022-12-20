/*
 * crc.c
 *
 *  Created on: 22 paź 2022
 *      Author: M
 */
#include "crc.h"
#include "main.h"
int RandomNumber[] = { 131456, 215134, 31254, 4135, 516454, 642754, 71363, 85362475, 9144316, 10341957, 11345134, 11354342 };
int k              = 0;
uint32_t Crc(uint32_t PreviuseCRC, uint32_t DataLength, uint8_t *data)
{
   PreviuseCRC ^= data[0];
   for(uint32_t i = 1; i < DataLength; i++)
   {
      uint32_t temp = data[i];
      if(temp == 0x00000000 )
      {
         temp = RandomNumber[k % (sizeof(RandomNumber)-1)];
         k++;
      }
      PreviuseCRC ^= ((uint32_t)(temp) * (256 * i));
      if(PreviuseCRC & 0x80000000)
      {
         PreviuseCRC ^= 0xABC245C2;
      }
      PreviuseCRC <<= 1;
   }
   k = 0;
   return PreviuseCRC;
}
CRCStatus_TypeDef RepareMessage(uint32_t OryginalCRC, uint8_t *data, uint32_t DataLength)
{
   uint8_t temp = 1;
   for(int i = 0; i < DataLength; i++)
   {
      for(int j = 0; j < 8; j++)
      {
         data[i] ^= (temp << j);
         if(OryginalCRC == Crc(CRC_INITIAL_VALUE, DataLength - 4, data))
         {
            return CRC_OK;
         }
         else
         {
            data[i] ^= (temp << j);
         }
      }
   }
   return CRC_ERROR;
}
