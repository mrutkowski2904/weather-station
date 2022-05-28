#include "dht11.h"

uint8_t DHT11_Start_Init(DHT11_TypeDef *dht) {
	GPIO_InitTypeDef dht_pin = { 0 };
	dht_pin.Pin = dht->Pin;
	dht_pin.Pull = GPIO_NOPULL;

	dht->recieved_bits = 0;
	dht->currently_recieving = dht->RH_Integral;
	dht->state = DHT11_STATE_INIT;
	dht_pin.Mode = GPIO_MODE_OUTPUT_OD;
	HAL_GPIO_Init(dht->Port, &dht_pin);

	// Init start sequence
	HAL_GPIO_WritePin(dht->Port, dht->Pin, GPIO_PIN_RESET);
	return dht->state;
}

void DHT11_Start_End(DHT11_TypeDef *dht) {
	if (dht->state == DHT11_STATE_INIT) {
		*dht->T_Decimal = 0;
		*dht->T_Integral = 0;
		*dht->RH_Decimal = 0;
		*dht->RH_Integral = 0;

		// finish init sequence
		HAL_GPIO_WritePin(dht->Port, dht->Pin, GPIO_PIN_SET);

		// configure pin for interrupt
		GPIO_InitTypeDef dht_pin = { 0 };
		dht_pin.Pin = dht->Pin;
		dht_pin.Pull = GPIO_NOPULL;
		dht_pin.Mode = GPIO_MODE_IT_RISING;
		HAL_GPIO_Init(dht->Port, &dht_pin);

		dht->state = DHT11_STATE_ACK;

		HAL_TIM_Base_Start(dht->usTimerHandle);
	}
}

__weak void DHT11_RecieveCpltCallback(DHT11_TypeDef *dht) {
	/*
	 * Copy this callback to your code, remove __weak in copied callback
	 */
}

void DHT11_IRQHandler(DHT11_TypeDef *dht) {
	dht->recieved_bits++;

	// end of ACK sequence
	if (dht->recieved_bits == 2 && dht->state == DHT11_STATE_ACK) {
		__HAL_TIM_SET_COUNTER(dht->usTimerHandle, 0U);
		dht->state = DHT11_STATE_BUSY;
		dht->recieved_bits = 0;
		return;
	}

	if (dht->state == DHT11_STATE_BUSY) {
		uint32_t current_at = __HAL_TIM_GET_COUNTER(dht->usTimerHandle);

		// next part of data frame
		if (dht->recieved_bits == 9) {
			if (dht->currently_recieving == dht->RH_Integral) {
				dht->currently_recieving = dht->RH_Decimal;
			} else if (dht->currently_recieving == dht->RH_Decimal) {
				dht->currently_recieving = dht->T_Integral;
			} else if (dht->currently_recieving == dht->T_Integral) {
				dht->currently_recieving = dht->T_Decimal;
			} else if (dht->currently_recieving == dht->T_Decimal) {
				dht->currently_recieving = NULL;
				HAL_TIM_Base_Stop(dht->usTimerHandle);
				dht->state = DHT11_STATE_FINISHED;
				DHT11_RecieveCpltCallback(dht);
			}

			dht->recieved_bits = 0;
		}

		if (dht->currently_recieving != NULL) {
			if ((current_at - dht->last_recieved_at) > 100) {
				// recieved 1
				*dht->currently_recieving |= (1 << (8 - dht->recieved_bits));
			}
			// 0 is already written
		}
		dht->last_recieved_at = current_at;
	}
}
