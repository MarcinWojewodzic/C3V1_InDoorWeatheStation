/*
 * Menu.c
 *
 *  Created on: Jan 3, 2023
 *      Author: M
 */
#include "main.h"

#include "FlagsDefinition.h"
#include "GFX_BW.h"
#include "MeasurmentVariable.h"
#include "Menu.h"
#include "cmsis_os.h"
#include "e_papier.h"
#include "printf.h"
#include "rtc.h"
#include "ssd1306_spi.h"
#include "stdio.h"
#include "tim.h"
extern osEventFlagsId_t C3V1FlagsHandle;
extern osMutexId_t MenuMutexHandle;
extern osMutexId_t ScreensDcMutexHandle;
extern osMutexId_t SSD1306MutexHandle;
extern osMutexId_t SPI1MutexHandle;
extern osMutexId_t RTCMutexHandle;
extern osMessageQueueId_t ChartQueueHandle;
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
      case MENU_CHOISE_CLEAR_EXTERNAL_MEMORY:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Czyszczenie Pamieci");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         sprintf(Temp, "WYMAGA POTWIERDZENIA");
         GFX_DrawString(0, 10, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         break;
      }
      case MENU_CHOISE_DARK_MODE:
      {
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "ON/OFF Tryb ciemny");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
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
   ChartDateAndType_TypeDef QueueChartData = { 0 };
   switch(Menu.ChoiseCnt)
   {
      case MENU_CHOISE_TIME:
      {
         RTC_TimeTypeDef RtcTime;
         RTC_DateTypeDef RtcDate;
         osMutexAcquire(RTCMutexHandle, osWaitForever);
         HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
         HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
         osMutexRelease(RTCMutexHandle);
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
                  osMutexAcquire(RTCMutexHandle, osWaitForever);
                  while(HAL_RTC_SetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN) != HAL_OK)
                  {
                     osDelay(100);
                  }
                  osMutexRelease(RTCMutexHandle);
               }
            }
            osDelay(50);
         }
         break;
      }
      case MENU_CHOISE_DATE:
      {
         RTC_TimeTypeDef RtcTime;
         RTC_DateTypeDef RtcDate;
         osMutexAcquire(RTCMutexHandle, osWaitForever);
         HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
         HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
         osMutexRelease(RTCMutexHandle);
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         while(Menu.f == 0)
         {
            if(Menu.ButtonCnt == 0)
            {
               RtcDate.Date = (TIM5->CNT / 4) % 31 + 1;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
               RtcDate.Month = (TIM5->CNT / 4) % 12 + 1;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
               RtcDate.Year = (TIM5->CNT / 4) % 99;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
                  osMutexAcquire(RTCMutexHandle, osWaitForever);
                  while(HAL_RTC_SetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN) != HAL_OK)
                  {
                     osDelay(100);
                  }
                  osMutexRelease(RTCMutexHandle);
               }
            }
            osDelay(50);
         }
         break;
      }
      case MENU_CHOISE_CHART:
      {
         RTC_TimeTypeDef RtcTime;
         RTC_DateTypeDef RtcDate;
         osMutexAcquire(RTCMutexHandle, osWaitForever);
         HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
         HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
         osMutexRelease(RTCMutexHandle);
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         Menu.f = 0;
         while(Menu.f == 0)
         {
            if(Menu.ButtonCnt == 0)
            {
               RtcDate.Date = (TIM5->CNT / 4) % 31 + 1;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
               RtcDate.Month = (TIM5->CNT / 4) % 12 + 1;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
               RtcDate.Year = (TIM5->CNT / 4) % 99;
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "%d : %d : 20%d :", RtcDate.Date, RtcDate.Month, RtcDate.Year);
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
                  Menu.ButtonCnt       = 0;
                  Menu.f               = 1;
                  QueueChartData.Date  = RtcDate.Date;
                  QueueChartData.Month = RtcDate.Month;
                  QueueChartData.Year  = RtcDate.Year;
               }
            }
            osDelay(50);
         }

         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(SSD1306MutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         ssd1306_clear();
         sprintf(Temp, "Dokonaj Wyboru");
         GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
         ssd1306_display();
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(SSD1306MutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            osDelay(100);
         }
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            osDelay(100);
            switch((TIM5->CNT / 4) % 9)
            {
               case INTERNAL_PM1:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "INTERNAL_PM1");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = INTERNAL_PM1;
                  break;
               }
               case INTERNAL_PM25:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "INTERNAL_PM25");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = INTERNAL_PM25;
                  break;
               }
               case INTERNAL_PM10:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "INTERNAL_PM10");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = INTERNAL_PM10;
                  break;
               }
               case EXTERNAL_PM1:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "EXTERNAL_PM1");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = EXTERNAL_PM1;
                  break;
               }
               case EXTERNAL_PM25:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "EXTERNAL_PM25");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = EXTERNAL_PM25;
                  break;
               }
               case EXTERNAL_PM10:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "EXTERNAL_PM10");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = EXTERNAL_PM10;
                  break;
               }
               case PRESSURE:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "PRESSURE");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = PRESSURE;
                  break;
               }
               case EXTERNAL_TEMPERATURE:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "EXTERNAL_TEMPERATURE");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = EXTERNAL_TEMPERATURE;
                  break;
               }
               case EXTERNAL_HUMIDITY:
               {
                  osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
                  osMutexAcquire(SSD1306MutexHandle, osWaitForever);
                  osMutexAcquire(SPI1MutexHandle, osWaitForever);
                  ssd1306_clear();
                  sprintf(Temp, "EXTERNAL_HUMIDITY");
                  GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
                  ssd1306_display();
                  osMutexRelease(SPI1MutexHandle);
                  osMutexRelease(SSD1306MutexHandle);
                  osMutexRelease(ScreensDcMutexHandle);
                  QueueChartData.ChartType = EXTERNAL_HUMIDITY;
               }
            }
         }
         osDelay(50);
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         TIM5->CNT = 0;
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            if((TIM5->CNT / 4) % 2 == 0)
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "WYMAGA POTWIERDZENIA");
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               sprintf(Temp, "NIE");
               GFX_DrawFillRectangle(5, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(10, 30, Temp, BLACK, 1, OLED);
               sprintf(Temp, "TAK");
               GFX_DrawString(100, 30, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            else
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "WYMAGA POTWIERDZENIA");
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               sprintf(Temp, "NIE");
               GFX_DrawString(10, 30, Temp, WHITE, 0, OLED);
               sprintf(Temp, "TAK");
               GFX_DrawFillRectangle(95, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(100, 30, Temp, BLACK, 1, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            osDelay(100);
         }

         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         if((TIM5->CNT / 4) % 2 == 1)
         {
            osMessageQueuePut(ChartQueueHandle, &QueueChartData, 0, osWaitForever);
         }
         break;
      }

      case MENU_CHOISE_CLEAR_EXTERNAL_MEMORY:
      {
         TIM5->CNT = 0;
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            if((TIM5->CNT / 4) % 2 == 0)
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "WYMAGA POTWIERDZENIA");
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               sprintf(Temp, "NIE");
               GFX_DrawFillRectangle(5, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(10, 30, Temp, BLACK, 1, OLED);
               sprintf(Temp, "TAK");
               GFX_DrawString(100, 30, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            else
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "WYMAGA POTWIERDZENIA");
               GFX_DrawString(0, 0, Temp, WHITE, 0, OLED);
               sprintf(Temp, "NIE");
               GFX_DrawString(10, 30, Temp, WHITE, 0, OLED);
               sprintf(Temp, "TAK");
               GFX_DrawFillRectangle(95, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(100, 30, Temp, BLACK, 1, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            osDelay(100);
         }

         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         if((TIM5->CNT / 4) % 2 == 1)
         {
            osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
            osMutexAcquire(SSD1306MutexHandle, osWaitForever);
            osMutexAcquire(SPI1MutexHandle, osWaitForever);
            ssd1306_clear();
            sprintf(Temp, "CZYSZCZENIE!!!");
            GFX_DrawString(0, 0, Temp, BLACK, 1, OLED);
            ssd1306_display();
            osDelay(10000);
            osMutexRelease(SPI1MutexHandle);
            osMutexRelease(SSD1306MutexHandle);
            osMutexRelease(ScreensDcMutexHandle);
         }
         break;
      }
      case MENU_CHOISE_DARK_MODE:
      {
         TIM5->CNT = 0;
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            if((TIM5->CNT / 4) % 2 == 0)
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "OFF");
               GFX_DrawFillRectangle(5, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(10, 30, Temp, BLACK, 1, OLED);
               sprintf(Temp, "ON");
               GFX_DrawString(100, 30, Temp, WHITE, 0, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            else
            {
               osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
               osMutexAcquire(SSD1306MutexHandle, osWaitForever);
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               ssd1306_clear();
               sprintf(Temp, "OFF");
               GFX_DrawString(10, 30, Temp, WHITE, 0, OLED);
               sprintf(Temp, "ON");
               GFX_DrawFillRectangle(95, 25, 25, 18, WHITE, OLED);
               GFX_DrawString(100, 30, Temp, BLACK, 1, OLED);
               ssd1306_display();
               osMutexRelease(SPI1MutexHandle);
               osMutexRelease(SSD1306MutexHandle);
               osMutexRelease(ScreensDcMutexHandle);
            }
            osDelay(100);
         }

         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         if((TIM5->CNT / 4) % 2 == 1)
         {
            osEventFlagsSet(C3V1FlagsHandle, E_PAPIER_DARK_MODE);
         }
         else
         {
            osEventFlagsClear(C3V1FlagsHandle, E_PAPIER_DARK_MODE);
         }
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
