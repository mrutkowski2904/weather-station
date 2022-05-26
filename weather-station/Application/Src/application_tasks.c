#include "application_tasks.h"

#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "fatfs.h"

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
	char *pData;

	// SD Card can be used after some time from power up
	osDelay(500);
	SdPoweredUp();

	for (;;) {
		osMessageQueueGet(sdCardWriteQueueHandle, &pData, NULL,
		osWaitForever);
		SaveToSdCard(pData);
	}
}

void StartButtonTask(void *argument) {
	uint8_t btn_pressed = 0;
	for (;;) {
		osMessageQueueGet(buttonClickQueueHandle, &btn_pressed, NULL,
		osWaitForever);
		HandleBtnClick(btn_pressed);
	}
}
