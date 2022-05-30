#include "ui.h"

#include <stdio.h>
#include <string.h>
#include "OLED_SSD1306.h"
#include "GFX_BW.h"

#include "sd_card.h"
#include "util.h"

volatile uint8_t current_menu_state = UI_MAIN;

static SD_Card_Info_t *sd_info = NULL;
static uint8_t setting_time = 0;
static uint8_t current_pos_time_set = HOUR_SETTING;

static RTC_TimeTypeDef time_set;
static RTC_DateTypeDef date_set;

static void DrawUIMain(uint8_t temperature, uint8_t humidity, uint32_t pressure);
static void DrawUITimeSet(void);
static void DrawUISdInfo(void);
static void HandleOkBtn(void);
static void TimeSetBtnRight(void);
static void TimeSetBtnLeft(void);

/*
 * <--> MAIN SCREEN <--> TIME_SET <--> SD_INFO <-->
 */
void NextUIState(void) {
	switch (current_menu_state) {
	case UI_MAIN:
		current_menu_state = UI_TIME_SET;
		break;
	case UI_TIME_SET:
		current_menu_state = UI_SD_INFO;
		break;
	case UI_SD_INFO:
		current_menu_state = UI_MAIN;
		break;
	default:
		current_menu_state = UI_MAIN;
		break;
	}
}

void PrevUIState(void) {
	switch (current_menu_state) {
	case UI_MAIN:
		current_menu_state = UI_SD_INFO;
		break;
	case UI_TIME_SET:
		current_menu_state = UI_MAIN;
		break;
	case UI_SD_INFO:
		current_menu_state = UI_TIME_SET;
		break;
	default:
		current_menu_state = UI_MAIN;
		break;
	}
}

void DrawCurrentUIState(uint8_t temperature, uint8_t humidity,
		uint32_t pressure) {
	SSD1306_Clear(BLACK);
	switch (current_menu_state) {
	case UI_MAIN:
		DrawUIMain(temperature, humidity, pressure);
		break;
	case UI_TIME_SET:
		DrawUITimeSet();
		break;
	case UI_SD_INFO:
		DrawUISdInfo();
		break;
	}
	SSD1306_Display();
}

void HandleBtnClick(uint8_t btn_pressed) {
	switch (btn_pressed) {
	case BUTTON_RIGHT_PRESSED:
		if (setting_time) {
			TimeSetBtnRight();
		} else {
			NextUIState();
			sd_info = NULL;
		}
		break;
	case BUTTON_LEFT_PRESSED:
		if (setting_time) {
			TimeSetBtnLeft();
		} else {
			PrevUIState();
			sd_info = NULL;
		}
		break;
	case BUTTON_OK_PRESSED:
		HandleOkBtn();
		break;
	}
}

static void DrawUIMain(uint8_t temperature, uint8_t humidity, uint32_t pressure) {
	char text_buff[128];
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	sprintf(text_buff, "  %02d:%02d:%02d", time.Hours, time.Minutes,
			time.Seconds);
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 16, 128, 16, WHITE);

	temperature ?
			sprintf(text_buff, "%d C   %d %%", temperature, humidity) :
			sprintf(text_buff, "-- C   -- %%");

	GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 36, 128, 36, WHITE);

	pressure ?
			sprintf(text_buff, "  %ld hPa", pressure) :
			sprintf(text_buff, "  ---- hPa");
	GFX_DrawString(0, 40, text_buff, WHITE, BLACK);
}

static void DrawUITimeSet(void) {
	char *timeString = NULL;
	char *dateString = NULL;
	char text_buff[64];
	static uint8_t frame_cnt = 0;

	sprintf(text_buff, "Data i Godz");
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 16, 128, 16, WHITE);

	if (setting_time) {
		timeString = GenerateTimeSetString(frame_cnt, current_pos_time_set,
				&time_set);
		GFX_DrawString(0, 20, timeString, WHITE, BLACK);

		dateString = GenerateDateSetString(frame_cnt, current_pos_time_set,
				&date_set);
		GFX_DrawString(0, 40, dateString, WHITE, BLACK);
	} else {

		sprintf(text_buff, "Ustawienia");
		GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
	}
	frame_cnt++;
}

static void DrawUISdInfo(void) {
	char text_buff[128];
	sprintf(text_buff, "Karta SD");
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 16, 128, 16, WHITE);

	if (sd_info == NULL) {
		sprintf(text_buff, "Nacisnij OK");
		GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
	} else {
		if (sd_info->available) {
			sprintf(text_buff, "%ld", sd_info->free);
			GFX_DrawString(0, 20, text_buff, WHITE, BLACK);

			sprintf(text_buff, "Wolne sekt.");
			GFX_DrawString(0, 36, text_buff, WHITE, BLACK);
		} else {
			sprintf(text_buff, "Niedostepna");
			GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
		}
	}
}

static void TimeSetBtnRight(void) {
	if (current_pos_time_set == HOUR_SETTING
			|| current_pos_time_set == MIN_SETTING
			|| current_pos_time_set == SEC_SETTING) {
		IncrementTimeSet(current_pos_time_set, &time_set);
	} else if (current_pos_time_set == DAY_SETTING
			|| current_pos_time_set == MONTH_SETTING
			|| current_pos_time_set == YEAR_SETTING) {
		IncrementDateSet(current_pos_time_set, &date_set);
	}
}

static void TimeSetBtnLeft(void) {
	if (current_pos_time_set == HOUR_SETTING
			|| current_pos_time_set == MIN_SETTING
			|| current_pos_time_set == SEC_SETTING) {
		DecrementTimeSet(current_pos_time_set, &time_set);
	} else if (current_pos_time_set == DAY_SETTING
			|| current_pos_time_set == MONTH_SETTING
			|| current_pos_time_set == YEAR_SETTING) {
		DecrementDateSet(current_pos_time_set, &date_set);
	}
}

static void HandleOkBtn(void) {
	switch (current_menu_state) {
	case UI_SD_INFO:
		sd_info = GetSdCardInfo();
		break;
	case UI_TIME_SET:
		if (setting_time) {
			switch (current_pos_time_set) {
			case HOUR_SETTING:
				current_pos_time_set = MIN_SETTING;
				break;
			case MIN_SETTING:
				current_pos_time_set = SEC_SETTING;
				break;
			case SEC_SETTING:
				current_pos_time_set = DAY_SETTING;
				break;
			case DAY_SETTING:
				current_pos_time_set = MONTH_SETTING;
				break;
			case MONTH_SETTING:
				current_pos_time_set = YEAR_SETTING;
				break;
			case YEAR_SETTING:
				current_pos_time_set = HOUR_SETTING;
				setting_time = 0;
				HAL_RTC_SetTime(&hrtc, &time_set, RTC_FORMAT_BIN);
				HAL_RTC_SetDate(&hrtc, &date_set, RTC_FORMAT_BIN);
				break;
			}
		} else {
			setting_time = 1;

			HAL_RTC_GetTime(&hrtc, &time_set, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &date_set, RTC_FORMAT_BIN);
		}
		break;
	}
}
