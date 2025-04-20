/*
 * afio_driver.h
 *
 *  Created on: Apr 16, 2025
 *      Author: tr6r2
 */

#ifndef AFIO_DRIVER_H_
#define AFIO_DRIVER_H_

#include "stm32f103xx.h"

typedef enum {
    AFIO_FUNC_EXTI,
    AFIO_FUNC_REMAP,
    AFIO_FUNC_JTAG_CFG
    // có thể mở rộng sau này như AFIO_FUNC_EVENT, v.v.
} AFIO_Function_t;

typedef struct {
    AFIO_Function_t Function;     // Mục đích sử dụng AFIO

    EXTI_Line_t EXTI_Line;        // Dòng EXTI
    GPIO_PortSrc_t GPIO_PortSrc;  // Port dùng cho EXTI line

    uint32_t RemapMask;           // Cho remap
    uint8_t JTAG_Config;          // Cho cấu hình JTAG
} AFIO_Config_t;


typedef struct {
	AFIO_Config_t Config;  // Cấu hình EXTI
    AFIO_TypeDef_t* Constance;
} AFIO_Handle_t;

void AFIO_Init(AFIO_Handle_t* hAFIOx);

#endif /* AFIO_DRIVER_H_ */
