/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define LED1_1_Pin GPIO_PIN_0
#define LED1_1_GPIO_Port GPIOC
#define LED1_2_Pin GPIO_PIN_1
#define LED1_2_GPIO_Port GPIOC
#define LED1_3_Pin GPIO_PIN_2
#define LED1_3_GPIO_Port GPIOC
#define LED1_4_Pin GPIO_PIN_3
#define LED1_4_GPIO_Port GPIOC
#define RED_Pin GPIO_PIN_4
#define RED_GPIO_Port GPIOA
#define YELLOW_Pin GPIO_PIN_5
#define YELLOW_GPIO_Port GPIOA
#define GREEN_Pin GPIO_PIN_6
#define GREEN_GPIO_Port GPIOA
#define LED1_5_Pin GPIO_PIN_4
#define LED1_5_GPIO_Port GPIOC
#define LED1_6_Pin GPIO_PIN_5
#define LED1_6_GPIO_Port GPIOC
#define LED2_1_Pin GPIO_PIN_0
#define LED2_1_GPIO_Port GPIOB
#define LED2_2_Pin GPIO_PIN_1
#define LED2_2_GPIO_Port GPIOB
#define LED2_3_Pin GPIO_PIN_2
#define LED2_3_GPIO_Port GPIOB
#define LED1_8_Pin GPIO_PIN_6
#define LED1_8_GPIO_Port GPIOC
#define LED1_7_Pin GPIO_PIN_7
#define LED1_7_GPIO_Port GPIOC
#define Gana1_Pin GPIO_PIN_8
#define Gana1_GPIO_Port GPIOA
#define Gana2_Pin GPIO_PIN_9
#define Gana2_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_10
#define BTN1_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_11
#define BTN2_GPIO_Port GPIOC
#define START_Pin GPIO_PIN_12
#define START_GPIO_Port GPIOC
#define LED2_4_Pin GPIO_PIN_3
#define LED2_4_GPIO_Port GPIOB
#define LED2_5_Pin GPIO_PIN_4
#define LED2_5_GPIO_Port GPIOB
#define LED2_6_Pin GPIO_PIN_5
#define LED2_6_GPIO_Port GPIOB
#define LED2_7_Pin GPIO_PIN_6
#define LED2_7_GPIO_Port GPIOB
#define LED2_8_Pin GPIO_PIN_7
#define LED2_8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
