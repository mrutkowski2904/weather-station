#include "util.h"

#include <stdio.h>
static char time_buff[64] = { 0 };
static char date_buff[64] = { 0 };

char* GenerateTimeSetString(uint8_t frame_cnt, uint8_t selected_entry,
		RTC_TimeTypeDef *time_set) {
	uint8_t blink = frame_cnt % 5;
	switch (selected_entry) {
	case HOUR_SETTING:
		// blink effect
		blink ? sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
						time_set->Minutes, time_set->Seconds) :
				sprintf(time_buff, "    :%02d:%02d", time_set->Minutes,
						time_set->Seconds);
		break;
	case MIN_SETTING:
		blink ? sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
						time_set->Minutes, time_set->Seconds) :
				sprintf(time_buff, "  %02d:  :%02d", time_set->Hours,
						time_set->Seconds);
		break;
	case SEC_SETTING:
		blink ? sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
						time_set->Minutes, time_set->Seconds) :
				sprintf(time_buff, "  %02d:%02d:  ", time_set->Hours,
						time_set->Minutes);
		break;
	case DAY_SETTING:
		sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
				time_set->Minutes, time_set->Seconds);
		break;
	case MONTH_SETTING:
		sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
				time_set->Minutes, time_set->Seconds);
		break;
	case YEAR_SETTING:
		sprintf(time_buff, "  %02d:%02d:%02d", time_set->Hours,
				time_set->Minutes, time_set->Seconds);
		break;
	}
	return time_buff;
}

char* GenerateDateSetString(uint8_t frame_cnt, uint8_t selected_entry,
		RTC_DateTypeDef *date_set) {
	uint8_t blink = frame_cnt % 5;

	switch (selected_entry) {
	case HOUR_SETTING:
		sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date, date_set->Month,
				date_set->Year);
		break;
	case MIN_SETTING:
		sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date, date_set->Month,
				date_set->Year);
		break;
	case SEC_SETTING:
		sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date, date_set->Month,
				date_set->Year);
		break;
	case DAY_SETTING:
		blink ? sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date,
						date_set->Month, date_set->Year) :
				sprintf(date_buff, "    -%02d-%02d", date_set->Month,
						date_set->Year);
		break;
	case MONTH_SETTING:
		blink ? sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date,
						date_set->Month, date_set->Year) :
				sprintf(date_buff, "  %02d-  -%02d", date_set->Date,
						date_set->Year);
		break;
	case YEAR_SETTING:
		blink ? sprintf(date_buff, "  %02d-%02d-%02d", date_set->Date,
						date_set->Month, date_set->Year) :
				sprintf(date_buff, "  %02d-%02d-  ", date_set->Date,
						date_set->Month);
		break;
	}
	return date_buff;
}
