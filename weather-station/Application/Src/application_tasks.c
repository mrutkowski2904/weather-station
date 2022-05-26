#include "application_tasks.h"

#include <stdio.h>
#include <string.h>

#include "OLED_SSD1306.h"
#include "GFX_BW.h"
#include "fonts/fonts.h"

void StartDisplayTask(void *argument) {
	uint32_t os_tick;

	SSD1306_I2cInit(&hi2c1);
	GFX_SetFont(font_8x5);
	GFX_SetFontSize(2);

	for (;;) {
		if (hi2c1.hdmatx->State == HAL_DMA_STATE_READY) {
			DrawCurrentUIState();
		}

		// 20 FPS
		os_tick = osKernelGetTickCount();
		osDelayUntil(os_tick + 50U);
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
