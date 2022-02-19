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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"

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
#define PWM_OUT_MTR_R_Pin LL_GPIO_PIN_5
#define PWM_OUT_MTR_R_GPIO_Port GPIOE
#define GPIO_OUT_MTR_R1_Pin LL_GPIO_PIN_6
#define GPIO_OUT_MTR_R1_GPIO_Port GPIOE
#define GPIO_OUT_MTR_R2_Pin LL_GPIO_PIN_13
#define GPIO_OUT_MTR_R2_GPIO_Port GPIOC
#define GPIO_IN_SW1_Pin LL_GPIO_PIN_0
#define GPIO_IN_SW1_GPIO_Port GPIOH
#define GPIO_IN_SW2_Pin LL_GPIO_PIN_1
#define GPIO_IN_SW2_GPIO_Port GPIOH
#define GPIO_IN_LED1_Pin LL_GPIO_PIN_0
#define GPIO_IN_LED1_GPIO_Port GPIOC
#define GPIO_IN_LED2_Pin LL_GPIO_PIN_1
#define GPIO_IN_LED2_GPIO_Port GPIOC
#define GPIO_IN_LED3_Pin LL_GPIO_PIN_2
#define GPIO_IN_LED3_GPIO_Port GPIOC
#define GPIO_IN_LED4_Pin LL_GPIO_PIN_3
#define GPIO_IN_LED4_GPIO_Port GPIOC
#define ADC_BAT_Pin LL_GPIO_PIN_0
#define ADC_BAT_GPIO_Port GPIOA
#define ADC_R_Pin LL_GPIO_PIN_1
#define ADC_R_GPIO_Port GPIOA
#define GPIO_IN_LEDR_Pin LL_GPIO_PIN_2
#define GPIO_IN_LEDR_GPIO_Port GPIOA
#define GPIO_OUT_HEAD_Pin LL_GPIO_PIN_3
#define GPIO_OUT_HEAD_GPIO_Port GPIOA
#define ADC_H_1_Pin LL_GPIO_PIN_4
#define ADC_H_1_GPIO_Port GPIOA
#define ADC_H_2_Pin LL_GPIO_PIN_5
#define ADC_H_2_GPIO_Port GPIOA
#define ADC_H_3_Pin LL_GPIO_PIN_6
#define ADC_H_3_GPIO_Port GPIOA
#define ADC_H_4_Pin LL_GPIO_PIN_7
#define ADC_H_4_GPIO_Port GPIOA
#define ADC_H_5_Pin LL_GPIO_PIN_4
#define ADC_H_5_GPIO_Port GPIOC
#define ADC_H_6_Pin LL_GPIO_PIN_5
#define ADC_H_6_GPIO_Port GPIOC
#define ADC_L_Pin LL_GPIO_PIN_1
#define ADC_L_GPIO_Port GPIOB
#define GPIO_IN_LEDL_Pin LL_GPIO_PIN_7
#define GPIO_IN_LEDL_GPIO_Port GPIOE
#define SPI_IMU_SCK_Pin LL_GPIO_PIN_13
#define SPI_IMU_SCK_GPIO_Port GPIOB
#define SPI_IMU_MISO_Pin LL_GPIO_PIN_14
#define SPI_IMU_MISO_GPIO_Port GPIOB
#define SPI_IMU_MOSI_Pin LL_GPIO_PIN_15
#define SPI_IMU_MOSI_GPIO_Port GPIOB
#define GPIO_OUT_IMU_Cs_Pin LL_GPIO_PIN_8
#define GPIO_OUT_IMU_Cs_GPIO_Port GPIOD
#define BLUETOOTH_TX_Pin LL_GPIO_PIN_6
#define BLUETOOTH_TX_GPIO_Port GPIOC
#define BLUETOOTH_RX_Pin LL_GPIO_PIN_7
#define BLUETOOTH_RX_GPIO_Port GPIOC
#define GPIO_OUT_MTR_L1_Pin LL_GPIO_PIN_9
#define GPIO_OUT_MTR_L1_GPIO_Port GPIOA
#define GPIO_OUT_MTR_L2_Pin LL_GPIO_PIN_10
#define GPIO_OUT_MTR_L2_GPIO_Port GPIOA
#define PWM_OUT_MTR_L_Pin LL_GPIO_PIN_11
#define PWM_OUT_MTR_L_GPIO_Port GPIOA
#define SPI_ENC_R_SCK_Pin LL_GPIO_PIN_10
#define SPI_ENC_R_SCK_GPIO_Port GPIOC
#define SPI_ENC_R_MISO_Pin LL_GPIO_PIN_11
#define SPI_ENC_R_MISO_GPIO_Port GPIOC
#define SPI_ENC_R_MOSI_Pin LL_GPIO_PIN_12
#define SPI_ENC_R_MOSI_GPIO_Port GPIOC
#define GPIO_OUT_ENC_R_Cs_Pin LL_GPIO_PIN_0
#define GPIO_OUT_ENC_R_Cs_GPIO_Port GPIOD
#define SPI_ENC_L_SCK_Pin LL_GPIO_PIN_3
#define SPI_ENC_L_SCK_GPIO_Port GPIOB
#define SPI_ENC_L_MISO_Pin LL_GPIO_PIN_4
#define SPI_ENC_L_MISO_GPIO_Port GPIOB
#define SPI_ENC_L_MOSI_Pin LL_GPIO_PIN_5
#define SPI_ENC_L_MOSI_GPIO_Port GPIOB
#define GPIO_OUT_ENC_L_Cs_Pin LL_GPIO_PIN_6
#define GPIO_OUT_ENC_L_Cs_GPIO_Port GPIOB
#define PWM_OUT_BUZZER_Pin LL_GPIO_PIN_9
#define PWM_OUT_BUZZER_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
