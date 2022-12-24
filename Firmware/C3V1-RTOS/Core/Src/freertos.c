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
#include "MeasurmentVariable.h"
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
   .stack_size = 1024 * 4,
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
/* Definitions for MeasurmentQueue */
osMessageQueueId_t MeasurmentQueueHandle;
const osMessageQueueAttr_t MeasurmentQueue_attributes = { .name = "MeasurmentQueue" };
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
/* Definitions for C3V1Flags */
osEventFlagsId_t C3V1FlagsHandle;
const osEventFlagsAttr_t C3V1Flags_attributes = { .name = "C3V1Flags" };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void RFP_DataFunction(uint8_t *Data, uint32_t DataLength, uint32_t DataStart);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartInitAndTimeTask(void *argument);
void StartRfpTask(void *argument);
void StartMeasurmentTask(void *argument);
void StartInternalMeasurmentTask(void *argument);
void StartMoonPhaseTask(void *argument);
void StartE_PapierDrawingTask(void *argument);
void StartE_PapierDisplayTask(void *argument);

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

   /* USER CODE BEGIN RTOS_MUTEX */
   /* add mutexes, ... */
   /* USER CODE END RTOS_MUTEX */

   /* USER CODE BEGIN RTOS_SEMAPHORES */
   /* add semaphores, ... */
   /* USER CODE END RTOS_SEMAPHORES */

   /* USER CODE BEGIN RTOS_TIMERS */
   /* start timers, add new ones, ... */
   /* USER CODE END RTOS_TIMERS */

   /* Create the queue(s) */
   /* creation of MeasurmentQueue */
   MeasurmentQueueHandle = osMessageQueueNew(16, sizeof(MV_TypeDef), &MeasurmentQueue_attributes);

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
   BME280_Init(&Bme, &hspi1, BME280_CS_GPIO_Port, BME280_CS_Pin);
   GFX_SetFont(font_8x5);
   RFP_RegisterDataFunction(RFP_DataFunction);
   osEventFlagsSet(C3V1FlagsHandle, INITIALIZE_ALL_FLAG);
   uint32_t Random;
   RTC_TimeTypeDef RtcTime;
   RTC_DateTypeDef RtcDate;
   char data[100];
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, INITIALIZE_ALL_FLAG, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
      osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
      osMutexAcquire(SSD1306MutexHandle, osWaitForever);
      osMutexAcquire(SPI1MutexHandle, osWaitForever);
      HAL_RNG_GenerateRandomNumber(&hrng, &Random);
      ssd1306_clear();
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      sprintf(data, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
      GFX_DrawString(0, 0, data, WHITE, 0, OLED);
      sprintf(data, "%d : %d ; 2022", RtcDate.Date, RtcDate.Month);
      GFX_DrawString(0, 10, data, WHITE, 0, OLED);
      sprintf(data, "RNG %d ", Random);
      GFX_DrawString(0, 20, data, WHITE, 0, OLED);
      ssd1306_display();
      osMutexRelease(SPI1MutexHandle);
      osMutexRelease(SSD1306MutexHandle);
      osMutexRelease(ScreensDcMutexHandle);
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
   MV_TypeDef _Mv = { 0 };
   /* Infinite loop */
   for(;;)
   {
      osEventFlagsWait(C3V1FlagsHandle, E_PAPIER_DRAWING_FLAG, osFlagsWaitAny, osWaitForever);
      osMessageQueueGet(MeasurmentQueueHandle, &_Mv, 0, osWaitForever);
      osMutexAcquire(E_PAPIERMutexHandle, osWaitForever);
      taskENTER_CRITICAL();
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
      HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
      sprintf(mes, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
      GFX_DrawString(0, 80, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%d : %d ; 2022", RtcDate.Date, RtcDate.Month);
      GFX_DrawString(0, 90, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Internal Temperature: %0.2f", _Mv.InternalTemperature);
      GFX_DrawString(0, 210, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Internal Humidity: %0.2f", _Mv.InternalHumidity);
      GFX_DrawString(0, 200, mes, BLACK, 1, E_PAPIER);
      sprintf(mes, "%Pressure: %0.2f", _Mv.Pressure);
      GFX_DrawString(0, 220, mes, BLACK, 1, E_PAPIER);
      taskEXIT_CRITICAL();
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
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
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if(huart->Instance == USART1)
   {
      Rfp.DataSize = Size;
      RFP_InterruptTask();
   }
}
/* USER CODE END Application */
