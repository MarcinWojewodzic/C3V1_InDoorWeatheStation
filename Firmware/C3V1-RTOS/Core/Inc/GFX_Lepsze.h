/*
 * GFX_Lepsze.h
 *
 *  Created on: Jul 29, 2021
 *      Author: Marcin
 */

#ifndef INC_GFX_LEPSZE_H_
#define INC_GFX_LEPSZE_H_
#define SPACE_WIDHT 2
#include "GFX_BW.h"
//
// Select if Background is transparent or some color in charecter drawing.
//
typedef enum
{
   BG_TRANSPARENT = 0,
   BG_COLOR       = 1
} BG_FONT;

//
// Character description structure
//
typedef struct
{
   uint8_t CharWidth;
   uint16_t CharOffset;
} FONT_CHAR_INFO;

//
// Font information structure
//
typedef struct
{
   uint8_t CharHeight;
   uint8_t StartChar;
   uint8_t EndChar;
   uint8_t SpaceWidth;
   const FONT_CHAR_INFO *CharDescription;
   const uint8_t *CharBitmaps;
} FONT_INFO;

void EF_SetFont(const FONT_INFO *Font);

uint8_t EF_GetFontHeight(void);
uint8_t EF_GetFontStartChar(void);
uint8_t EF_GetFontEndChar(void);
uint8_t EF_GetFontSpaceWidth(void);

uint8_t EF_PutChar(char ch, uint16_t PosX, uint16_t PosY, uint16_t Fcolor, BG_FONT Btransparent, uint16_t Bcolor, Screen_TypeDef Screen);
uint8_t EF_PutString(const uint8_t *pStr, uint16_t PosX, uint16_t PosY, uint16_t Fcolor, BG_FONT Btransparent, uint16_t Bcolor,
                     Screen_TypeDef Screen);

#endif /* INC_GFX_LEPSZE_H_ */
