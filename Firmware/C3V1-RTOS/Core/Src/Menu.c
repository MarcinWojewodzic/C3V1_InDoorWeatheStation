/*
 * Menu.c
 *
 *  Created on: Jan 3, 2023
 *      Author: M
 */
#include "main.h"

#include "GFX_BW.h"
#include "Menu.h"
#include "cmsis_os.h"
#include "e_papier.h"
#include "printf.h"
#include "rtc.h"
#include "ssd1306_spi.h"
#include "stdio.h"
#include "tim.h"
extern osMutexId_t MenuMutexHandle;
extern osMutexId_t ScreensDcMutexHandle;
extern osMutexId_t SSD1306MutexHandle;
extern osMutexId_t SPI1MutexHandle;

static void MENU_ChangeState(void);

static void MENU_IdleFunction(void);
static void MENU_ChoiseFunction(void);
static void MENU_RunningFunction(void);
static void MENU_EndFunction(void);
MenuTransitionTable_TypeDef TransitionTable[] = { { MENU_STATE_IDLE, MENU_STATE_CHOISE, MENU_EVENT_NEW },
                                                  { MENU_STATE_CHOISE, MENU_STATE_RUNNING, MENU_EVENT_CHOISE },
                                                  { MENU_STATE_CHOISE, MENU_STATE_END, MENU_EVENT_END },
                                                  { MENU_STATE_RUNNING, MENU_STATE_END, MENU_EVENT_END },
                                                  { MENU_EVENT_END, MENU_STATE_IDLE, MENU_EVENT_NOTHING } };
MenuFunctions_TypeDef StateFunction[]         = { { MENU_IdleFunction }, { MENU_ChoiseFunction }, { MENU_RunningFunction }, { MENU_EndFunction } };
Menu_TypeDef Menu                             = { 0 };
static void MENU_ChangeState(void)
{
   for(int i = 0; i < TRANSITION_TABLE_SIZE; i++)
   {
      if(Menu.State == TransitionTable[i].Source && Menu.NewEvent == TransitionTable[i].Event)
      {
         Menu.State    = TransitionTable[i].Destination;
         Menu.NewEvent = MENU_EVENT_NOTHING;
         return;
      }
   }
   Menu.NewEvent = MENU_EVENT_NOTHING;
}
void MENU_Init(void)
{
   HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}
void MENU_Handler(void)
{
   MENU_ChangeState();
   if(StateFunction[Menu.State].MenuFunction != NULL)
   {
      StateFunction[Menu.State].MenuFunction();
   }
}
static void MENU_IdleFunction(void)
{
   if(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
   {
      osMutexAcquire(MenuMutexHandle, osWaitForever);
      Menu.NewEvent = MENU_EVENT_NEW;
      while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
      {
         osDelay(100);
      }
   }
}
static void MENU_ChoiseFunction(void)
{
   char Temp[100];
   switch((TIM5->CNT / 4) % MENU_CHOISE_CNT)
   {
      case MENU_CHOISE_TIME:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Ustaw godzine");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      case MENU_CHOISE_DATE:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Ustaw date");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      case MENU_CHOISE_CHART:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Wykresy");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      default:
      {
         break;
      }
   }
   if(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
   {
      Menu.NewEvent  = MENU_EVENT_CHOISE;
      Menu.ChoiseCnt = (TIM5->CNT / 4) % MENU_CHOISE_CNT;
      TIM5->CNT      = 0;
      Menu.f         = 0;
      while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
      {
         osDelay(100);
      }
   }
}
static void MENU_RunningFunction(void)
{
   char Temp[100];
   switch(Menu.ChoiseCnt)
   {
      case MENU_CHOISE_TIME:
      {
         RTC_TimeTypeDef RtcTime;
         RTC_DateTypeDef RtcDate;
         HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
         HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         while(Menu.f == 0)
         {
            if(Menu.ButtonCnt == 0)
            {
               RtcTime.Hours = (TIM5->CNT / 4) % 24;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
               if(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
               {
                  Menu.ButtonCnt++;
                  TIM5->CNT = 0;
                  while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
                  {
                     osDelay(100);
                  }
               }
            }
            else if(Menu.ButtonCnt == 1)
            {
               RtcTime.Minutes = (TIM5->CNT / 4) % 60;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
               if(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
               {
                  Menu.ButtonCnt++;
                  TIM5->CNT = 0;
                  while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
                  {
                     osDelay(100);
                  }
               }
            }
            else
            {
               RtcTime.Seconds = (TIM5->CNT / 4) % 60;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
               if(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
               {
                  Menu.ButtonCnt++;
                  TIM5->CNT = 0;
                  while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
                  {
                     osDelay(100);
                  }
                  Menu.ButtonCnt = 0;
                  Menu.f         = 1;
                  HAL_RTC_SetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
                  HAL_RTC_SetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
               }
            }
            osDelay(50);
         }
         break;
      }
      case MENU_CHOISE_DATE:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Ustaw date");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      case MENU_CHOISE_CHART:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Wykresy");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      default:
      {
         break;
      }
   }
   Menu.NewEvent = MENU_EVENT_END;
}
static void MENU_EndFunction(void)
{
   Menu.ButtonCnt = 0;
   Menu.ChoiseCnt = 0;
   Menu.NewEvent  = MENU_EVENT_NOTHING;
   Menu.f         = 0;
   TIM5->CNT      = 0;
   osMutexRelease(MenuMutexHandle);
}
