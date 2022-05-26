#include "ui.h"

#include <stdio.h>
#include <string.h>
#include "OLED_SSD1306.h"
#include "GFX_BW.h"

#include "sd_card.h"

volatile uint8_t current_menu_state = UI_MAIN;

static SD_Card_Info_t *sd_info = NULL;
static uint8_t setting_time = 0;

static void DrawUIMain(void);
static void DrawUITimeSet(void);
static void DrawUISdInfo(void);
static void HandleOkBtn(void);

/*
 * <--> MENU <--> TIME_SET <--> SD_INFO <-->
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

void DrawCurrentUIState(void) {
	SSD1306_Clear(BLACK);
	switch (current_menu_state) {
	case UI_MAIN:
		DrawUIMain();
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

		} else {
			NextUIState();
			sd_info = NULL;
		}
		break;
	case BUTTON_LEFT_PRESSED:
		if (setting_time) {
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

static void DrawUIMain(void) {
	char text_buff[128];
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	sprintf(text_buff, "  %02d:%02d:%02d", time.Hours, time.Minutes,
			time.Seconds);
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 16, 128, 16, WHITE);

	sprintf(text_buff, "%02d C   %02d %%", 23, 98);
	GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 36, 128, 36, WHITE);

	sprintf(text_buff, "  %d hPa", 1021);
	GFX_DrawString(0, 40, text_buff, WHITE, BLACK);
}

static void DrawUITimeSet(void) {
	char text_buff[128];
	sprintf(text_buff, "Data i Godz");
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
	GFX_DrawLine(0, 16, 128, 16, WHITE);
	if (setting_time) {

	} else {
		sprintf(text_buff, "Ustawienia");
		GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
	}
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

			sprintf(text_buff, "Wolne skt.");
			GFX_DrawString(0, 36, text_buff, WHITE, BLACK);
		} else {
			sprintf(text_buff, "Niedostepna");
			GFX_DrawString(0, 20, text_buff, WHITE, BLACK);
		}
	}
}

static void HandleOkBtn(void) {
	switch (current_menu_state) {
	case UI_SD_INFO:
		sd_info = GetSdCardInfo();
		break;
	case UI_TIME_SET:
		// DEBUG
		setting_time = !setting_time;
		break;
	}
}
