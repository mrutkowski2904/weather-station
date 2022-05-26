#ifndef INC_APPLICATION_TASKS_H_
#define INC_APPLICATION_TASKS_H_

#include "main.h"
#include "ui.h"
#include "sd_card.h"

#include "cmsis_os.h"

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;
extern I2C_HandleTypeDef hi2c1;

extern volatile uint8_t current_menu_state;

extern osMessageQueueId_t sdCardWriteQueueHandle;
extern osMessageQueueId_t buttonClickQueueHandle;

#endif /* INC_APPLICATION_TASKS_H_ */
