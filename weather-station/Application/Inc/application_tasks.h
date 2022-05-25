#ifndef INC_APPLICATION_TASKS_H_
#define INC_APPLICATION_TASKS_H_

#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

extern osMessageQueueId_t sdCardWriteQueueHandle;

void StartDisplayTask(void *argument);

#endif /* INC_APPLICATION_TASKS_H_ */
