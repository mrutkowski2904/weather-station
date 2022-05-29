#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "cmsis_os.h"

#include "main.h"

void SdPoweredUp(void);
void SaveToSdCard(uint32_t data);
SD_Card_Info_t* GetSdCardInfo(void);

extern RTC_HandleTypeDef hrtc;

#endif /* INC_SD_CARD_H_ */
