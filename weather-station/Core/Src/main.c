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
#include "cmsis_os.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application_tasks.h"
#include "ui.h"
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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c2_tx;
DMA_HandleTypeDef hdma_i2c2_rx;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* Definitions for displayTask */
osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = { .name = "displayTask",
		.stack_size = 768 * 4, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for sdcardTask */
osThreadId_t sdcardTaskHandle;
const osThreadAttr_t sdcardTask_attributes = { .name = "sdcardTask",
		.stack_size = 768 * 4, .priority = (osPriority_t) osPriorityRealtime, };
/* Definitions for buttonTask */
osThreadId_t buttonTaskHandle;
const osThreadAttr_t buttonTask_attributes = { .name = "buttonTask",
		.stack_size = 200 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for dht11Task */
osThreadId_t dht11TaskHandle;
const osThreadAttr_t dht11Task_attributes = { .name = "dht11Task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for dataTask */
osThreadId_t dataTaskHandle;
const osThreadAttr_t dataTask_attributes = { .name = "dataTask", .stack_size =
		200 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for lps25hbTask */
osThreadId_t lps25hbTaskHandle;
const osThreadAttr_t lps25hbTask_attributes = { .name = "lps25hbTask",
		.stack_size = 200 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for sdCardWriteQueue */
osMessageQueueId_t sdCardWriteQueueHandle;
const osMessageQueueAttr_t sdCardWriteQueue_attributes = { .name =
		"sdCardWriteQueue" };
/* Definitions for buttonClickQueue */
osMessageQueueId_t buttonClickQueueHandle;
const osMessageQueueAttr_t buttonClickQueue_attributes = { .name =
		"buttonClickQueue" };
/* Definitions for temperatureQueue */
osMessageQueueId_t temperatureQueueHandle;
const osMessageQueueAttr_t temperatureQueue_attributes = { .name =
		"temperatureQueue" };
/* Definitions for humidityQueue */
osMessageQueueId_t humidityQueueHandle;
const osMessageQueueAttr_t humidityQueue_attributes =
		{ .name = "humidityQueue" };
/* Definitions for displayQueue */
osMessageQueueId_t displayQueueHandle;
const osMessageQueueAttr_t displayQueue_attributes = { .name = "displayQueue" };
/* Definitions for pressureQueue */
osMessageQueueId_t pressureQueueHandle;
const osMessageQueueAttr_t pressureQueue_attributes =
		{ .name = "pressureQueue" };
/* Definitions for sensorTimer */
osTimerId_t sensorTimerHandle;
const osTimerAttr_t sensorTimer_attributes = { .name = "sensorTimer" };
/* Definitions for dht11StartSem */
osSemaphoreId_t dht11StartSemHandle;
const osSemaphoreAttr_t dht11StartSem_attributes = { .name = "dht11StartSem" };
/* Definitions for dataSavePendingSem */
osSemaphoreId_t dataSavePendingSemHandle;
const osSemaphoreAttr_t dataSavePendingSem_attributes = { .name =
		"dataSavePendingSem" };
/* Definitions for lps25hbStartSem */
osSemaphoreId_t lps25hbStartSemHandle;
const osSemaphoreAttr_t lps25hbStartSem_attributes =
		{ .name = "lps25hbStartSem" };
/* USER CODE BEGIN PV */

DHT11_TypeDef hdht;
LPS25HB_TypeDef hlps25hb;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM7_Init(void);
static void MX_I2C2_Init(void);
void StartDisplayTask(void *argument);
void StartSDCardTask(void *argument);
void StartButtonTask(void *argument);
void StartDHT11Task(void *argument);
void StartDataTask(void *argument);
void StartLPS25HBTask(void *argument);
void SensorTimerCallback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_USART2_UART_Init();
	MX_SPI2_Init();
	MX_FATFS_Init();
	MX_RTC_Init();
	MX_I2C1_Init();
	MX_TIM7_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* Create the semaphores(s) */
	/* creation of dht11StartSem */
	dht11StartSemHandle = osSemaphoreNew(1, 1, &dht11StartSem_attributes);

	/* creation of dataSavePendingSem */
	dataSavePendingSemHandle = osSemaphoreNew(1, 1,
			&dataSavePendingSem_attributes);

	/* creation of lps25hbStartSem */
	lps25hbStartSemHandle = osSemaphoreNew(1, 1, &lps25hbStartSem_attributes);

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* Create the timer(s) */
	/* creation of sensorTimer */
	sensorTimerHandle = osTimerNew(SensorTimerCallback, osTimerPeriodic, NULL,
			&sensorTimer_attributes);

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	osTimerStart(sensorTimerHandle, 3000);
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of sdCardWriteQueue */
	sdCardWriteQueueHandle = osMessageQueueNew(8, sizeof(DataPackage_t),
			&sdCardWriteQueue_attributes);

	/* creation of buttonClickQueue */
	buttonClickQueueHandle = osMessageQueueNew(16, sizeof(uint8_t),
			&buttonClickQueue_attributes);

	/* creation of temperatureQueue */
	temperatureQueueHandle = osMessageQueueNew(16, sizeof(uint8_t),
			&temperatureQueue_attributes);

	/* creation of humidityQueue */
	humidityQueueHandle = osMessageQueueNew(16, sizeof(uint8_t),
			&humidityQueue_attributes);

	/* creation of displayQueue */
	displayQueueHandle = osMessageQueueNew(8, sizeof(DataPackage_t),
			&displayQueue_attributes);

	/* creation of pressureQueue */
	pressureQueueHandle = osMessageQueueNew(8, sizeof(uint32_t),
			&pressureQueue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of displayTask */
	displayTaskHandle = osThreadNew(StartDisplayTask, NULL,
			&displayTask_attributes);

	/* creation of sdcardTask */
	sdcardTaskHandle = osThreadNew(StartSDCardTask, NULL,
			&sdcardTask_attributes);

	/* creation of buttonTask */
	buttonTaskHandle = osThreadNew(StartButtonTask, NULL,
			&buttonTask_attributes);

	/* creation of dht11Task */
	dht11TaskHandle = osThreadNew(StartDHT11Task, NULL, &dht11Task_attributes);

	/* creation of dataTask */
	dataTaskHandle = osThreadNew(StartDataTask, NULL, &dataTask_attributes);

	/* creation of lps25hbTask */
	lps25hbTaskHandle = osThreadNew(StartLPS25HBTask, NULL,
			&lps25hbTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2
			| RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_HSI;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x0000020B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x0000020B;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */
	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	RTC_AlarmTypeDef sAlarm = { 0 };

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN Check_RTC_BKUP */

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0;
	sTime.Minutes = 29;
	sTime.Seconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = RTC_MONTH_MAY;
	sDate.Date = 29;
	sDate.Year = 22;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	/** Enable the Alarm A
	 */
	sAlarm.AlarmTime.Hours = 0;
	sAlarm.AlarmTime.Minutes = 30;
	sAlarm.AlarmTime.Seconds = 0;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 7;
	hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void) {

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 72 - 1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 0xffff - 1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	/* DMA1_Channel6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	/* DMA1_Channel7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin | DEBUG_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin = LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : DEBUG_Pin */
	GPIO_InitStruct.Pin = DEBUG_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DEBUG_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SD_CS_Pin */
	GPIO_InitStruct.Pin = SD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : DHT11_Pin */
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : BTN_RIGHT_Pin BTN_LEFT_Pin */
	GPIO_InitStruct.Pin = BTN_RIGHT_Pin | BTN_LEFT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) {

	}
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c == &hi2c2) {
		LPS25HB_WriteCpltIRQ(&hlps25hb);
	}
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c == &hi2c2) {
		LPS25HB_ReadCpltIRQ(&hlps25hb);
	}
}

void ProcessClicksDebounced(uint16_t pin) {
	uint8_t btn_pressed = 0;

	if (pin == B1_Pin) {
		btn_pressed = BUTTON_OK_PRESSED;
		osMessageQueuePut(buttonClickQueueHandle, &btn_pressed, 0U, 0U);
	} else if (pin == BTN_LEFT_Pin) {
		btn_pressed = BUTTON_LEFT_PRESSED;
		osMessageQueuePut(buttonClickQueueHandle, &btn_pressed, 0U, 0U);
	} else if (pin == BTN_RIGHT_Pin) {
		btn_pressed = BUTTON_RIGHT_PRESSED;
		osMessageQueuePut(buttonClickQueueHandle, &btn_pressed, 0U, 0U);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	static uint32_t last_click = 0;
	uint32_t curr_click = HAL_GetTick();

	if (GPIO_Pin == DHT11_Pin) {
		DHT11_IRQHandler(&hdht);
	}

	// no overflow
	if (last_click < curr_click && curr_click - last_click > DEBOUNCE_MS) {
		ProcessClicksDebounced(GPIO_Pin);
	}
	// overflow
	else if (last_click > curr_click && last_click - curr_click > DEBOUNCE_MS) {
		ProcessClicksDebounced(GPIO_Pin);
	}

	last_click = HAL_GetTick();
}

void DHT11_RecieveCpltCallback(DHT11_TypeDef *dht) {
	uint8_t checksum = dht->data[4];
	uint8_t t_decimal = dht->data[3];
	uint8_t t_integral = dht->data[2];
	uint8_t rh_integral = dht->data[0];
	// rh decimal always 0 on dht11

	if (checksum == (t_decimal + t_integral + rh_integral)) {
		osMessageQueuePut(temperatureQueueHandle, &t_integral, 0, 0);
		osMessageQueuePut(humidityQueueHandle, &rh_integral, 0, 0);
	}
}

void LPS25HB_ReadCpltUserCallback(LPS25HB_TypeDef *lps25hb) {
	uint32_t valid_pressure = 0;

	if (lps25hb->read == LPS25HB_READ_PRESSURE) {
		valid_pressure = *lps25hb->pressure_buff / 4096;
		osMessageQueuePut(pressureQueueHandle, &valid_pressure, 0, 0);
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	osSemaphoreRelease(dataSavePendingSemHandle);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDisplayTask */
/**
 * @brief  Function implementing the displayTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDisplayTask */
__weak void StartDisplayTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSDCardTask */
/**
 * @brief Function implementing the sdcardTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSDCardTask */
__weak void StartSDCardTask(void *argument) {
	/* USER CODE BEGIN StartSDCardTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartSDCardTask */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
 * @brief Function implementing the buttonTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartButtonTask */
__weak void StartButtonTask(void *argument) {
	/* USER CODE BEGIN StartButtonTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartButtonTask */
}

/* USER CODE BEGIN Header_StartDHT11Task */
/**
 * @brief Function implementing the dht11Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDHT11Task */
__weak void StartDHT11Task(void *argument) {
	/* USER CODE BEGIN StartDHT11Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartDHT11Task */
}

/* USER CODE BEGIN Header_StartDataTask */
/**
 * @brief Function implementing the dataTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDataTask */
__weak void StartDataTask(void *argument) {
	/* USER CODE BEGIN StartDataTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartDataTask */
}

/* USER CODE BEGIN Header_StartLPS25HBTask */
/**
 * @brief Function implementing the lps25hbTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLPS25HBTask */
__weak void StartLPS25HBTask(void *argument) {
	/* USER CODE BEGIN StartLPS25HBTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartLPS25HBTask */
}

/* SensorTimerCallback function */
void SensorTimerCallback(void *argument) {
	/* USER CODE BEGIN SensorTimerCallback */
	osSemaphoreRelease(dht11StartSemHandle);
	osSemaphoreRelease(lps25hbStartSemHandle);
	/* USER CODE END SensorTimerCallback */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
