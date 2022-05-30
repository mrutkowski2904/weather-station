#include "lps25hb.h"

#include <string.h>

// Power ON, measure once per second, update register after read
#define LPS25HB_ENABLE_MASK		( (1 << 7) | (1 << 4) | (1 << 2) )

void LPS25HB_Config(LPS25HB_TypeDef *lps25hb) {
	lps25hb->cmd = 0;
	lps25hb->cmd |= LPS25HB_ENABLE_MASK;
	lps25hb->state = LPS25HB_STATE_NOT_CONFIGURED;
	HAL_I2C_Mem_Write_DMA(lps25hb->hi2c, lps25hb->addr, LPS25HB_CTRL_REG1, 1,
			&lps25hb->cmd, 1);
}

void LPS25HB_ReadPressure(LPS25HB_TypeDef *lps25hb) {
	lps25hb->read = LPS25HB_READ_PRESSURE;
	*lps25hb->pressure_buff = 0;
	HAL_I2C_Mem_Read_DMA(lps25hb->hi2c, lps25hb->addr,
	LPS25HB_PRESS_OUT_XL | 0x80, 1, (uint8_t*) lps25hb->pressure_buff, 3);
}

void LPS25HB_ReadCpltIRQ(LPS25HB_TypeDef *lps25hb) {
	if (lps25hb->state != LPS25HB_STATE_CONFIGURED) {
		return;
	}

	if (lps25hb->read == LPS25HB_READ_PRESSURE) {
		LPS25HB_ReadCpltUserCallback(lps25hb);
	} else if (lps25hb->read == LPS25HB_READ_TEMPERATURE) {
		LPS25HB_ReadCpltUserCallback(lps25hb);
	}
}

void LPS25HB_WriteCpltIRQ(LPS25HB_TypeDef *lps25hb) {
	if (lps25hb->state == LPS25HB_STATE_NOT_CONFIGURED) {
		lps25hb->state = LPS25HB_STATE_CONFIGURED;
	}
}

__weak void LPS25HB_ReadCpltUserCallback(LPS25HB_TypeDef *lps25hb) {

}
