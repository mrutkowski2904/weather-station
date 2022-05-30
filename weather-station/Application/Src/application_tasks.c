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
	uint32_t pressure = 0;

	DataPackage_t data_package;

	SSD1306_I2cInit(&hi2c1);
	GFX_SetFont(font_8x5);
	GFX_SetFontSize(2);

	for (;;) {
		if (osMessageQueueGet(displayQueueHandle, &data_package, 0, 0)
				== osOK) {
			pressure = data_package.pressure;
			humidity = data_package.humidity;
			temperature = data_package.temperature;
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
	DataPackage_t data;

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
	uint32_t pressure_buff = 0;

	uint8_t valid_temperature = 0;
	uint8_t valid_humidity = 0;
	uint32_t valid_pressure = 0;

	// temperature, humidity, pressure
	DataPackage_t data_package;

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
		if (osMessageQueueGet(pressureQueueHandle, &pressure_buff, NULL, 0)
				== osOK) {
			valid_pressure = pressure_buff;
			new_data = 1;
		}

		if (new_data) {
			new_data = 0;

			data_package.humidity = valid_humidity;
			data_package.pressure = valid_pressure;
			data_package.temperature = valid_temperature;

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

void StartLPS25HBTask(void *argument) {
	uint32_t pressure_buff;

	hlps25hb.addr = LPS25HB_I2C_ADDR;
	hlps25hb.pressure_buff = &pressure_buff;
	hlps25hb.hi2c = &hi2c2;

	LPS25HB_Config(&hlps25hb);
	osDelay(1200);

	for (;;) {
		if (osSemaphoreAcquire(lps25hbStartSemHandle, osWaitForever) == osOK) {
			LPS25HB_ReadPressure(&hlps25hb);
		}
	}
}
