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

	uint8_t temperature = 0;
	uint8_t humidity = 0;
	uint16_t pressure = 0;

	uint32_t data_package = 0;

	SSD1306_I2cInit(&hi2c1);
	GFX_SetFont(font_8x5);
	GFX_SetFontSize(2);

	for (;;) {
		if (osMessageQueueGet(displayQueueHandle, &data_package, 0, 0)
				== osOK) {
			pressure = (uint16_t) (data_package & 0x0000ffff);
			humidity = (uint8_t) ((data_package >> 16) & 0x000000ff);
			temperature = (uint8_t) ((data_package >> 24) & 0x000000ff);
		}

		if (hi2c1.hdmatx->State == HAL_DMA_STATE_READY) {
			DrawCurrentUIState(temperature, humidity, pressure);
		}

		// 20 FPS
		os_tick = osKernelGetTickCount();
		osDelayUntil(os_tick + 50U);
	}
}

void StartSDCardTask(void *argument) {
	uint32_t data;

	// SD Card can be used after some time from power up
	osDelay(500);
	SdPoweredUp();

	for (;;) {
		osMessageQueueGet(sdCardWriteQueueHandle, &data, NULL,
		osWaitForever);
		SaveToSdCard(data);
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

void StartDataTask(void *argument) {
	uint8_t new_data = 0;
	uint8_t temperature_buff = 0;
	uint8_t humidity_buff = 0;

	uint8_t valid_temperature = 0;
	uint8_t valid_humidity = 0;
	uint16_t valid_pressure = 0;

	// temperature, humidity, pressure
	uint32_t data_package = 0;

	for (;;) {
		if (osMessageQueueGet(temperatureQueueHandle, &temperature_buff, NULL,
				0) == osOK) {
			valid_temperature = temperature_buff;
			new_data = 1;
		}
		if (osMessageQueueGet(humidityQueueHandle, &humidity_buff, NULL, 0)
				== osOK) {
			valid_humidity = humidity_buff;
			new_data = 1;
		}

		if (new_data) {
			new_data = 0;
			data_package = (valid_temperature << 24) | (valid_humidity << 16)
					| (valid_pressure);
			osMessageQueuePut(displayQueueHandle, &data_package, 0, 0);
		}

		// Save to SD card - RTC interrupt happend
		if (osSemaphoreAcquire(dataSavePendingSemHandle, 0) == osOK) {
			osMessageQueuePut(sdCardWriteQueueHandle, &data_package, 0, 0);
		}

		osDelay(100);
	}
}

void StartDHT11Task(void *argument) {
	uint8_t data[5] = { 0 };

	hdht.Pin = DHT11_Pin;
	hdht.Port = DHT11_GPIO_Port;
	hdht.usTimerHandle = &htim7;
	hdht.data = data;

	// DHT11 needs time before it can be used for the first time after power up
	osDelay(1000);

	for (;;) {
		if (osSemaphoreAcquire(dht11StartSemHandle, osWaitForever) == osOK) {
			DHT11_Start_Init(&hdht);
			osDelay(20);
			int32_t state = osKernelLock();
			DHT11_Start_End(&hdht);
			osKernelRestoreLock(state);
		}
	}
}
