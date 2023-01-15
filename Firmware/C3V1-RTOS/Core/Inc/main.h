/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SSD1306_CS_Pin GPIO_PIN_4
#define SSD1306_CS_GPIO_Port GPIOA
#define SCREENS_DC_Pin GPIO_PIN_0
#define SCREENS_DC_GPIO_Port GPIOB
#define EPAPIER_CS_Pin GPIO_PIN_1
#define EPAPIER_CS_GPIO_Port GPIOB
#define EPAPIER_BUSY_Pin GPIO_PIN_2
#define EPAPIER_BUSY_GPIO_Port GPIOB
#define EPAPIER_RST_Pin GPIO_PIN_10
#define EPAPIER_RST_GPIO_Port GPIOB
#define SSD1306_RST_Pin GPIO_PIN_11
#define SSD1306_RST_GPIO_Port GPIOB
#define HEARTBEAT_Pin GPIO_PIN_12
#define HEARTBEAT_GPIO_Port GPIOB
#define BME280_CS_Pin GPIO_PIN_13
#define BME280_CS_GPIO_Port GPIOB
#define ENCODER_SWITCH_Pin GPIO_PIN_14
#define ENCODER_SWITCH_GPIO_Port GPIOB
#define PMS3003_SET_Pin GPIO_PIN_15
#define PMS3003_SET_GPIO_Port GPIOB
#define HC12_SET_Pin GPIO_PIN_11
#define HC12_SET_GPIO_Port GPIOA
#define PMS3003_RST_Pin GPIO_PIN_12
#define PMS3003_RST_GPIO_Port GPIOA
#define SIGNAL_Pin GPIO_PIN_15
#define SIGNAL_GPIO_Port GPIOA
#define FRAM_CS_Pin GPIO_PIN_6
#define FRAM_CS_GPIO_Port GPIOB
#define FRAM_WP_Pin GPIO_PIN_7
#define FRAM_WP_GPIO_Port GPIOB
#define FRAM_HOLD_Pin GPIO_PIN_8
#define FRAM_HOLD_GPIO_Port GPIOB
#define FLASH_CS_Pin GPIO_PIN_9
#define FLASH_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
