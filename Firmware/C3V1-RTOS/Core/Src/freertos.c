/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "printf.h"

#include "GFX_BW.h"
#include "e_papier.h"
#include "flash_spi.h"
#include "fonts/fonts.h"
#include "fram.h"
#include "rfp.h"
#include "ssd1306_spi.h"

#include "dma.h"
#include "gpio.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

#include "bme280_spi.h"

#include "AirPollutionNorms.h"
#include "FlagsDefinition.h"
#include "FramOrganization.h"
#include "GFX_Lepsze.h"
#include "MeasurmentVariable.h"
#include "Menu.h"
#include "PMS.h"
#include "WS2812b.h"
#include "crc.h"
#include "flash_spi.h"
#include "fonts/Tnr10ppt.h"
#include "fonts/Tnr12ppt.h"
#include "fonts/Tnr16ppt.h"
#include "fonts/Tnr22ppt.h"
#include "fonts/Tnr28ppt.h"
#include "fram.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
RFP_TypeDef Rfp      = { 0 };
BME280_t Bme         = { 0 };
flash_t Flash        = { 0 };
fram_t Fram          = { 0 };
uint8_t PMSData[100] = { 0 }, PMSFlag = 0;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
   .name       = "defaultTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for InitAndTimeTask */
osThreadId_t InitAndTimeTaskHandle;
const osThreadAttr_t InitAndTimeTask_attributes = {
   .name       = "InitAndTimeTask",
   .stack_size = 6144 * 4,
   .priority   = (osPriority_t)osPriorityRealtime7,
};
/* Definitions for RfpTask */
osThreadId_t RfpTaskHandle;
const osThreadAttr_t RfpTask_attributes = {
   .name       = "RfpTask",
   .stack_size = 4096 * 4,
   .priority   = (osPriority_t)osPriorityRealtime7,
};
/* Definitions for MeasurmentTask */
osThreadId_t MeasurmentTaskHandle;
const osThreadAttr_t MeasurmentTask_attributes = {
   .name       = "MeasurmentTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for InternalMeasurmentTask */
osThreadId_t InternalMeasurmentTaskHandle;
const osThreadAttr_t InternalMeasurmentTask_attributes = {
   .name       = "InternalMeasurmentTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for MoonPhaseTask */
osThreadId_t MoonPhaseTaskHandle;
const osThreadAttr_t MoonPhaseTask_attributes = {
   .name       = "MoonPhaseTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityLow7,
};
/* Definitions for E_PapierDrawingTask */
osThreadId_t E_PapierDrawingTaskHandle;
const osThreadAttr_t E_PapierDrawingTask_attributes = {
   .name       = "E_PapierDrawingTask",
   .stack_size = 5000 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for E_PapierDisplayTask */
osThreadId_t E_PapierDisplayTaskHandle;
const osThreadAttr_t E_PapierDisplayTask_attributes = {
   .name       = "E_PapierDisplayTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for MenuTask */
osThreadId_t MenuTaskHandle;
const osThreadAttr_t MenuTask_attributes = {
   .name       = "MenuTask",
   .stack_size = 4096 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for SaveMemoryTask */
osThreadId_t SaveMemoryTaskHandle;
const osThreadAttr_t SaveMemoryTask_attributes = {
   .name       = "SaveMemoryTask",
   .stack_size = 4096 * 4,
   .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for ChartTask */
osThreadId_t ChartTaskHandle;
const osThreadAttr_t ChartTask_attributes = {
   .name       = "ChartTask",
   .stack_size = 6144 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for RfpMessageTask */
osThreadId_t RfpMessageTaskHandle;
const osThreadAttr_t RfpMessageTask_attributes = {
   .name       = "RfpMessageTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityRealtime,
};
/* Definitions for SunriseAndSunsetTask */
osThreadId_t SunriseAndSunsetTaskHandle;
const osThreadAttr_t SunriseAndSunsetTask_attributes = {
   .name       = "SunriseAndSunsetTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for WS2812Task */
osThreadId_t WS2812TaskHandle;
const osThreadAttr_t WS2812Task_attributes = {
   .name       = "WS2812Task",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for MeasurmentQueue */
osMessageQueueId_t MeasurmentQueueHandle;
const osMessageQueueAttr_t MeasurmentQueue_attributes = { .name = "MeasurmentQueue" };
/* Definitions for MoonPhaseQueue */
osMessageQueueId_t MoonPhaseQueueHandle;
const osMessageQueueAttr_t MoonPhaseQueue_attributes = { .name = "MoonPhaseQueue" };
/* Definitions for ChartQueue */
osMessageQueueId_t ChartQueueHandle;
const osMessageQueueAttr_t ChartQueue_attributes = { .name = "ChartQueue" };
/* Definitions for DataToSaveQueue */
osMessageQueueId_t DataToSaveQueueHandle;
const osMessageQueueAttr_t DataToSaveQueue_attributes = { .name = "DataToSaveQueue" };
/* Definitions for RfpMessageQueue */
osMessageQueueId_t RfpMessageQueueHandle;
const osMessageQueueAttr_t RfpMessageQueue_attributes = { .name = "RfpMessageQueue" };
/* Definitions for ExternalMeasurmentQueue */
osMessageQueueId_t ExternalMeasurmentQueueHandle;
const osMessageQueueAttr_t ExternalMeasurmentQueue_attributes = { .name = "ExternalMeasurmentQueue" };
/* Definitions for SunriseQueue */
osMessageQueueId_t SunriseQueueHandle;
const osMessageQueueAttr_t SunriseQueue_attributes = { .name = "SunriseQueue" };
/* Definitions for WS2812Queue */
osMessageQueueId_t WS2812QueueHandle;
const osMessageQueueAttr_t WS2812Queue_attributes = { .name = "WS2812Queue" };
/* Definitions for WS2812bMeasurmentQueue */
osMessageQueueId_t WS2812bMeasurmentQueueHandle;
const osMessageQueueAttr_t WS2812bMeasurmentQueue_attributes = { .name = "WS2812bMeasurmentQueue" };
/* Definitions for TimeBreakMeasurmentQueue */
osMessageQueueId_t TimeBreakMeasurmentQueueHandle;
const osMessageQueueAttr_t TimeBreakMeasurmentQueue_attributes = { .name = "TimeBreakMeasurmentQueue" };
/* Definitions for MenuTimer */
osTimerId_t MenuTimerHandle;
const osTimerAttr_t MenuTimer_attributes = { .name = "MenuTimer" };
/* Definitions for ScreensDcMutex */
osMutexId_t ScreensDcMutexHandle;
const osMutexAttr_t ScreensDcMutex_attributes = { .name = "ScreensDcMutex" };
/* Definitions for SSD1306Mutex */
osMutexId_t SSD1306MutexHandle;
const osMutexAttr_t SSD1306Mutex_attributes = { .name = "SSD1306Mutex" };
/* Definitions for SPI1Mutex */
osMutexId_t SPI1MutexHandle;
const osMutexAttr_t SPI1Mutex_attributes = { .name = "SPI1Mutex" };
/* Definitions for E_PAPIERMutex */
osMutexId_t E_PAPIERMutexHandle;
const osMutexAttr_t E_PAPIERMutex_attributes = { .name = "E_PAPIERMutex" };
/* Definitions for BME280Mutex */
osMutexId_t BME280MutexHandle;
const osMutexAttr_t BME280Mutex_attributes = { .name = "BME280Mutex" };
/* Definitions for MenuMutex */
osMutexId_t MenuMutexHandle;
const osMutexAttr_t MenuMutex_attributes = { .name = "MenuMutex" };
/* Definitions for RTCMutex */
osMutexId_t RTCMutexHandle;
const osMutexAttr_t RTCMutex_attributes = { .name = "RTCMutex" };
/* Definitions for EncoderMutex */
osMutexId_t EncoderMutexHandle;
const osMutexAttr_t EncoderMutex_attributes = { .name = "EncoderMutex" };
/* Definitions for C3V1Flags */
osEventFlagsId_t C3V1FlagsHandle;
const osEventFlagsAttr_t C3V1Flags_attributes = { .name = "C3V1Flags" };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void RFP_DataFunction(uint8_t *Data, uint32_t DataLength, uint32_t DataStart);
static double faza(double Rok, double Miesiac, double Dzien, double godzina, double min, double sec);
static double rang(double x);
static void Memory_ClearBuffer(PageVariable_TypeDef *Pv);
static float mapf(float val, float in_min, float in_max, float out_min, float out_max);
static uint32_t map(uint32_t val, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
void Wschod(double R, double M, double D, double *Wsch, double *Tran, double *Zach);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartInitAndTimeTask(void *argument);
void StartRfpTask(void *argument);
void StartMeasurmentTask(void *argument);
void StartInternalMeasurmentTask(void *argument);
void StartMoonPhaseTask(void *argument);
void StartE_PapierDrawingTask(void *argument);
void StartE_PapierDisplayTask(void *argument);
void StartMenuTask(void *argument);
void StartSaveMemoryTask(void *argument);
void StartChartTask(void *argument);
void StartRfpMessageTask(void *argument);
void StartSunriseAndSunsetTask(void *argument);
void StartWS2812Task(void *argument);
void MenuTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
   /* USER CODE BEGIN Init */

   /* USER CODE END Init */
   /* Create the mutex(es) */
   /* creation of ScreensDcMutex */
   ScreensDcMutexHandle = osMutexNew(&ScreensDcMutex_attributes);

   /* creation of SSD1306Mutex */
   SSD1306MutexHandle = osMutexNew(&SSD1306Mutex_attributes);

   /* creation of SPI1Mutex */
   SPI1MutexHandle = osMutexNew(&SPI1Mutex_attributes);

   /* creation of E_PAPIERMutex */
   E_PAPIERMutexHandle = osMutexNew(&E_PAPIERMutex_attributes);

   /* creation of BME280Mutex */
   BME280MutexHandle = osMutexNew(&BME280Mutex_attributes);

   /* creation of MenuMutex */
   MenuMutexHandle = osMutexNew(&MenuMutex_attributes);

   /* creation of RTCMutex */
   RTCMutexHandle = osMutexNew(&RTCMutex_attributes);

   /* creation of EncoderMutex */
   EncoderMutexHandle = osMutexNew(&EncoderMutex_attributes);

   /* USER CODE BEGIN RTOS_MUTEX */
   /* add mutexes, ... */
   /* USER CODE END RTOS_MUTEX */

   /* USER CODE BEGIN RTOS_SEMAPHORES */
   /* add semaphores, ... */
   /* USER CODE END RTOS_SEMAPHORES */

   /* Create the timer(s) */
   /* creation of MenuTimer */
   MenuTimerHandle = osTimerNew(MenuTimerCallback, osTimerOnce, NULL, &MenuTimer_attributes);

   /* USER CODE BEGIN RTOS_TIMERS */
   /* start timers, add new ones, ... */
   /* USER CODE END RTOS_TIMERS */

   /* Create the queue(s) */
   /* creation of MeasurmentQueue */
   MeasurmentQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &MeasurmentQueue_attributes);

   /* creation of MoonPhaseQueue */
   MoonPhaseQueueHandle = osMessageQueueNew(16, sizeof(double), &MoonPhaseQueue_attributes);

   /* creation of ChartQueue */
   ChartQueueHandle = osMessageQueueNew(16, sizeof(ChartDateAndType_TypeDef), &ChartQueue_attributes);

   /* creation of DataToSaveQueue */
   DataToSaveQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &DataToSaveQueue_attributes);

   /* creation of RfpMessageQueue */
   RfpMessageQueueHandle = osMessageQueueNew(16, sizeof(RfpMessage_TypeDef), &RfpMessageQueue_attributes);

   /* creation of ExternalMeasurmentQueue */
   ExternalMeasurmentQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &ExternalMeasurmentQueue_attributes);

   /* creation of SunriseQueue */
   SunriseQueueHandle = osMessageQueueNew(16, sizeof(Sunrise_TypeDef), &SunriseQueue_attributes);

   /* creation of WS2812Queue */
   WS2812QueueHandle = osMessageQueueNew(16, sizeof(Sunrise_TypeDef), &WS2812Queue_attributes);

   /* creation of WS2812bMeasurmentQueue */
   WS2812bMeasurmentQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &WS2812bMeasurmentQueue_attributes);

   /* creation of TimeBreakMeasurmentQueue */
   TimeBreakMeasurmentQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &TimeBreakMeasurmentQueue_attributes);

   /* USER CODE BEGIN RTOS_QUEUES */
   /* add queues, ... */
   /* USER CODE END RTOS_QUEUES */

   /* Create the thread(s) */
   /* creation of defaultTask */
   defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

   /* creation of InitAndTimeTask */
   InitAndTimeTaskHandle = osThreadNew(StartInitAndTimeTask, NULL, &InitAndTimeTask_attributes);

   /* creation of RfpTask */
   RfpTaskHandle = osThreadNew(StartRfpTask, NULL, &RfpTask_attributes);

   /* creation of MeasurmentTask */
   MeasurmentTaskHandle = osThreadNew(StartMeasurmentTask, NULL, &MeasurmentTask_attributes);

   /* creation of InternalMeasurmentTask */
   InternalMeasurmentTaskHandle = osThreadNew(StartInternalMeasurmentTask, NULL, &InternalMeasurmentTask_attributes);

   /* creation of MoonPhaseTask */
   MoonPhaseTaskHandle = osThreadNew(StartMoonPhaseTask, NULL, &MoonPhaseTask_attributes);

   /* creation of E_PapierDrawingTask */
   E_PapierDrawingTaskHandle = osThreadNew(StartE_PapierDrawingTask, NULL, &E_PapierDrawingTask_attributes);

   /* creation of E_PapierDisplayTask */
   E_PapierDisplayTaskHandle = osThreadNew(StartE_PapierDisplayTask, NULL, &E_PapierDisplayTask_attributes);

   /* creation of MenuTask */
   MenuTaskHandle = osThreadNew(StartMenuTask, NULL, &MenuTask_attributes);

   /* creation of SaveMemoryTask */
   SaveMemoryTaskHandle = osThreadNew(StartSaveMemoryTask, NULL, &SaveMemoryTask_attributes);

   /* creation of ChartTask */
   ChartTaskHandle = osThreadNew(StartChartTask, NULL, &ChartTask_attributes);

   /* creation of RfpMessageTask */
   RfpMessageTaskHandle = osThreadNew(StartRfpMessageTask, NULL, &RfpMessageTask_attributes);

   /* creation of SunriseAndSunsetTask */
   SunriseAndSunsetTaskHandle = osThreadNew(StartSunriseAndSunsetTask, NULL, &SunriseAndSunsetTask_attributes);

   /* creation of WS2812Task */
   WS2812TaskHandle = osThreadNew(StartWS2812Task, NULL, &WS2812Task_attributes);

   /* USER CODE BEGIN RTOS_THREADS */
   /* add threads, ... */
   /* USER CODE END RTOS_THREADS */

   /* creation of C3V1Flags */
   C3V1FlagsHandle = osEventFlagsNew(&C3V1Flags_attributes);

   /* USER CODE BEGIN RTOS_EVENTS */
   /* add events, ... */
   /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
   /* USER CODE BEGIN StartDefaultTask */
   /* Infinite loop */
   for(;;)
   {
      HAL_GPIO_TogglePin(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin);
      HAL_IWDG_Refresh(&hiwdg);
      osDelay(100);
   }
   /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartInitAndTimeTask */
/**
 * @brief Function implementing the InitAndTimeTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInitAndTimeTask */
void StartInitAndTimeTask(void *argument)
{
   /* USER CODE BEGIN StartInitAndTimeTask */
   RFP_Init(&Rfp, RFP_IDWS);
   e_papier_init(&hspi1);
   ssd1306_init(&hspi1);
   taskENTER_CRITICAL();
   BME280_Init(&Bme, &hspi1, BME280_CS_GPIO_Port, BME280_CS_Pin);
   GFX_SetFont(font_8x5);
   RFP_RegisterDataFunction(RFP_DataFunction);
   MENU_Init();
   fram_Init(&Fram, &hspi1, FRAM_HOLD_GPIO_Port, FRAM_CS_GPIO_Port, FRAM_WP_GPIO_Port, FRAM_HOLD_Pin, FRAM_CS_Pin, FRAM_WP_Pin);
   flash_Init(&Flash, &hspi1, FLASH_CS_GPIO_Port, FLASH_CS_Pin);
   //   fram_ChipErase(&Fram);
   //   HAL_IWDG_Refresh(&hiwdg);
   //   flash_ChipErase(&Flash);
   EF_SetFont(&timesNewRoman_12ptFontInfo);
   EF_PutString((uint8_t *)"WITAJ W PROJEKCIE INŻYNIERSKIM", 0, 0, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"NA TEN MOMENT TRWA INICJALIZACJA", 0, 20, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"ZA OKOŁO 2 MINUTY POJAWIĄ SIĘ WYNIKI", 0, 40, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"SPRAWDŹ ZIELONĄ DIODE!", 0, 80, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"JEŻELI MIGA TO WSZYSTKO DOBRZE", 0, 100, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"JEŻELI NIE TO NALEŻY WCISĄĆ RESET", 0, 120, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"FIRMWARE VERSION: 11.2", 0, 260, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"HARDWARE VERSION: 2.1", 0, 280, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"AUTOR: MARCIN WOJEWODZIC", 0, 220, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   EF_PutString((uint8_t *)"PROMOTOR: Dr Inż. MARCIN RODZIEWICZ", 0, 240, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
   e_papier_display();
   taskEXIT_CRITICAL();
   PMS_Reset();
   PMS_EnterSleepMode();
   osEventFlagsSet(C3V1FlagsHandle, INITIALIZE_ALL_FLAG);
   RTC_TimeTypeDef RtcTime;
   RTC_DateTypeDef RtcDate;
   RTC_DateTypeDef ActualRtcDate = { 0 };
   char data[100];
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
      osMutexAcquire(MenuMutexHandle, osWaitForever);
      osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
      osMutexAcquire(SSD1306MutexHandle, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      ssd1306_clear();
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      sprintf(data, "%d : %d", RtcTime.Hours, RtcTime.Minutes);
      EF_SetFont(&timesNewRoman_22ptFontInfo);
      if(RtcTime.Hours > 10)
      {
         EF_PutString((uint8_t *)data, 23, 0, WHITE, BG_TRANSPARENT, BLACK, OLED);
      }
      else
      {
         EF_PutString((uint8_t *)data, 38, 0, WHITE, BG_TRANSPARENT, BLACK, OLED);
      }
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      sprintf(data, "%d : %d : 20%d", RtcDate.Date, RtcDate.Month, RtcDate.Year);
      EF_PutString((uint8_t *)data, 10, 30, WHITE, BG_TRANSPARENT, BLACK, OLED);
      ;
      ssd1306_display();
      osMutexRelease(SPI1MutexHandle);
      osMutexRelease(SSD1306MutexHandle);
      osMutexRelease(ScreensDcMutexHandle);
      osMutexRelease(MenuMutexHandle);
      if(ActualRtcDate.Date != RtcDate.Date)
      {
         ActualRtcDate.Date = RtcDate.Date;
         osEventFlagsSet(C3V1FlagsHandle, MOON_PHASE_FLAG);
         osEventFlagsSet(C3V1FlagsHandle, NEW_DAY_TO_SAVE);
         osEventFlagsSet(C3V1FlagsHandle, NEW_DAY_FLAG);
      }
      osDelay(1000);
   }
   /* USER CODE END StartInitAndTimeTask */
}

/* USER CODE BEGIN Header_StartRfpTask */
/**
 * @brief Function implementing the RfpTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRfpTask */
void StartRfpTask(void *argument)
{
   /* USER CODE BEGIN StartRfpTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   /* Infinite loop */
   for(;;)
   {
      if(Rfp.Initialize == RFP_INITIALIZE)
      {
         RFP_Handle();
      }
      osDelay(10);
   }
   /* USER CODE END StartRfpTask */
}

/* USER CODE BEGIN Header_StartMeasurmentTask */
/**
 * @brief Function implementing the MeasurmentTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMeasurmentTask */
void StartMeasurmentTask(void *argument)
{
   /* USER CODE BEGIN StartMeasurmentTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   uint32_t Time                 = 1200000;
   uint8_t Command               = RFP_START_MEASURMENT;
   RfpMessage_TypeDef RfpMessage = { 0 };
   MV_TypeDef _Mv                = { 0 };
   /* Infinite loop */
   for(;;)
   {
      RfpMessage.AdditionalData = 0;
      RfpMessage.Data           = Command;
      RfpMessage.MessageType    = RFP_COMMAND;
      osMessageQueuePut(RfpMessageQueueHandle, &RfpMessage, 0, osWaitForever);
      PMSFlag = 0;
      PMS_ExitSleepMode();
      osEventFlagsWait(C3V1FlagsHandle, SEND_MEASURMENT_COMMAND_FLAG, osFlagsWaitAny, osWaitForever);
      osMessageQueueGet(TimeBreakMeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      if(!(_Mv.BatteryState & 0x01))
      {
         Time -= 600000;
         Time -= (100000 * _Mv.Brightness / 1000);
         Time -= (300000 * (_Mv.BatteryVoltage * 100) / 430);
      }
      else
      {
         Time -= (750000 * (_Mv.BatteryVoltage * 100) / 430);
      }
      osDelay(Time);
      Time = 1200000;
   }
   /* USER CODE END StartMeasurmentTask */
}

/* USER CODE BEGIN Header_StartInternalMeasurmentTask */
/**
 * @brief Function implementing the InternalMeasurmentTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInternalMeasurmentTask */
void StartInternalMeasurmentTask(void *argument)
{
   /* USER CODE BEGIN StartInternalMeasurmentTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   MV_TypeDef _Mv = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(ExternalMeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      osMutexAcquire(BME280MutexHandle, osWaitForever);
      HAL_UARTEx_ReceiveToIdle_DMA(&huart2, PMSData, 100);
      uint32_t Time = HAL_GetTick();
      while(PMSFlag == 0)
      {
         osDelay(100);
         if(HAL_GetTick() - Time > 10000)
         {
            break;
         }
      }
      _Mv.InternalPM1  = ((PMSData[4] << 8) | PMSData[5]);
      _Mv.InternalPM25 = ((PMSData[6] << 8) | PMSData[7]);
      _Mv.InternalPM10 = ((PMSData[8] << 8) | PMSData[9]);
      PMS_EnterSleepMode();
      BME280_ReadAll(&Bme, &_Mv.InternalTemperature, &_Mv.Pressure, &_Mv.InternalHumidity);
      osMutexRelease(BME280MutexHandle);
      _Mv.Pressure += 10.0;
      osMutexRelease(SPI1MutexHandle);
      osMessageQueuePut(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMessageQueuePut(DataToSaveQueueHandle, &_Mv, 0, osWaitForever);
      osMessageQueuePut(WS2812bMeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMessageQueuePut(TimeBreakMeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osDelay(1);
   }
   /* USER CODE END StartInternalMeasurmentTask */
}

/* USER CODE BEGIN Header_StartMoonPhaseTask */
/**
 * @brief Function implementing the MoonPhaseTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMoonPhaseTask */
void StartMoonPhaseTask(void *argument)
{
   /* USER CODE BEGIN StartMoonPhaseTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, MOON_PHASE_FLAG, osFlagsWaitAny, osWaitForever);
      RTC_TimeTypeDef RtcTime;
      RTC_DateTypeDef RtcDate;
      double PhaseMoon = 0.0, TempPhaseNew = 0, TempPhaseOld = 0;
      uint32_t Cnt          = 0;
      uint8_t FullMoonPhase = 0, ThirdMoonPhase = 0, FirstMoonPhase = 0, NewMoonPhase = 0;
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      TempPhaseNew = faza((RtcDate.Year + 2000), RtcDate.Month, RtcDate.Date, 0, 0, 0);
      TempPhaseOld = TempPhaseNew;
      for(int Hours = 0; Hours < 24; Hours++)
      {
         for(int Minutes = 0; Minutes < 60; Minutes++)
         {

            TempPhaseNew = faza((RtcDate.Year + 2000), RtcDate.Month, RtcDate.Date, Hours, Minutes, 0);
            PhaseMoon += TempPhaseNew;
            if(TempPhaseNew > -50.5 && TempPhaseNew < -49.5)
            {
               ThirdMoonPhase = 1;
            }
            if(TempPhaseNew > -0.5 && TempPhaseNew < 0.5)
            {
               NewMoonPhase = 1;
            }
            if(TempPhaseNew > 49.5 && TempPhaseNew < 50.5)
            {
               FirstMoonPhase = 1;
            }
            if(TempPhaseNew < TempPhaseOld && FullMoonPhase == 0)
            {
               if(NewMoonPhase == 0)
               {
                  FullMoonPhase = 1;
               }
            }
            else
            {
               TempPhaseOld = TempPhaseNew;
            }
            Cnt++;
            osDelay(5);
         }
      }
      PhaseMoon /= (Cnt);
      if(FullMoonPhase == 1)
      {
         FullMoonPhase = 0;
         PhaseMoon     = 100;
      }
      else if(ThirdMoonPhase == 1)
      {
         ThirdMoonPhase = 0;
         PhaseMoon      = -50;
      }
      else if(NewMoonPhase == 1)
      {
         NewMoonPhase = 0;
         PhaseMoon    = 0;
      }
      else if(FirstMoonPhase == 1)
      {
         FirstMoonPhase = 0;
         PhaseMoon      = 50;
      }
      osMessageQueuePut(MoonPhaseQueueHandle, &PhaseMoon, 0, osWaitForever);
      osDelay(1);
   }
   /* USER CODE END StartMoonPhaseTask */
}

/* USER CODE BEGIN Header_StartE_PapierDrawingTask */
/**
 * @brief Function implementing the E_PapierDrawingTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartE_PapierDrawingTask */
void StartE_PapierDrawingTask(void *argument)
{
   /* USER CODE BEGIN StartE_PapierDrawingTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   RTC_TimeTypeDef RtcTime;
   RTC_DateTypeDef RtcDate;
   MV_TypeDef _Mv          = { 0 };
   double _MoonPhase       = 0;
   Sunrise_TypeDef Sunrise = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      e_papier_clear();
      for(int i = 1; i < 3; i++)
      {
         GFX_DrawLine(150 * i, 0, 150 * i, 300, BLACK, E_PAPIER);
      }
      GFX_DrawLine(0, 40, 400, 40, BLACK, E_PAPIER);
      char mes[100];
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      osMessageQueueGet(MoonPhaseQueueHandle, &_MoonPhase, 0, 0);
      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"WARUNKI ", 0, 0, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"ZEWNĘTRZNE", 0, 20, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_PutString((uint8_t *)"PM 1.0", 0, 40, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      sprintf(mes, "%d ug/m3", _Mv.ExtPM1);
      EF_PutString((uint8_t *)mes, 0, 55, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"PM 2.5", 0, 80, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d ug/m3", _Mv.ExtPM25);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 0, 95, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"PM 10.0", 0, 120, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d ug/m3", _Mv.ExtPM10);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 0, 135, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"TEMPERATURA", 0, 160, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f T", _Mv.ExtTemperature);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 0, 175, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"WILGOTNOŚĆ", 0, 200, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f %%", _Mv.ExtHumidity);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 0, 215, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"NAPIĘCIE", 0, 240, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"BATERII", 0, 255, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2fV", _Mv.BatteryVoltage);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 0, 270, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"WARUNKI ", 152, 0, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"WEWNĘTRZNE", 152, 20, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_PutString((uint8_t *)"PM 1.0", 152, 40, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      sprintf(mes, "%d ug/m3", _Mv.InternalPM1);
      EF_PutString((uint8_t *)mes, 152, 55, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"PM 2.5", 152, 80, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d ug/m3", _Mv.InternalPM25);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 152, 95, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"PM 10.0", 152, 120, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d ug/m3", _Mv.InternalPM10);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 152, 135, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"TEMPERATURA", 152, 160, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f T", _Mv.InternalTemperature);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 152, 175, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"WILGOTNOŚĆ", 152, 200, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f %%", _Mv.InternalHumidity);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 152, 215, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_12ptFontInfo);
      EF_PutString((uint8_t *)"CIŚNIENIE", 152, 240, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f hPa", _Mv.Pressure);
      EF_SetFont(&timesNewRoman_16ptFontInfo);
      EF_PutString((uint8_t *)mes, 152, 255, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_SetFont(&timesNewRoman_10ptFontInfo);
      EF_PutString((uint8_t *)"SEKCJA", 300, 0, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"DODATKOWA", 300, 20, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      EF_PutString((uint8_t *)"FAZA", 300, 40, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"KSIĘŻYCA", 300, 52, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%0.2f%%", _MoonPhase);
      if(_MoonPhase == 100)
      {
         EF_PutString((uint8_t *)"PEŁNIA", 300, 64, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      else if(_MoonPhase == -50)
      {
         EF_PutString((uint8_t *)"3 KWADRA", 300, 64, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      else if(_MoonPhase == 0)
      {
         EF_PutString((uint8_t *)"NÓW", 300, 64, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      else if(_MoonPhase == 50)
      {
         EF_PutString((uint8_t *)"1 KWADRA", 300, 64, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      else
      {
         if(_MoonPhase < 0)
         {
            _MoonPhase = -_MoonPhase;
         }
         sprintf(mes, "%0.2f%%", _MoonPhase);
         EF_PutString((uint8_t *)mes, 300, 64, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      EF_PutString((uint8_t *)"CZAS", 300, 76, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"NADEJŚCIA", 300, 88, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"DANYCH", 300, 100, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d : %d : %d", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
      EF_PutString((uint8_t *)mes, 300, 112, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d : %d : 20%d", RtcDate.Date, RtcDate.Month, RtcDate.Year);
      EF_PutString((uint8_t *)mes, 300, 124, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      osMessageQueueGet(SunriseQueueHandle, &Sunrise, 0, 0);
      uint8_t TempHour     = 0;
      uint8_t TempMinute   = 0;
      uint8_t TempSecond   = 0;
      uint32_t TempSunrise = (Sunrise.Sunrise + 1.0) * 3600;
      TempHour             = (TempSunrise / 3600);
      TempSunrise -= (TempHour * 3600);
      TempMinute = (TempSunrise / 60);
      TempSunrise -= (TempMinute * 60);
      TempSecond = TempSunrise;
      EF_PutString((uint8_t *)"WSCHÓD", 300, 136, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"SŁOŃCA", 300, 148, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d : %d : %d", TempHour, TempMinute, TempSecond);
      EF_PutString((uint8_t *)mes, 300, 160, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      uint32_t TempSunset = (Sunrise.Sunset + 1.0) * 3600;
      TempHour            = (TempSunset / 3600);
      TempSunset -= (TempHour * 3600);
      TempMinute = (TempSunset / 60);
      TempSunset -= (TempMinute * 60);
      TempSecond = TempSunset;
      EF_PutString((uint8_t *)"ZACHÓD", 300, 172, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"SŁOŃCA", 300, 184, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d : %d : %d", TempHour, TempMinute, TempSecond);
      EF_PutString((uint8_t *)mes, 300, 196, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      uint32_t TempSunup = (Sunrise.Sunup + 1.0) * 3600;
      TempHour           = (TempSunup / 3600);
      TempSunup -= (TempHour * 3600);
      TempMinute = (TempSunup / 60);
      TempSunup -= (TempMinute * 60);
      TempSecond = TempSunup;
      EF_PutString((uint8_t *)"GÓROWANIE", 300, 208, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      EF_PutString((uint8_t *)"SŁOŃCA", 300, 220, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      sprintf(mes, "%d : %d : %d", TempHour, TempMinute, TempSecond);
      EF_PutString((uint8_t *)mes, 300, 236, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);

      if(!(_Mv.BatteryState & 0x01))
      {
         EF_PutString((uint8_t *)"ŁADOWANIE", 300, 248, BLACK, BG_TRANSPARENT, WHITE, E_PAPIER);
      }
      osMutexRelease(E_PAPIERMutexHandle);
      osEventFlagsSet(C3V1FlagsHandle, E_PAPIER_DISPLAY_FLAG);
      osEventFlagsSet(C3V1FlagsHandle, SEND_MEASURMENT_COMMAND_FLAG);

      osDelay(1);
   }
   /* USER CODE END StartE_PapierDrawingTask */
}

/* USER CODE BEGIN Header_StartE_PapierDisplayTask */
/**
 * @brief Function implementing the E_PapierDisplayTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartE_PapierDisplayTask */
void StartE_PapierDisplayTask(void *argument)
{
   /* USER CODE BEGIN StartE_PapierDisplayTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, E_PAPIER_DISPLAY_FLAG, osFlagsWaitAny, osWaitForever);
      osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      e_papier_display();
      e_papier_clear();
      osMutexRelease(SPI1MutexHandle);
      osMutexRelease(E_PAPIERMutexHandle);
      osMutexRelease(ScreensDcMutexHandle);
      osDelay(1);
   }
   /* USER CODE END StartE_PapierDisplayTask */
}

/* USER CODE BEGIN Header_StartMenuTask */
/**
 * @brief Function implementing the MenuTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMenuTask */
void StartMenuTask(void *argument)
{
   /* USER CODE BEGIN StartMenuTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   /* Infinite loop */
   for(;;)
   {
      osMutexAcquire(EncoderMutexHandle, osWaitForever);
      MENU_Handler();
      osMutexRelease(EncoderMutexHandle);
      osDelay(10);
   }
   /* USER CODE END StartMenuTask */
}

/* USER CODE BEGIN Header_StartSaveMemoryTask */
/**
 * @brief Function implementing the SaveMemoryTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSaveMemoryTask */
void StartSaveMemoryTask(void *argument)
{
   /* USER CODE BEGIN StartSaveMemoryTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   osEventFlagsWait(C3V1FlagsHandle, NEW_DAY_TO_SAVE, osFlagsWaitAny, osWaitForever);
   PageVariable_TypeDef Pv             = { 0 };
   MV_TypeDef _Mv                      = { 0 };
   uint32_t Cnt                        = 0;
   RTC_TimeTypeDef RtcTime             = { 0 };
   RTC_DateTypeDef RtcDate             = { 0 };
   PageVariable_TypeDef ConfirmPv      = { 0 };
   FramDateChart_TypeDef FramDateChart = { 0 };
   uint8_t FramDataChartExistFlag      = 0;
   osMutexAcquire(SPI1MutexHandle, osWaitForever);
   uint32_t PageCnt          = fram_Read32(&Fram, LAST_PAGE_NUMBER_ADDR);
   uint32_t FramDateChartCnt = fram_Read32(&Fram, DATE_CHART_CNT_ADDR);
   osMutexRelease(SPI1MutexHandle);
   osMutexAcquire(RTCMutexHandle, osWaitForever);
   HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
   HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
   osMutexRelease(RTCMutexHandle);

   for(int i = DATE_CHART_ADDR_START; i < DATE_CHART_ADDR_END; i += 12)
   {
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      fram_Read(&Fram, i, &FramDateChart, 12);
      osMutexRelease(SPI1MutexHandle);
      if(FramDateChart.Date == RtcDate.Date && FramDateChart.Month == RtcDate.Month && FramDateChart.Year == RtcDate.Year)
      {
         FramDataChartExistFlag = 1;
         // FramDateChart.Length   = 0;
         // fram_Write(&Fram, DATE_CHART_ADDR_START + FramDateChartCnt * sizeof(FramDateChart_TypeDef), &FramDateChart,
         // sizeof(FramDateChart_TypeDef));
         break;
      }
   }
   if(FramDataChartExistFlag == 0)
   {
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      fram_Increment32(&Fram, DATE_CHART_CNT_ADDR);
      FramDateChart.Date           = RtcDate.Date;
      FramDateChart.Month          = RtcDate.Month;
      FramDateChart.Year           = RtcDate.Year;
      FramDateChart.StartFlashPage = PageCnt;
      FramDateChart.Length         = 0;
      FramDateChart.Crc            = CRC_INITIAL_VALUE;
      FramDateChartCnt             = fram_Read32(&Fram, DATE_CHART_CNT_ADDR);
      fram_Write(&Fram, DATE_CHART_ADDR_START + FramDateChartCnt * 12, &FramDateChart, 12);
      osMutexRelease(SPI1MutexHandle);
   }

   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(DataToSaveQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      if(osEventFlagsWait(C3V1FlagsHandle, NEW_DAY_TO_SAVE, osFlagsWaitAny, 1) != osFlagsErrorTimeout)
      {
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         PageCnt    = fram_Read32(&Fram, LAST_PAGE_NUMBER_ADDR);
         Pv.PageCRC = Crc(CRC_INITIAL_VALUE, 252, &Pv);
         flash_WritePage(&Flash, PageCnt, &Pv);
         flash_ReadPage(&Flash, PageCnt, &ConfirmPv);
         if(Pv.PageCRC == ConfirmPv.PageCRC)
         {
            fram_Increment32(&Fram, LAST_PAGE_NUMBER_ADDR);
            PageCnt = fram_Read32(&Fram, LAST_PAGE_NUMBER_ADDR);
            FramDateChart.Length++;
            FramDateChart.Crc = Crc(FramDateChart.Crc, 256, &Pv);
            FramDateChartCnt  = fram_Read32(&Fram, DATE_CHART_CNT_ADDR);
            fram_Write(&Fram, DATE_CHART_ADDR_START + FramDateChartCnt * 12, &FramDateChart, 12);
            Memory_ClearBuffer(&ConfirmPv);
            Memory_ClearBuffer(&Pv);
         }
         fram_Increment32(&Fram, DATE_CHART_CNT_ADDR);
         FramDateChart.Date           = RtcDate.Date;
         FramDateChart.Month          = RtcDate.Month;
         FramDateChart.Year           = RtcDate.Year;
         FramDateChart.StartFlashPage = PageCnt;
         FramDateChart.Length         = 0;
         FramDateChart.Crc            = CRC_INITIAL_VALUE;
         FramDateChartCnt             = fram_Read32(&Fram, DATE_CHART_CNT_ADDR);
         fram_Write(&Fram, DATE_CHART_ADDR_START + FramDateChartCnt * 12, &FramDateChart, 12);
         Cnt = 0;
         osMutexRelease(SPI1MutexHandle);
      }
      Pv.Record[Cnt].ExternalHumidity    = _Mv.ExtHumidity;
      Pv.Record[Cnt].ExternalPM1         = _Mv.ExtPM1;
      Pv.Record[Cnt].ExternalPM10        = _Mv.ExtPM10;
      Pv.Record[Cnt].ExternalPM25        = _Mv.ExtPM25;
      Pv.Record[Cnt].ExternalTemperature = _Mv.ExtTemperature;
      Pv.Record[Cnt].Hour                = RtcTime.Hours;
      Pv.Record[Cnt].InternalPM1         = _Mv.InternalPM1;
      Pv.Record[Cnt].InternalPM10        = _Mv.InternalPM10;
      Pv.Record[Cnt].InternalPM25        = _Mv.InternalPM25;
      Pv.Record[Cnt].Minute              = RtcTime.Minutes;
      Pv.Record[Cnt].Pressure            = _Mv.Pressure;
      Pv.Record[Cnt].Reserved            = Cnt;
      Pv.Record[Cnt].Second              = RtcTime.Seconds;
      Cnt++;
      if(Cnt == 9)
      {
         Cnt        = 0;
         Pv.PageCRC = Crc(CRC_INITIAL_VALUE, 252, &Pv);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         PageCnt = fram_Read32(&Fram, LAST_PAGE_NUMBER_ADDR);
         flash_WritePage(&Flash, PageCnt, &Pv);
         flash_ReadPage(&Flash, PageCnt, &ConfirmPv);
         if(Pv.PageCRC == ConfirmPv.PageCRC)
         {
            fram_Increment32(&Fram, LAST_PAGE_NUMBER_ADDR);
            PageCnt = fram_Read32(&Fram, LAST_PAGE_NUMBER_ADDR);
            FramDateChart.Length++;
            FramDateChart.Crc = Crc(FramDateChart.Crc, 256, &Pv);
            FramDateChartCnt  = fram_Read32(&Fram, DATE_CHART_CNT_ADDR);
            fram_Write(&Fram, DATE_CHART_ADDR_START + FramDateChartCnt * 12, &FramDateChart, 12);
            Memory_ClearBuffer(&ConfirmPv);
            Memory_ClearBuffer(&Pv);
         }
         osMutexRelease(SPI1MutexHandle);
      }
      osDelay(1);
   }
   /* USER CODE END StartSaveMemoryTask */
}

/* USER CODE BEGIN Header_StartChartTask */
/**
 * @brief Function implementing the ChartTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartChartTask */
void StartChartTask(void *argument)
{
   /* USER CODE BEGIN StartChartTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   ChartTypeVariable_TypeDef ChartVariable;
   uint8_t Hour[800]                   = { 0 };
   uint8_t Minute[800]                 = { 0 };
   ChartDateAndType_TypeDef Cda        = { 0 };
   uint8_t FramDataChartExistFlag      = 0;
   FramDateChart_TypeDef FramDateChart = { 0 };
   PageVariable_TypeDef Pv             = { 0 };
   uint16_t Y_Axis[800]                = { 0 };
   uint16_t X_Axis[800]                = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(ChartQueueHandle, &Cda, 0, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      e_papier_clear();
      GFX_DrawLine(63, 50, 63, 281, BLACK, E_PAPIER);
      GFX_DrawLine(40, 281, 399, 281, BLACK, E_PAPIER);
      GFX_DrawLine(40, 223, 399, 223, BLACK, E_PAPIER);
      GFX_DrawLine(40, 165, 399, 165, BLACK, E_PAPIER);
      GFX_DrawLine(40, 108, 399, 108, BLACK, E_PAPIER);
      GFX_DrawLine(40, 50, 399, 50, BLACK, E_PAPIER);
      for(int i = 1; i < 24; i++)
      {
         GFX_DrawLine(64 + i * 14, 281, 64 + i * 14, 270, BLACK, E_PAPIER);
      }
      for(int i = 1; i < 24; i++)
      {
         char Temp[3];
         sprintf(Temp, "%d", i);
         GFX_DrawString(64 + i * 14 - 5, 290, Temp, BLACK, 1, E_PAPIER);
      }
      osMutexRelease(E_PAPIERMutexHandle);
      if(Cda.ChartType == PRESSURE || Cda.ChartType == EXTERNAL_TEMPERATURE || Cda.ChartType == EXTERNAL_HUMIDITY)
      {
         for(int i = DATE_CHART_ADDR_START; i < DATE_CHART_ADDR_END; i += 12)
         {
            osMutexAcquire(SPI1MutexHandle, osWaitForever);
            fram_Read(&Fram, i, &FramDateChart, 12);
            osMutexRelease(SPI1MutexHandle);
            if(FramDateChart.Date == Cda.Date && FramDateChart.Month == Cda.Month && FramDateChart.Year == Cda.Year)
            {
               FramDataChartExistFlag = 1;
               break;
            }
         }
         if(FramDataChartExistFlag == 1)
         {
            FramDataChartExistFlag = 0;
            for(int i = 0; i < FramDateChart.Length; i++)
            {
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               flash_ReadPage(&Flash, i + FramDateChart.StartFlashPage, &Pv);
               osMutexRelease(SPI1MutexHandle);
               uint32_t TempCrc = Crc(CRC_INITIAL_VALUE, 252, &Pv);
               if(TempCrc == Pv.PageCRC)
               {
                  for(int j = (i * 9), k = 0; k < 9; k++)
                  {
                     if(Cda.ChartType == PRESSURE)
                     {
                        ChartVariable.FloatingPointType[j + k] = Pv.Record[k].Pressure;
                     }
                     else if(Cda.ChartType == EXTERNAL_TEMPERATURE)
                     {
                        ChartVariable.FloatingPointType[j + k] = Pv.Record[k].ExternalTemperature;
                     }
                     else
                     {
                        ChartVariable.FloatingPointType[j + k] = Pv.Record[k].ExternalHumidity;
                     }
                     Minute[j + k] = Pv.Record[k].Minute;
                     Hour[j + k]   = Pv.Record[k].Hour;
                  }
               }
            }
         }
         float _Max = 0, _Min = 10000000.0;
         int a      = 0;
         float _Avg = 0;
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            _Avg += ChartVariable.FloatingPointType[i];
         }
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            if(ChartVariable.FloatingPointType[i] > 1200.0)
            {
               ChartVariable.FloatingPointType[i] = (_Avg / (FramDateChart.Length * 9));
            }
            if(_Max < ChartVariable.FloatingPointType[i])
            {
               _Max = ChartVariable.FloatingPointType[i];
               a    = i;
            }
            if(_Min > ChartVariable.FloatingPointType[i] && ChartVariable.FloatingPointType[i] != 0.0)
            {
               _Min = ChartVariable.FloatingPointType[i];
            }
         }
         for(int i = 0; i < 800; i++)
         {
            if(ChartVariable.FloatingPointType[i] == 0)
            {
               ChartVariable.FloatingPointType[i] = _Min;
            }
            Y_Axis[i] = (uint16_t)mapf(ChartVariable.FloatingPointType[i], _Min, _Max, 20.0, 250.0);
            Y_Axis[i] = 300 - Y_Axis[i];
         }
         for(int i = 0; i < 800; i++)
         {
            if(Hour[i] == 0 && Minute[i] == 0 && X_Axis[i] == 0)
            {
               X_Axis[i] = 0xffff;
            }
            else
            {
               X_Axis[i] = map(Hour[i] * 60 + Minute[i], Hour[i] * 60, (Hour[i] + 1) * 60, 64 + (Hour[i] * 14), 64 + ((Hour[i] + 1) * 14));
            }
         }
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         if(X_Axis[0] != 0xffff && X_Axis[1] != 0xffff)
         {
            GFX_DrawLine(X_Axis[0], Y_Axis[0], X_Axis[1], Y_Axis[1], BLACK, E_PAPIER);
         }
         for(int i = 1; i < 800; i++)
         {
            if(X_Axis[i + 1] != 0xffff && X_Axis[i] != 0xffff)
            {
               GFX_DrawLine(X_Axis[i], Y_Axis[i], X_Axis[i + 1], Y_Axis[i + 1], BLACK, E_PAPIER);
            }
         }
         char Temp[10];
         float TempDifferencje = _Max - _Min;
         sprintf(Temp, "%0.2f", (_Min + TempDifferencje / 4));
         GFX_DrawString(0, 218, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%0.2f", (_Min + TempDifferencje / 2));
         GFX_DrawString(0, 160, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%0.2f", (_Max - TempDifferencje / 4));
         GFX_DrawString(0, 103, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%0.2f", _Min);
         GFX_DrawString(0, 276, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%0.2f", _Max);
         GFX_DrawString(0, 45, Temp, BLACK, 1, E_PAPIER);
         e_papier_display();
         osMutexAcquire(EncoderMutexHandle, osWaitForever);
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            osDelay(100);
         }
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         osMutexRelease(EncoderMutexHandle);
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(E_PAPIERMutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
      }
      else
      {
         for(int i = DATE_CHART_ADDR_START; i < DATE_CHART_ADDR_END; i += 12)
         {
            osMutexAcquire(SPI1MutexHandle, osWaitForever);
            fram_Read(&Fram, i, &FramDateChart, 12);
            osMutexRelease(SPI1MutexHandle);
            if(FramDateChart.Date == Cda.Date && FramDateChart.Month == Cda.Month && FramDateChart.Year == Cda.Year)
            {
               FramDataChartExistFlag = 1;
               break;
            }
         }
         if(FramDataChartExistFlag == 1)
         {
            FramDataChartExistFlag = 0;
            for(int i = 0; i < FramDateChart.Length; i++)
            {
               osMutexAcquire(SPI1MutexHandle, osWaitForever);
               flash_ReadPage(&Flash, i + FramDateChart.StartFlashPage, &Pv);
               osMutexRelease(SPI1MutexHandle);
               uint32_t TempCrc = Crc(CRC_INITIAL_VALUE, 252, &Pv);
               if(TempCrc == Pv.PageCRC)
               {
                  for(int j = (i * 9), k = 0; k < 9; k++)
                  {
                     if(Cda.ChartType == INTERNAL_PM1)
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].InternalPM1;
                     }
                     else if(Cda.ChartType == INTERNAL_PM25)
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].InternalPM25;
                     }
                     else if(Cda.ChartType == INTERNAL_PM10)
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].InternalPM10;
                     }
                     else if(Cda.ChartType == EXTERNAL_PM1)
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].ExternalPM1;
                     }
                     else if(Cda.ChartType == EXTERNAL_PM25)
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].ExternalPM25;
                     }
                     else
                     {
                        ChartVariable.IntegerType[j + k] = Pv.Record[k].ExternalPM10;
                     }
                     Minute[j + k] = Pv.Record[k].Minute;
                     Hour[j + k]   = Pv.Record[k].Hour;
                  }
               }
            }
         }
         uint16_t _Max = 0, _Min = 65000;
         uint32_t _Avg = 0;
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            _Avg += ChartVariable.IntegerType[i];
         }
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            if(ChartVariable.IntegerType[i] > 500)
            {
               ChartVariable.IntegerType[i] = _Avg / (FramDateChart.Length * 9);
            }
            if(_Max < ChartVariable.IntegerType[i])
            {
               _Max = ChartVariable.IntegerType[i];
            }
            if(_Min > ChartVariable.IntegerType[i] && ChartVariable.IntegerType[i] != 0.0)
            {
               _Min = ChartVariable.IntegerType[i];
            }
         }
         for(int i = 0; i < 800; i++)
         {
            if(ChartVariable.IntegerType[i] == 0)
            {
               ChartVariable.IntegerType[i] = _Min;
            }
            Y_Axis[i] = (uint16_t)map(ChartVariable.IntegerType[i], _Min, _Max, 20, 250);
            Y_Axis[i] = 300 - Y_Axis[i];
         }
         for(int i = 0; i < 800; i++)
         {
            if(Hour[i] == 0 && Minute[i] == 0 && X_Axis[i] == 0)
            {
               X_Axis[i] = 0xffff;
            }
            else
            {
               X_Axis[i] = map(Hour[i] * 60 + Minute[i], Hour[i] * 60, (Hour[i] + 1) * 60, 64 + (Hour[i] * 14), 64 + ((Hour[i] + 1) * 14));
            }
         }
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         char Temp[10];
         uint16_t TempDifferencje = _Max - _Min;
         sprintf(Temp, "%d", (_Min + TempDifferencje / 4));
         GFX_DrawString(0, 218, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%d", (_Min + TempDifferencje / 2));
         GFX_DrawString(0, 160, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%d", (_Max - TempDifferencje / 4));
         GFX_DrawString(0, 103, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%d", _Min);
         GFX_DrawString(0, 276, Temp, BLACK, 1, E_PAPIER);
         sprintf(Temp, "%d", _Max);
         GFX_DrawString(0, 45, Temp, BLACK, 1, E_PAPIER);
         if(X_Axis[0] != 0xffff && X_Axis[1] != 0xffff)
         {
            GFX_DrawLine(X_Axis[0], Y_Axis[0], X_Axis[1], Y_Axis[1], BLACK, E_PAPIER);
         }
         for(int i = 1; i < 800; i++)
         {
            if(X_Axis[i + 1] != 0xffff && X_Axis[i] != 0xffff)
            {
               GFX_DrawLine(X_Axis[i], Y_Axis[i], X_Axis[i + 1], Y_Axis[i + 1], BLACK, E_PAPIER);
            }
         }
         e_papier_display();
         osMutexAcquire(EncoderMutexHandle, osWaitForever);
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 1)
         {
            osDelay(100);
         }
         while(HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == 0)
         {
            osDelay(100);
         }
         osMutexRelease(EncoderMutexHandle);
         osMutexRelease(SPI1MutexHandle);
         osMutexRelease(E_PAPIERMutexHandle);
         osMutexRelease(ScreensDcMutexHandle);
      }
      for(int i = 0; i < 800; i++)
      {
         ChartVariable.IntegerType[i]       = 0;
         ChartVariable.FloatingPointType[i] = 0;
         Y_Axis[i]                          = 0;
         X_Axis[i]                          = 0;
         Hour[i]                            = 0;
         Minute[i]                          = 0;
      }
      FramDataChartExistFlag = 0;
      osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      e_papier_clear();
      e_papier_display();
      osMutexRelease(SPI1MutexHandle);
      osMutexRelease(E_PAPIERMutexHandle);
      osMutexRelease(ScreensDcMutexHandle);
      osDelay(1);
   }
   /* USER CODE END StartChartTask */
}

/* USER CODE BEGIN Header_StartRfpMessageTask */
/**
 * @brief Function implementing the RfpMessageTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRfpMessageTask */
void StartRfpMessageTask(void *argument)
{
   /* USER CODE BEGIN StartRfpMessageTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   RfpMessage_TypeDef RfpMessage = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(RfpMessageQueueHandle, &RfpMessage, 0, osWaitForever);
      while(Rfp.State != RFP_STATE_IDLE)
      {
         osDelay(100);
      }
      if(RfpMessage.AdditionalData == 0)
      {
         RFP_SendData(RFP_ODWS, RfpMessage.MessageType, &RfpMessage.Data, 1);
      }
      else
      {
         uint8_t Temp[3];
         Temp[0] = RfpMessage.Data;
         Temp[1] = ((RfpMessage.AdditionalData & 0xff) >> 8);
         Temp[2] = (RfpMessage.AdditionalData & 0xff);
         RFP_SendData(RFP_ODWS, RfpMessage.MessageType, Temp, 3);
      }
      while(Rfp.State != RFP_STATE_IDLE)
      {
         osDelay(100);
      }
      osDelay(1);
   }
   /* USER CODE END StartRfpMessageTask */
}

/* USER CODE BEGIN Header_StartSunriseAndSunsetTask */
/**
 * @brief Function implementing the SunriseAndSunsetTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSunriseAndSunsetTask */
void StartSunriseAndSunsetTask(void *argument)
{
   /* USER CODE BEGIN StartSunriseAndSunsetTask */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   RTC_TimeTypeDef RtcTime = { 0 };
   RTC_DateTypeDef RtcDate = { 0 };
   Sunrise_TypeDef Sunrise = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, NEW_DAY_FLAG, osFlagsWaitAny, osWaitForever);
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      Wschod(RtcDate.Year + 2000, RtcDate.Month, RtcDate.Date, &Sunrise.Sunrise, &Sunrise.Sunup, &Sunrise.Sunset);
      osMessageQueuePut(SunriseQueueHandle, &Sunrise, 0, osWaitForever);
      osMessageQueuePut(WS2812QueueHandle, &Sunrise, 0, osWaitForever);
      osDelay(1);
   }
   /* USER CODE END StartSunriseAndSunsetTask */
}

/* USER CODE BEGIN Header_StartWS2812Task */
/**
 * @brief Function implementing the WS2812Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartWS2812Task */
void StartWS2812Task(void *argument)
{
   /* USER CODE BEGIN StartWS2812Task */
   osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
   Sunrise_TypeDef Sunrise = { 0 };
   MV_TypeDef Mv           = { 0 };
   RTC_TimeTypeDef RtcTime = { 0 };
   RTC_DateTypeDef RtcDate = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osMessageQueueGet(WS2812bMeasurmentQueueHandle, &Mv, 0, 0);
      osMessageQueueGet(WS2812QueueHandle, &Sunrise, 0, 0);
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      uint32_t TempTimeSunrise = (Sunrise.Sunrise + 1.0) * 3600;
      uint32_t TempTimeSunset  = (Sunrise.Sunset + 1.0) * 3600;
      uint32_t ActualTime      = RtcTime.Hours * 3600 + RtcTime.Minutes * 60 + RtcTime.Seconds;
      uint8_t r = 0, g = 0, b = 0;
      if(ActualTime >= TempTimeSunrise && ActualTime <= TempTimeSunset)
      {
         g = 10;
         if(Mv.ExtPM10 > PM10_VERY_GOOD_TRESHOLD || Mv.ExtPM25 > PM25_VERY_GOOD_TRESHOLD)
         {
            r = 10;
         }
         if(Mv.ExtPM10 > PM10_SUFFICIENT_TRESHOLD || Mv.ExtPM25 > PM25_SUFFICIENT_TRESHOLD)
         {
            r = 10;
            g = 5;
         }
         if(Mv.ExtPM10 > PM10_VERY_BAD_TRESHOLD || Mv.ExtPM25 > PM25_VERY_BAD_TRESHOLD)
         {
            r = 10;
            g = 0;
         }
         WS2812_SetPixel(r, g, b);
      }
      else
      {
         WS2812_SetPixel(0, 0, 0);
      }
      osDelay(100);
   }
   /* USER CODE END StartWS2812Task */
}

/* MenuTimerCallback function */
void MenuTimerCallback(void *argument)
{
   /* USER CODE BEGIN MenuTimerCallback */

   /* USER CODE END MenuTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void Memory_ClearBuffer(PageVariable_TypeDef *Pv)
{
   for(int i = 0; i < 9; i++)
   {
      Pv->Record[i].ExternalHumidity    = 0;
      Pv->Record[i].ExternalPM1         = 0;
      Pv->Record[i].ExternalPM10        = 0;
      Pv->Record[i].ExternalPM25        = 0;
      Pv->Record[i].ExternalTemperature = 0;
      Pv->Record[i].Hour                = 0;
      Pv->Record[i].InternalPM1         = 0;
      Pv->Record[i].InternalPM10        = 0;
      Pv->Record[i].InternalPM25        = 0;
      Pv->Record[i].Minute              = 0;
      Pv->Record[i].Pressure            = 0;
      Pv->Record[i].Reserved            = 0;
      Pv->Record[i].Second              = 0;
      Pv->Record[i].Reserved            = 0;
   }
   Pv->PageCRC = CRC_INITIAL_VALUE;
}
static void RFP_DataFunction(uint8_t *Data, uint32_t DataLength, uint32_t DataStart)
{
   uint8_t *TempPtr;
   MV_TypeDef Mv = { 0 };
   TempPtr       = &Mv.ExtHumidity;
   *TempPtr      = Data[DataStart];
   TempPtr++;
   *TempPtr = Data[DataStart + 1];
   TempPtr++;
   *TempPtr = Data[DataStart + 2];
   TempPtr++;
   *TempPtr = Data[DataStart + 3];
   TempPtr++;
   TempPtr  = &Mv.ExtTemperature;
   *TempPtr = Data[DataStart + 5];
   TempPtr++;
   *TempPtr = Data[DataStart + 6];
   TempPtr++;
   *TempPtr = Data[DataStart + 7];
   TempPtr++;
   *TempPtr      = Data[DataStart + 8];
   Mv.Brightness = ((Data[DataStart + 10] << 8) | Data[DataStart + 11]);
   TempPtr       = &Mv.BatteryVoltage;
   *TempPtr      = Data[DataStart + 13];
   TempPtr++;
   *TempPtr = Data[DataStart + 14];
   TempPtr++;
   *TempPtr = Data[DataStart + 15];
   TempPtr++;
   *TempPtr        = Data[DataStart + 16];
   TempPtr         = NULL;
   Mv.ExtPM1       = (Data[22 + 3] | (Data[21 + 3] << 8));
   Mv.ExtPM25      = (Data[28 + 3] | (Data[27 + 3] << 8));
   Mv.ExtPM10      = (Data[25 + 3] | (Data[24 + 3] << 8));
   Mv.BatteryState = Data[DataStart + 18];
   osMessageQueuePut(ExternalMeasurmentQueueHandle, &Mv, 0, osWaitForever);
}
static double rang(double x)
{
   double A, b;
   b = x / 360;
   A = 360 * (b - (int)b);
   if(A < 0)
      A = A + 360;
   return A;
}
static double faza(double Rok, double Miesiac, double Dzien, double godzina, double min, double sec)
{
   if(Miesiac > 2)
   {
      Miesiac = Miesiac;
      Rok     = Rok;
   }
   if(Miesiac <= 2)
   {
      Miesiac = Miesiac + 12;
      Rok     = Rok - 1;
   }
   double A, b, phi1, phi2, jdp, tzd, elm, ams, aml, asd;
   A   = (int)(Rok / 100);
   b   = 2 - A + (int)(A / 4);
   jdp = (int)(365.25 * (Rok + 4716)) + (int)(30.6001 * (Miesiac + 1)) + Dzien + b + ((godzina + min / 60 + sec / 3600) / 24) - 1524.5;
   jdp = jdp;
   tzd = (jdp - 2451545) / 36525;
   elm = rang(297.8502042 + 445267.1115168 * tzd - (0.00163 * tzd * tzd) + tzd * tzd * tzd / 545868 - tzd * tzd * tzd * tzd / 113065000);
   ams = rang(357.5291092 + 35999.0502909 * tzd - 0.0001536 * tzd * tzd + tzd * tzd * tzd / 24490000);
   aml = rang(134.9634114 + 477198.8676313 * tzd - 0.008997 * tzd * tzd + tzd * tzd * tzd / 69699 - tzd * tzd * tzd * tzd / 14712000);
   asd = 180 - elm - (6.289 * sin((M_PI / 180) * ((aml)))) + (2.1 * sin((M_PI / 180) * ((ams)))) - (1.274 * sin((M_PI / 180) * (((2 * elm) - aml))))
         - (0.658 * sin((M_PI / 180) * ((2 * elm)))) - (0.214 * sin((M_PI / 180) * ((2 * aml)))) - (0.11 * sin((M_PI / 180) * ((elm))));
   phi1 = (1 + cos((M_PI / 180) * (asd))) / 2;

   tzd = (jdp + (0.5 / 24) - 2451545) / 36525;
   elm = rang(297.8502042 + 445267.1115168 * tzd - (0.00163 * tzd * tzd) + tzd * tzd * tzd / 545868 - tzd * tzd * tzd * tzd / 113065000);
   ams = rang(357.5291092 + 35999.0502909 * tzd - 0.0001536 * tzd * tzd + tzd * tzd * tzd / 24490000);
   aml = rang(134.9634114 + 477198.8676313 * tzd - 0.008997 * tzd * tzd + tzd * tzd * tzd / 69699 - tzd * tzd * tzd * tzd / 14712000);
   asd = 180 - elm - (6.289 * sin((M_PI / 180) * ((aml)))) + (2.1 * sin((M_PI / 180) * ((ams)))) - (1.274 * sin((M_PI / 180) * (((2 * elm) - aml))))
         - (0.658 * sin((M_PI / 180) * ((2 * elm)))) - (0.214 * sin((M_PI / 180) * ((2 * aml)))) - (0.11 * sin((M_PI / 180) * ((elm))));
   phi2 = (1 + cos((M_PI / 180) * (asd))) / 2;

   if((phi2 - phi1) < 0)
   {
      phi1 = -1 * phi1;
   }
   return (100 * phi1);
}
double modd(double x, double y)
{
   double il;

   il = (int32_t)(x / y);
   return (x - (il * y));
}
void Wschod(double R, double M, double D, double *Wsch, double *Tran, double *Zach)
{
   double J, Cent, L, G, O, F, E, A, C, U, UD, P, Lat, Long, Req;
   Lat  = 52.291452;
   Long = 17.363596;
   Req  = -0.833;
   J    = 367 * R - (int32_t)(7 * (R + (int32_t)((M + 9) / 12)) / 4) + (int32_t)(275 * M / 9) + D - 730531.5;
   Cent = J / 36525;
   L    = modd((4.8949504201433 + 628.331969753199 * Cent), 6.28318530718);
   G    = modd((6.2400408 + 628.3019501 * Cent), 6.28318530718);
   O    = 0.409093 - 0.0002269 * Cent;
   F    = 0.033423 * sin(G) + 0.00034907 * sin(2 * G);
   E    = 0.0430398 * sin(2 * (L + F)) - 0.00092502 * sin(4 * (L + F)) - F;
   A    = asin(sin(O) * sin(L + F));
   C    = (sin(0.017453293 * Req) - sin(0.017453293 * Lat) * sin(A)) / (cos(0.017453293 * Lat) * cos(A));

   *Wsch = (3.14159 - (E + 0.017453293 * Long + 1 * acos(C))) * 57.29577951 / 15;
   *Tran = (3.14159 - (E + 0.017453293 * Long + 0 * acos(C))) * 57.29577951 / 15;
   *Zach = (3.14159 - (E + 0.017453293 * Long + -1 * acos(C))) * 57.29577951 / 15;
}
static float mapf(float val, float in_min, float in_max, float out_min, float out_max)
{
   return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static uint32_t map(uint32_t val, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
   return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if(huart->Instance == USART1)
   {
      Rfp.DataSize = Size;
      RFP_InterruptTask();
      HAL_GPIO_TogglePin(SIGNAL_GPIO_Port, SIGNAL_Pin);
   }
   else if(huart->Instance == USART2)
   {
      if(PMSData[0] == 0x42)
      {
         PMSFlag = 1;
      }
      else
      {
         HAL_UARTEx_ReceiveToIdle_DMA(&huart2, PMSData, 100);
      }
   }
}
/* USER CODE END Application */
