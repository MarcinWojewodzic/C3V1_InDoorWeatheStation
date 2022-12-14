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
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

#include "bme280_spi.h"

#include "FlagsDefinition.h"
#include "FramOrganization.h"
#include "MeasurmentVariable.h"
#include "Menu.h"
#include "crc.h"
#include "flash_spi.h"
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
RFP_TypeDef Rfp = { 0 };
BME280_t Bme    = { 0 };
flash_t Flash   = { 0 };
fram_t Fram     = { 0 };
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
   .name       = "defaultTask",
   .stack_size = 128 * 4,
   .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for InitAndTimeTask */
osThreadId_t InitAndTimeTaskHandle;
const osThreadAttr_t InitAndTimeTask_attributes = {
   .name       = "InitAndTimeTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityRealtime7,
};
/* Definitions for RfpTask */
osThreadId_t RfpTaskHandle;
const osThreadAttr_t RfpTask_attributes = {
   .name       = "RfpTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for MeasurmentTask */
osThreadId_t MeasurmentTaskHandle;
const osThreadAttr_t MeasurmentTask_attributes = {
   .name       = "MeasurmentTask",
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for InternalMeasurmentTask */
osThreadId_t InternalMeasurmentTaskHandle;
const osThreadAttr_t InternalMeasurmentTask_attributes = {
   .name       = "InternalMeasurmentTask",
   .stack_size = 1024 * 4,
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
   .stack_size = 1024 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for E_PapierDisplayTask */
osThreadId_t E_PapierDisplayTaskHandle;
const osThreadAttr_t E_PapierDisplayTask_attributes = {
   .name       = "E_PapierDisplayTask",
   .stack_size = 256 * 4,
   .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for MenuTask */
osThreadId_t MenuTaskHandle;
const osThreadAttr_t MenuTask_attributes = {
   .name       = "MenuTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for SaveMemoryTask */
osThreadId_t SaveMemoryTaskHandle;
const osThreadAttr_t SaveMemoryTask_attributes = {
   .name       = "SaveMemoryTask",
   .stack_size = 2048 * 4,
   .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for ChartTask */
osThreadId_t ChartTaskHandle;
const osThreadAttr_t ChartTask_attributes = {
   .name       = "ChartTask",
   .stack_size = 8096 * 4,
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
   taskENTER_CRITICAL();
   RFP_Init(&Rfp, RFP_IDWS);
   e_papier_init(&hspi1);
   ssd1306_init(&hspi1);
   BME280_Init(&Bme, &hspi1, BME280_CS_GPIO_Port, BME280_CS_Pin);
   GFX_SetFont(font_8x5);
   RFP_RegisterDataFunction(RFP_DataFunction);
   MENU_Init();
   fram_Init(&Fram, &hspi1, FRAM_HOLD_GPIO_Port, FRAM_CS_GPIO_Port, FRAM_WP_GPIO_Port, FRAM_HOLD_Pin, FRAM_CS_Pin, FRAM_WP_Pin);
   flash_Init(&Flash, &hspi1, FLASH_CS_GPIO_Port, FLASH_CS_Pin);
   //   fram_ChipErase(&Fram);
   //   HAL_IWDG_Refresh(&hiwdg);
   //   flash_ChipErase(&Flash);
   taskEXIT_CRITICAL();
   osEventFlagsSet(C3V1FlagsHandle, INITIALIZE_ALL_FLAG);
   uint32_t Random;
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
      HAL_RNG_GenerateRandomNumber(&hrng, &Random);
      ssd1306_clear();
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      sprintf(data, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
      GFX_DrawString(0, 0, data, WHITE, 0, OLED);
      sprintf(data, "%d : %d ; 20%d", RtcDate.Date, RtcDate.Month, RtcDate.Year);
      GFX_DrawString(0, 10, data, WHITE, 0, OLED);
      sprintf(data, "RNG %d ", Random);
      GFX_DrawString(0, 20, data, WHITE, 0, OLED);
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
   uint32_t Time   = 10000;
   uint8_t Command = RFP_START_MEASURMENT;
   /* Infinite loop */
   for(;;)
   {
      RFP_SendData(RFP_ODWS, RFP_COMMAND, &Command, 1);
      osEventFlagsWait(C3V1FlagsHandle, SEND_MEASURMENT_COMMAND_FLAG, osFlagsWaitAny, osWaitForever);
      osDelay(Time);
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
      osMessageQueueGet(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      osMutexAcquire(BME280MutexHandle, osWaitForever);
      BME280_ReadAll(&Bme, &_Mv.InternalTemperature, &_Mv.Pressure, &_Mv.InternalHumidity);
      osMutexRelease(BME280MutexHandle);
      osMutexRelease(SPI1MutexHandle);
      osEventFlagsSet(C3V1FlagsHandle, SEND_MEASURMENT_COMMAND_FLAG);
      osMessageQueuePut(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMessageQueuePut(DataToSaveQueueHandle, &_Mv, 0, osWaitForever);
      osEventFlagsSet(C3V1FlagsHandle, E_PAPIER_DRAWING_FLAG);
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
      double PhaseMoon = 0.0;
      uint32_t Cnt     = 0;
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      for(int Hours = 0; Hours < 24; Hours++)
      {
         for(int Minutes = 0; Minutes < 60; Minutes++)
         {
            for(int Seconds = 0; Seconds < 60; Seconds++)
            {
               PhaseMoon += faza((RtcDate.Year + 2000), RtcDate.Month, RtcDate.Date, Hours, Minutes, Seconds);
               Cnt++;
               osDelay(5);
            }
         }
      }
      PhaseMoon /= (Cnt);
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
   MV_TypeDef _Mv    = { 0 };
   double _MoonPhase = 0;
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, E_PAPIER_DRAWING_FLAG, osFlagsWaitAny, osWaitForever);
      osMessageQueueGet(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      e_papier_clear();
      char mes[100];
      sprintf(mes, "H %0.2f", _Mv.ExtHumidity);
      GFX_DrawString(0, 0, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "T %0.2f", _Mv.ExtTemperature);
      GFX_DrawString(0, 10, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "PM1 %d", _Mv.ExtPM1);
      GFX_DrawString(0, 20, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "PM10 %d", _Mv.ExtPM10);
      GFX_DrawString(0, 30, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "PM2,5 %d", _Mv.ExtPM25);
      GFX_DrawString(0, 40, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "Battery Level %0.2f", _Mv.BatteryVoltage);
      GFX_DrawString(0, 50, mes, BLACK, 1, E_PAPIER);
      if(!(_Mv.BatteryState & 0x01))
      {
         GFX_DrawString(0, 60, "Battery Charging", BLACK, 1, E_PAPIER);
      }
      else
      {
         GFX_DrawString(0, 60, "Battery No Charging", BLACK, 1, E_PAPIER);
      }
      if(!(_Mv.BatteryState & 0x02))
      {
         GFX_DrawString(0, 70, "Battery Standby", BLACK, 1, E_PAPIER);
      }
      else
      {
         GFX_DrawString(0, 70, "Battery No Standby", BLACK, 1, E_PAPIER);
      }
      osMutexAcquire(RTCMutexHandle, osWaitForever);
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      osMutexRelease(RTCMutexHandle);
      sprintf(mes, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
      GFX_DrawString(0, 80, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%d : %d ; 20%d", RtcDate.Date, RtcDate.Month, RtcDate.Year);
      GFX_DrawString(0, 90, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Internal Temperature: %0.2f", _Mv.InternalTemperature);
      GFX_DrawString(0, 210, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Internal Humidity: %0.2f", _Mv.InternalHumidity);
      GFX_DrawString(0, 200, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Pressure: %0.2f", _Mv.Pressure);
      GFX_DrawString(0, 220, mes, BLACK, 1, E_PAPIER);
      osMessageQueueGet(MoonPhaseQueueHandle, &_MoonPhase, 0, 0);
      sprintf(mes, "Moon Phase: %0.2f%%", _MoonPhase);
      GFX_DrawString(0, 230, mes, BLACK, 1, E_PAPIER);
      osMutexRelease(E_PAPIERMutexHandle);
      osEventFlagsSet(C3V1FlagsHandle, E_PAPIER_DISPLAY_FLAG);
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
   float FloatingPointType[400]        = { 0 };
   uint16_t IntegerType[400]           = { 0 };
   uint8_t Hour[400]                   = { 0 };
   uint8_t Minute[400]                 = { 0 };
   ChartDateAndType_TypeDef Cda        = { 0 };
   uint8_t FramDataChartExistFlag      = 0;
   FramDateChart_TypeDef FramDateChart = { 0 };
   PageVariable_TypeDef Pv             = { 0 };
   uint16_t Y_Axis[400]                = { 0 };
   uint16_t X_Axis[400]                = { 0 };
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
                        FloatingPointType[j + k] = Pv.Record[k].Pressure;
                     }
                     else if(Cda.ChartType == EXTERNAL_TEMPERATURE)
                     {
                        FloatingPointType[j + k] = Pv.Record[k].ExternalTemperature;
                     }
                     else
                     {
                        FloatingPointType[j + k] = Pv.Record[k].ExternalHumidity;
                     }
                     Minute[j + k] = Pv.Record[k].Minute;
                     Hour[j + k]   = Pv.Record[k].Hour;
                  }
               }
            }
         }
         float _Max = 0, _Min = 10000000.0;
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            if(_Max < FloatingPointType[i])
            {
               _Max = FloatingPointType[i];
            }
            if(_Min > FloatingPointType[i] && FloatingPointType[i] != 0.0)
            {
               _Min = FloatingPointType[i];
            }
         }
         for(int i = 0; i < 400; i++)
         {
            if(FloatingPointType[i] == 0)
            {
               FloatingPointType[i] = _Min;
            }
            Y_Axis[i] = (uint16_t)mapf(FloatingPointType[i], _Min, _Max, 20.0, 250.0);
            Y_Axis[i] = 300 - Y_Axis[i];
         }
         for(int i = 0; i < 400; i++)
         {
            if(Hour[i] != 0 && Minute[i] != 0)
            {
               X_Axis[i] = map(Hour[i] * 60 + Minute[i], Hour[i] * 60, (Hour[i] + 1) * 60, 64 + (Hour[i] * 14), 64 + ((Hour[i] + 1) * 14));
            }
            else if(X_Axis[i] == 0)
            {
               X_Axis[i] = 0xffff;
            }
         }
         osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
         osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
         osMutexAcquire(SPI1MutexHandle, osWaitForever);
         if(X_Axis[0] != 0xffff && X_Axis[1] != 0xffff)
         {
            GFX_DrawLine(X_Axis[0], Y_Axis[0], X_Axis[1], Y_Axis[1], BLACK, E_PAPIER);
         }
         for(int i = 1; i < 399; i++)
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
                        IntegerType[j + k] = Pv.Record[k].InternalPM1;
                     }
                     else if(Cda.ChartType == INTERNAL_PM25)
                     {
                        IntegerType[j + k] = Pv.Record[k].InternalPM25;
                     }
                     else if(Cda.ChartType == INTERNAL_PM10)
                     {
                        IntegerType[j + k] = Pv.Record[k].InternalPM10;
                     }
                     else if(Cda.ChartType == EXTERNAL_PM1)
                     {
                        IntegerType[j + k] = Pv.Record[k].ExternalPM1;
                     }
                     else if(Cda.ChartType == EXTERNAL_PM25)
                     {
                        IntegerType[j + k] = Pv.Record[k].ExternalPM25;
                     }
                     else
                     {
                        IntegerType[j + k] = Pv.Record[k].ExternalPM10;
                     }
                     Minute[j + k] = Pv.Record[k].Minute;
                     Hour[j + k]   = Pv.Record[k].Hour;
                  }
               }
            }
         }
         uint16_t _Max = 0, _Min = 10000000;
         for(int i = 0; i < FramDateChart.Length * 9; i++)
         {
            if(_Max < IntegerType[i])
            {
               _Max = IntegerType[i];
            }
            if(_Min > IntegerType[i] && IntegerType[i] != 0.0)
            {
               _Min = IntegerType[i];
            }
         }
         for(int i = 0; i < 400; i++)
         {
            if(IntegerType[i] == 0)
            {
               IntegerType[i] = _Min;
            }
            Y_Axis[i] = (uint16_t)map(IntegerType[i], _Min, _Max, 20, 250);
            Y_Axis[i] = 300 - Y_Axis[i];
         }
         for(int i = 0; i < 400; i++)
         {
            if(Hour[i] != 0 && Minute[i] != 0)
            {
               X_Axis[i] = map(Hour[i] * 60 + Minute[i], Hour[i] * 60, (Hour[i] + 1) * 60, 64 + (Hour[i] * 14), 64 + ((Hour[i] + 1) * 14));
            }
            else if(X_Axis[i] == 0)
            {
               X_Axis[i] = 0xffff;
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
         for(int i = 1; i < 399; i++)
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
      for(int i = 0; i < 400; i++)
      {
         IntegerType[i]       = 0;
         FloatingPointType[i] = 0;
         Y_Axis[i]            = 0;
         X_Axis[i]            = 0;
         Hour[i]              = 0;
         Minute[i]            = 0;
      }
      FramDataChartExistFlag = 0;
      osDelay(1);
   }
   /* USER CODE END StartChartTask */
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
   *TempPtr = Data[DataStart + 8];
   TempPtr  = &Mv.BatteryVoltage;
   *TempPtr = Data[DataStart + 13];
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
   osMessageQueuePut(MeasurmentQueueHandle, &Mv, 0, osWaitForever);
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
   }
}
/* USER CODE END Application */
