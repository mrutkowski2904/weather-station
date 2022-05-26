#include "ui.h"

#include <stdio.h>
#include <string.h>
#include "OLED_SSD1306.h"
#include "GFX_BW.h"

volatile uint8_t current_menu_state = UI_MAIN;

static void DrawUIMain(void);
static void DrawUITimeSet(void);
static void DrawUISdInfo(void);

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
	sprintf(text_buff, "Time set");
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
}

static void DrawUISdInfo(void) {
	char text_buff[128];
	sprintf(text_buff, "SD info");
	GFX_DrawString(0, 0, text_buff, WHITE, BLACK);
}
