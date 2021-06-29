/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

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
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOC
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA
#define ADC2_Pin GPIO_PIN_1
#define ADC2_GPIO_Port GPIOA
#define V5R_Pin GPIO_PIN_4
#define V5R_GPIO_Port GPIOA
#define V4R_Pin GPIO_PIN_5
#define V4R_GPIO_Port GPIOA
#define V3R_Pin GPIO_PIN_6
#define V3R_GPIO_Port GPIOA
#define V2R_Pin GPIO_PIN_7
#define V2R_GPIO_Port GPIOA
#define V1R_Pin GPIO_PIN_0
#define V1R_GPIO_Port GPIOB
#define SV1_Pin GPIO_PIN_1
#define SV1_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_8
#define CS_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_9
#define SCLK_GPIO_Port GPIOA
#define SDIO_Pin GPIO_PIN_10
#define SDIO_GPIO_Port GPIOA
#define VA_Pin GPIO_PIN_11
#define VA_GPIO_Port GPIOA
#define VB_Pin GPIO_PIN_12
#define VB_GPIO_Port GPIOA
#define SV2_Pin GPIO_PIN_15
#define SV2_GPIO_Port GPIOA
#define V4L_Pin GPIO_PIN_3
#define V4L_GPIO_Port GPIOB
#define V3L_Pin GPIO_PIN_4
#define V3L_GPIO_Port GPIOB
#define V1L_Pin GPIO_PIN_5
#define V1L_GPIO_Port GPIOB
#define V2L_Pin GPIO_PIN_6
#define V2L_GPIO_Port GPIOB
#define V5L_Pin GPIO_PIN_7
#define V5L_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
