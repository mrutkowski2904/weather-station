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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11.h"
#include "lps25hb.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct SD_Card_Info {
	uint8_t available;
	uint32_t free;
} SD_Card_Info_t;

enum TimeSetPosition {
	HOUR_SETTING = 0,
	MIN_SETTING,
	SEC_SETTING,
	DAY_SETTING,
	MONTH_SETTING,
	YEAR_SETTING
};

typedef struct {
	uint8_t temperature;
	uint8_t humidity;
	uint32_t pressure;
} DataPackage_t;

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define DEBUG_Pin GPIO_PIN_6
#define DEBUG_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_1
#define SD_CS_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_10
#define DHT11_GPIO_Port GPIOC
#define DHT11_EXTI_IRQn EXTI15_10_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BTN_RIGHT_Pin GPIO_PIN_8
#define BTN_RIGHT_GPIO_Port GPIOB
#define BTN_RIGHT_EXTI_IRQn EXTI9_5_IRQn
#define BTN_LEFT_Pin GPIO_PIN_9
#define BTN_LEFT_GPIO_Port GPIOB
#define BTN_LEFT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE 			hspi2
#define DEBOUNCE_MS 			150

#define BUTTON_LEFT_PRESSED		0x01
#define BUTTON_RIGHT_PRESSED	0x02
#define BUTTON_OK_PRESSED		0x04

#define LPS25HB_I2C_ADDR		0xBA
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
