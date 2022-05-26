#ifndef INC_UI_H_
#define INC_UI_H_

#include "main.h"

extern RTC_HandleTypeDef hrtc;

enum UIStates {
	UI_MAIN = 0, UI_TIME_SET, UI_SD_INFO
};

void NextUIState(void);
void PrevUIState(void);
void DrawCurrentUIState(void);

#endif /* INC_UI_H_ */
