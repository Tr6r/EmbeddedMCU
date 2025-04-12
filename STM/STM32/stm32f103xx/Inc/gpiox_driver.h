/*
 * gpiox_driver.h
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */

#ifndef GPIOX_DRIVER_H_
#define GPIOX_DRIVER_H_

#include "stm32f103xx.h"


typedef enum {
	GPIO_MODE_INPUT = 0x00,
	GPIO_MODE_OUTPUT_10MHz = 0x01,
	GPIO_MODE_OUTPUT_2MHz = 0x10,
	GPIO_MODE_OUTPUT_50MHz = 0x11,
} GPIO_Mode_t;
typedef enum {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1 = 1,
	GPIO_PIN_2 = 2,
	GPIO_PIN_3 = 3,
	GPIO_PIN_4 = 4,
	GPIO_PIN_5 = 5,
	GPIO_PIN_6 = 6,
	GPIO_PIN_7 = 7,
	GPIO_PIN_8 = 8,
	GPIO_PIN_9 = 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15
} GPIO_Pin_t;

typedef enum {
	GPIO_CNF_INPUT_ANALOG = 0x00,      // Analog input
	GPIO_CNF_INPUT_FLOATING = 0x01,    // Floating input
	GPIO_CNF_INPUT_PU_PD = 0x02,
	GPIO_CNF_OUTPUT_PP = 0x00,         // Push-Pull output
	GPIO_CNF_OUTPUT_OD = 0x01,         // Open-Drain output
	GPIO_CNF_AF_PP = 0x10,             // Alternate function Push-Pull output
	GPIO_CNF_AF_OD = 0x11
} GPIO_CNF_t;

typedef enum {
	GPIO_PUSH_PULL = 0, GPIO_OPENDRAIN,
} GPIO_Pull_t;

typedef struct {
	GPIO_Mode_t Mode;
	GPIO_CNF_t CNF;
	GPIO_Pin_t Pin;

} GPIO_Config_t;

typedef struct {
	GPIO_Config_t GPIO_Config;
	GPIO_TypeDef *Instance;
} GPIO_Handle_t;

GPIO_Handle_t GPIO_Init(GPIO_TypeDef *Instance, GPIO_Pin_t Pin,
		GPIO_Mode_t Mode, GPIO_CNF_t CNF);
void GPIO_WritePin(GPIO_Handle_t *xGPIO,int flag);
#endif /* GPIOX_DRIVER_H_ */
