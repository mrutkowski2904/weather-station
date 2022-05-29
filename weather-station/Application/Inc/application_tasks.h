#ifndef INC_APPLICATION_TASKS_H_
#define INC_APPLICATION_TASKS_H_

#include "main.h"
#include "ui.h"
#include "sd_card.h"

#include "cmsis_os.h"

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim7;
extern DHT11_TypeDef hdht;

extern volatile uint8_t current_menu_state;

extern osMessageQueueId_t sdCardWriteQueueHandle;
extern osMessageQueueId_t buttonClickQueueHandle;
extern osMessageQueueId_t temperatureQueueHandle;
extern osMessageQueueId_t humidityQueueHandle;
extern osMessageQueueId_t displayQueueHandle;
extern osSemaphoreId_t dht11StartSemHandle;
extern osSemaphoreId_t dataSavePendingSemHandle;

#endif /* INC_APPLICATION_TASKS_H_ */
