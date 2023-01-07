/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GFX_BW.h"
#include "e_papier.h"
#include "flash_spi.h"
#include "fonts/fonts.h"
#include "fram.h"
#include "rfp.h"
#include "ssd1306_spi.h"
#include <stdio.h>
#include <stdlib.h>
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

/* USER CODE BEGIN PV */
// fram_t fram1;
// RFP_TypeDef rfp;
// flash_t flash1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void RFP_DataFunction(uint8_t *Data, uint32_t DataLength, uint32_t DataStart);
void RFP_MessageExitDeepSleep(uint8_t *Data, uint32_t DataLength, uint32_t DataStart);
float h, t, b;
uint16_t pm1, pm25, pm10;
uint8_t f = 0;
RTC_TimeTypeDef RtcTime;
RTC_DateTypeDef RtcDate;
uint8_t State;
RFP_TypeDef rfp;
fram_t fram1;
flash_t flash1;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_RNG_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
   ssd1306_init(&hspi1);
   ssd1306_clear();
   ssd1306_display();
   GFX_SetFont(font_8x5);
   GFX_DrawString(0, 50, "abcdef", WHITE, 0, OLED);
   ssd1306_display();
   e_papier_init(&hspi1);
   e_papier_display();
   ssd1306_display();
   fram_Init(&fram1, &hspi1, FRAM_HOLD_GPIO_Port, FRAM_CS_GPIO_Port, FRAM_WP_GPIO_Port, FRAM_HOLD_Pin, FRAM_CS_Pin, FRAM_WP_Pin);
   RFP_Init(&rfp, RFP_IDWS);
   flash_Init(&flash1, &hspi1, FLASH_CS_GPIO_Port, FLASH_CS_Pin);
   RFP_RegisterDataFunction(RFP_DataFunction);
   RFP_RegisterMessageFunction(RFP_EXIT_DEEP_SLEEP, RFP_MessageExitDeepSleep);
   uint8_t data = RFP_START_MEASURMENT;
   RFP_SendData(RFP_ODWS, RFP_COMMAND, &data, 1);
   HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
   uint32_t Time = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   while(1)
   {
      if(HAL_GetTick() - Time > 1000)
      {
         ssd1306_clear();
         Time = HAL_GetTick();
         HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
         HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
         char data[100];
         sprintf(data, "%d h %d m %d s", RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds);
         GFX_DrawString(0, 0, data, WHITE, 0, OLED);
         sprintf(data, "%d : %d ; 2022", RtcDate.Date, RtcDate.Month);
         GFX_DrawString(0, 10, data, WHITE, 0, OLED);
         ssd1306_display();
      }
      RFP_Handle();
      if(f == 1)
      {
         f = 0;
         char mes[100];
         sprintf(mes, "H %0.2f", h);
         GFX_DrawString(0, 0, mes, BLACK, 1, E_PAPIER);
         sprintf(mes, "T %0.2f", t);
         GFX_DrawString(0, 10, mes, BLACK, 1, E_PAPIER);
         sprintf(mes, "PM1 %d", pm1);
         GFX_DrawString(0, 20, mes, BLACK, 1, E_PAPIER);
         sprintf(mes, "PM10 %d", pm10);
         GFX_DrawString(0, 30, mes, BLACK, 1, E_PAPIER);
         sprintf(mes, "PM2,5 %d", pm25);
         GFX_DrawString(0, 40, mes, BLACK, 1, E_PAPIER);
         sprintf(mes, "Battery Level %0.2f", b);
         GFX_DrawString(0, 50, mes, BLACK, 1, E_PAPIER);
         if(!(State & 0x01))
         {
            GFX_DrawString(0, 60, "Battery Charging", BLACK, 1, E_PAPIER);
         }
         else
         {
            GFX_DrawString(0, 60, "Battery No Charging", BLACK, 1, E_PAPIER);
         }
         if(!(State & 0x02))
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
         e_papier_display();
         e_papier_clear();
         HAL_Delay(10000);
         //RFP_SendData(RFP_ODWS, RFP_COMMAND, &data, 1);
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
}

/* USER CODE BEGIN 4 */
void RFP_DataFunction(uint8_t *Data, uint32_t DataLength, uint32_t DataStart)
{
   uint8_t *TempPtr;
   TempPtr  = &h;
   *TempPtr = Data[DataStart];
   TempPtr++;
   *TempPtr = Data[DataStart + 1];
   TempPtr++;
   *TempPtr = Data[DataStart + 2];
   TempPtr++;
   *TempPtr = Data[DataStart + 3];
   TempPtr++;
   TempPtr  = &t;
   *TempPtr = Data[DataStart + 5];
   TempPtr++;
   *TempPtr = Data[DataStart + 6];
   TempPtr++;
   *TempPtr = Data[DataStart + 7];
   TempPtr++;
   *TempPtr = Data[DataStart + 8];
   TempPtr  = &b;
   *TempPtr = Data[DataStart + 13];
   TempPtr++;
   *TempPtr = Data[DataStart + 14];
   TempPtr++;
   *TempPtr = Data[DataStart + 15];
   TempPtr++;
   *TempPtr        = Data[DataStart + 16];
   TempPtr         = NULL;
   pm1             = (Data[22 + 3] | (Data[21 + 3] << 8));
   pm25            = (Data[28 + 3] | (Data[27 + 3] << 8));
   pm10            = (Data[25 + 3] | (Data[24 + 3] << 8));
   State           = Data[DataStart + 18];
   f               = 1;
   uint8_t Temp[2] = { RFP_GO_TO_DEEP_SLEEP, 5 };
   RFP_SendData(RFP_ODWS, RFP_COMMAND, Temp, 2);
}
void RFP_MessageExitDeepSleep(uint8_t *Data, uint32_t DataLength, uint32_t DataStart)
{
   uint8_t data = RFP_START_MEASURMENT;
   RFP_SendData(RFP_ODWS, RFP_COMMAND, &data, 1);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   HAL_GPIO_TogglePin(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin);
   rfp.DataSize = Size;
   RFP_InterruptTask();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
   /* User can add his own implementation to report the HAL error return state */
   __disable_irq();
   while(1)
   {
   }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
   /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
