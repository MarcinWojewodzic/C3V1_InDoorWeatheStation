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
#include <stdio.h>
#include <stdlib.h>

#include "GFX_BW.h"
#include "e_papier.h"
#include "flash_spi.h"
#include "fonts/fonts.h"
#include "fram.h"
#include "rfp.h"
#include "ssd1306_spi.h"

#include "dma.h"
#include "gpio.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
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
RFP_TypeDef Rfp;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
   .name       = "defaultTask",
   .stack_size = 128 * 4,
   .priority   = (osPriority_t)osPriorityNormal,
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
/* Definitions for ScreensDcMutex */
osMutexId_t ScreensDcMutexHandle;
const osMutexAttr_t ScreensDcMutex_attributes = { .name = "ScreensDcMutex" };
/* Definitions for SSD1306Mutex */
osMutexId_t SSD1306MutexHandle;
const osMutexAttr_t SSD1306Mutex_attributes = { .name = "SSD1306Mutex" };
/* Definitions for C3V1Flags */
osEventFlagsId_t C3V1FlagsHandle;
const osEventFlagsAttr_t C3V1Flags_attributes = { .name = "C3V1Flags" };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartInitAndTimeTask(void *argument);
void StartRfpTask(void *argument);

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

   /* USER CODE BEGIN RTOS_MUTEX */
   /* add mutexes, ... */
   /* USER CODE END RTOS_MUTEX */

   /* USER CODE BEGIN RTOS_SEMAPHORES */
   /* add semaphores, ... */
   /* USER CODE END RTOS_SEMAPHORES */

   /* USER CODE BEGIN RTOS_TIMERS */
   /* start timers, add new ones, ... */
   /* USER CODE END RTOS_TIMERS */

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
      osDelay(1);
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
   GFX_SetFont(font_8x5);
   int Random;
   RTC_TimeTypeDef RtcTime;
   RTC_DateTypeDef RtcDate;
   char data[100];
   /* Infinite loop */
   for(;;)
   {
      osMutexAcquire(ScreensDcMutexHandle, osWaitForever);
      osMutexAcquire(SSD1306MutexHandle, osWaitForever);
      // TODO
      HAL_RNG_GenerateRandomNumber(&hrng, (uint32_t*)&Random);
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
