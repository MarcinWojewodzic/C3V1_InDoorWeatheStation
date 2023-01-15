/*
 * Menu.h
 *
 *  Created on: Jan 3, 2023
 *      Author: M
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_
#define TRANSITION_TABLE_SIZE sizeof(TransitionTable) / sizeof(MenuTransitionTable_TypeDef)
typedef enum
{
   MENU_STATE_IDLE,
   MENU_STATE_CHOISE,
   MENU_STATE_RUNNING,
   MENU_STATE_END
} MenuState_TypeDef;
typedef enum
{
   MENU_EVENT_NOTHING,
   MENU_EVENT_NEW,
   MENU_EVENT_CHOISE,
   MENU_EVENT_END
} MenuEvent_TypeDef;
typedef enum
{
   MENU_CHOISE_TIME,
   MENU_CHOISE_DATE,
   MENU_CHOISE_CHART,
   MENU_CHOISE_CLEAR_EXTERNAL_MEMORY,
   MENU_CHOISE_DARK_MODE,
   MENU_CHOISE_CNT
} MenuChoise_TypeDef;
typedef struct
{
   uint32_t ChoiseCnt;
   uint32_t ButtonCnt;
   MenuState_TypeDef State;
   MenuEvent_TypeDef NewEvent;
   uint8_t f;
} Menu_TypeDef;
typedef struct
{
   void (*MenuFunction)(void);
} MenuFunctions_TypeDef;
typedef struct
{
   MenuState_TypeDef Source;
   MenuState_TypeDef Destination;
   MenuEvent_TypeDef Event;
} MenuTransitionTable_TypeDef;

void MENU_Init(void);
void MENU_Handler(void);
#endif /* INC_MENU_H_ */
