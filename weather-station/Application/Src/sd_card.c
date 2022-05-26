#include "sd_card.h"

#include <stdio.h>
#include <string.h>

#include "cmsis_os.h"
#include "fatfs.h"

static uint8_t sd_available = 0;
static SD_Card_Info_t sd_info = { 0 };

void SaveToSdCard(char *data) {
	uint8_t data_len;
	char filename[64];
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	if (sd_available) {
		FATFS FatFs;
		FIL fil;
		FRESULT fres;
		UINT wroteBytes;

		data_len = strlen(data);

		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

		int32_t state = osKernelLock();
		fres = f_mount(&FatFs, "", 1);
		if (fres != FR_OK) {
			sd_available = 0;
			osKernelRestoreLock(state);
			return;
		}

		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		sprintf(filename, "%02d-%02d.csv", date.Date, date.Month);

		fres = f_open(&fil, filename, FA_WRITE | FA_OPEN_ALWAYS);
		if (fres != FR_OK) {
			sd_available = 0;
			f_mount(NULL, "", 0);
			osKernelRestoreLock(state);
			return;
		}
		f_lseek(&fil, f_size(&fil));

		fres = f_write(&fil, data, data_len, &wroteBytes);
		if (fres != FR_OK) {
			sd_available = 0;
			f_close(&fil);
			f_mount(NULL, "", 0);
			osKernelRestoreLock(state);
			return;
		}

		f_close(&fil);
		f_mount(NULL, "", 0);
		osKernelRestoreLock(state);
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
	}
}

SD_Card_Info_t* GetSdCardInfo(void) {
	FATFS fs;
	FATFS *freeFs;
	FRESULT fres;
	DWORD free_clusters, free_sectors;

	if (sd_available) {
		int32_t state = osKernelLock();
		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

		fres = f_mount(&fs, "", 1);
		if (fres == FR_OK) {
			f_getfree("", &free_clusters, &freeFs);

			free_sectors = free_clusters * freeFs->csize;
			sd_info.free = free_sectors;

			f_mount(NULL, "", 0);
		} else {
			sd_available = 0;
		}

		HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
		osKernelRestoreLock(state);
	}

	sd_info.available = sd_available;
	return &sd_info;
}

void SdPoweredUp(void) {
	sd_available = 1;
}
