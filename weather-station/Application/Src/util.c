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

void IncrementTimeSet(uint8_t selected_entry, RTC_TimeTypeDef *time_set) {
	switch (selected_entry) {
	case HOUR_SETTING:
		time_set->Hours++;
		if (time_set->Hours == 24) {
			time_set->Hours = 0;
		}
		break;

	case MIN_SETTING:
		time_set->Minutes++;
		if (time_set->Minutes == 60) {
			time_set->Minutes = 0;
		}
		break;

	case SEC_SETTING:
		time_set->Seconds++;
		if (time_set->Seconds == 60) {
			time_set->Seconds = 0;
		}
		break;
	}
}

void DecrementTimeSet(uint8_t selected_entry, RTC_TimeTypeDef *time_set) {
	switch (selected_entry) {
	case HOUR_SETTING:
		time_set->Hours--;
		if (time_set->Hours == 0xff) {
			time_set->Hours = 23;
		}
		break;

	case MIN_SETTING:
		time_set->Minutes--;
		if (time_set->Minutes == 0xff) {
			time_set->Minutes = 59;
		}
		break;

	case SEC_SETTING:
		time_set->Seconds--;
		if (time_set->Seconds == 0xff) {
			time_set->Seconds = 59;
		}
		break;
	}
}

/*
 * TODO: Validate number of days in a month and leap year
 */
void IncrementDateSet(uint8_t selected_entry, RTC_DateTypeDef *date_set) {
	switch (selected_entry) {
	case DAY_SETTING:
		date_set->Date++;
		if (date_set->Date == 32) {
			date_set->Date = 1;
		}
		break;
	case MONTH_SETTING:
		date_set->Month++;
		if (date_set->Month == 13) {
			date_set->Month = 1;
		}
		break;
	case YEAR_SETTING:
		date_set->Year++;
		if (date_set->Year == 100) {
			date_set->Year = 0;
		}
		break;
	}
}

void DecrementDateSet(uint8_t selected_entry, RTC_DateTypeDef *date_set) {
	switch (selected_entry) {
	case DAY_SETTING:
		date_set->Date--;
		if (date_set->Date == 0) {
			date_set->Date = 31;
		}
		break;
	case MONTH_SETTING:
		date_set->Month--;
		if (date_set->Month == 0) {
			date_set->Month = 12;
		}
		break;
	case YEAR_SETTING:
		date_set->Year--;
		if (date_set->Year == 0xff) {
			date_set->Year = 99;
		}
		break;
	}
}
