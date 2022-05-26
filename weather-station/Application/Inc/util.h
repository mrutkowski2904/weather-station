#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "main.h"

char* GenerateTimeSetString(uint8_t frame_cnt, uint8_t selected_entry,
		RTC_TimeTypeDef *time_set);

char* GenerateDateSetString(uint8_t frame_cnt, uint8_t selected_entry,
		RTC_DateTypeDef *date_set);

void IncrementTimeSet(uint8_t selected_entry, RTC_TimeTypeDef *time_set);
void DecrementTimeSet(uint8_t selected_entry, RTC_TimeTypeDef *time_set);

void IncrementDateSet(uint8_t selected_entry, RTC_DateTypeDef *date_set);
void DecrementDateSet(uint8_t selected_entry, RTC_DateTypeDef *date_set);

#endif /* INC_UTIL_H_ */
