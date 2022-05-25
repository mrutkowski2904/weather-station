#include "application_tasks.h"

#include <stdio.h>
#include <string.h>

void StartDisplayTask(void *argument) {
	for (;;) {
		osThreadYield();
	}
}

void StartSDCardTask(void *argument) {
	static uint8_t sd_available = 1;

	char *pData;
	uint8_t data_len;

	char filename[64];
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	// SD Card can be used after some time from power up
	osDelay(1000);

	for (;;) {
		osMessageQueueGet(sdCardWriteQueueHandle, &pData, NULL,
		osWaitForever);

		if (sd_available) {
			FATFS FatFs;
			FIL fil;
			FRESULT fres;
			UINT wroteBytes;

			data_len = strlen(pData);

			HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

			int32_t state = osKernelLock();
			fres = f_mount(&FatFs, "", 1);
			if (fres != FR_OK) {
				sd_available = 0;
				osKernelRestoreLock(state);
				continue;
			}

			HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
			sprintf(filename, "%02d-%02d.csv", date.Date, date.Month);

			fres = f_open(&fil, filename,
			FA_WRITE | FA_OPEN_ALWAYS);
			if (fres != FR_OK) {
				sd_available = 0;
				osKernelRestoreLock(state);
				continue;
			}
			f_lseek(&fil, f_size(&fil));

			fres = f_write(&fil, pData, data_len, &wroteBytes);
			if (fres != FR_OK) {
				sd_available = 0;
				osKernelRestoreLock(state);
				continue;
			}

			f_close(&fil);
			f_mount(NULL, "", 0);
			osKernelRestoreLock(state);
			HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
		}

	}
}
