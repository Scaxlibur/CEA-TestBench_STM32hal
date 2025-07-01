/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define MISO_Pin GPIO_PIN_0
#define MISO_GPIO_Port GPIOA
#define CONVST_Pin GPIO_PIN_1
#define CONVST_GPIO_Port GPIOA
#define RANGE_Pin GPIO_PIN_2
#define RANGE_GPIO_Port GPIOA
#define Change_Pin GPIO_PIN_5
#define Change_GPIO_Port GPIOA
#define OS0_Pin GPIO_PIN_4
#define OS0_GPIO_Port GPIOC
#define OS1_Pin GPIO_PIN_5
#define OS1_GPIO_Port GPIOC
#define OS2_Pin GPIO_PIN_6
#define OS2_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_10
#define RST_GPIO_Port GPIOC
#define RD_Pin GPIO_PIN_11
#define RD_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_12
#define CS_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
