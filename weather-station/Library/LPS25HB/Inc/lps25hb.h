/*
 * lps25hb.h
 *
 *	The MIT License
 *  Created on: 30 maj 2022
 *      Author: Maciej
 */

#ifndef LPS25HB_INC_LPS25HB_H_
#define LPS25HB_INC_LPS25HB_H_

#include "stm32f3xx_hal.h"

/* Driver States */
#define LPS25HB_STATE_NOT_CONFIGURED	0x01
#define LPS25HB_STATE_CONFIGURED		0x02

/* Read States */
#define LPS25HB_READ_TEMPERATURE		0x01
#define LPS25HB_READ_PRESSURE			0x02

/* Registers */
#define LPS25HB_CTRL_REG1 				0x20
#define LPS25HB_PRESS_OUT_XL			0x28
#define LPS25HB_PRESS_OUT_L				0x29
#define LPS25HB_PRESS_OUT_H				0x2A

typedef struct {
	/* ---------- CONFIGURABLE ---------- */
	I2C_HandleTypeDef *hi2c;
	uint8_t addr;
	uint32_t *pressure_buff;

	/* ---------- INTERNAL DO NOT MODIFY ---------- */
	uint8_t state;
	uint8_t read;
	uint8_t cmd;
} LPS25HB_TypeDef;

void LPS25HB_Config(LPS25HB_TypeDef *lps25hb);
void LPS25HB_ReadPressure(LPS25HB_TypeDef *lps25hb);
void LPS25HB_ReadCpltUserCallback(LPS25HB_TypeDef *lps25hb);

/*
 * Put in
 * HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
 */
void LPS25HB_ReadCpltIRQ(LPS25HB_TypeDef *lps25hb);

/*
 * Put in
 * HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
 */
void LPS25HB_WriteCpltIRQ(LPS25HB_TypeDef *lps25hb);

#endif /* LPS25HB_INC_LPS25HB_H_ */
