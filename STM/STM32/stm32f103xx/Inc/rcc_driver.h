/*
 * rcc_driver.h
 *
 *  Created on: Apr 10, 2025
 *      Author: tr6r2
 */

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_

#include "stm32f103xx.h"

typedef enum {
    RCC_PLL_MUL_2  = 0b0000,
    RCC_PLL_MUL_3  = 0b0001,
    RCC_PLL_MUL_4  = 0b0010,
    RCC_PLL_MUL_5  = 0b0011,
    RCC_PLL_MUL_6  = 0b0100,
    RCC_PLL_MUL_7  = 0b0101,
    RCC_PLL_MUL_8  = 0b0110,
    RCC_PLL_MUL_9  = 0b0111,
    RCC_PLL_MUL_10 = 0b1000,
    RCC_PLL_MUL_11 = 0b1001,
    RCC_PLL_MUL_12 = 0b1010,
    RCC_PLL_MUL_13 = 0b1011,
    RCC_PLL_MUL_14 = 0b1100,
    RCC_PLL_MUL_15 = 0b1101,
    RCC_PLL_MUL_16 = 0b1110
} RCC_PLLMul_t;

typedef enum {
    RCC_AHB_DIV_1   = 0b0000,
    RCC_AHB_DIV_2   = 0b1000,
    RCC_AHB_DIV_4   = 0b1001,
    RCC_AHB_DIV_8   = 0b1010,
    RCC_AHB_DIV_16  = 0b1011,
    RCC_AHB_DIV_64  = 0b1100,
    RCC_AHB_DIV_128 = 0b1101,
    RCC_AHB_DIV_256 = 0b1110,
    RCC_AHB_DIV_512 = 0b1111
} RCC_AHB_Prescaler_t;

typedef enum {
    RCC_APB_DIV_1  = 0b000,
    RCC_APB_DIV_2  = 0b100,
    RCC_APB_DIV_4  = 0b101,
    RCC_APB_DIV_8  = 0b110,
    RCC_APB_DIV_16 = 0b111
} RCC_APB_Prescaler_t;


typedef enum {
    RCC_CLK_HSI = 0,
    RCC_CLK_HSE = 1,
    RCC_CLK_PLL = 2
} RCC_ClockSource_t;

typedef enum {
    RCC_PLL_SRC_HSI_DIV2 = 0,
    RCC_PLL_SRC_HSE      = 1
} RCC_PLLSource_t;

typedef struct {
	RCC_ClockSource_t clk_src;         // HSI, HSE, PLL
	RCC_PLLSource_t pll_src;         // HSI/2 hoặc HSE
	RCC_PLLMul_t pll_mul;         // Hệ số nhân PLL (x2 đến x16)
	RCC_AHB_Prescaler_t ahb_prescaler;   // Chia AHB bus
    RCC_APB_Prescaler_t apb1_prescaler;  // Chia APB1
    RCC_APB_Prescaler_t apb2_prescaler;  // Chia APB2
} RCC_Config_t;

typedef struct {
	RCC_TypeDef_t *Instance;
    RCC_Config_t Config;
} RCC_Handle_t;

void RCC_ConfigClock(RCC_Handle_t* hRcc);

#endif /* RCC_DRIVER_H_ */
