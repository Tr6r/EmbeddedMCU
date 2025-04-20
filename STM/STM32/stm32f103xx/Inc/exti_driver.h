/*
 * exti_driver.h
 *
 *  Created on: Apr 16, 2025
 *      Author: tr6r2
 */

#ifndef EXTI_DRIVER_H_
#define EXTI_DRIVER_H_

#include "stm32f103xx.h"
#ifndef IRQn_DEF
#define EXTI0_IRQn      6
#define EXTI1_IRQn      7
#define EXTI2_IRQn      8
#define EXTI3_IRQn      9
#define EXTI4_IRQn      10
#define EXTI9_5_IRQn    23
#define EXTI15_10_IRQn  40
#endif
typedef enum {
    EXTI_TRIGGER_RISING = 0,  // Rising edge trigger
    EXTI_TRIGGER_FALLING = 1, // Falling edge trigger
    EXTI_TRIGGER_BOTH = 2     // Both rising and falling edge trigger
} EXTI_Trigger_t;

// Cấu trúc cấu hình EXTI
typedef struct {
    EXTI_Line_t EXTI_Line;   // Số line EXTI (0 đến 15)
    EXTI_Trigger_t Trigger;  // Kiểu kích hoạt: rising, falling, both
    GPIO_PortSrc_t PortSrc;     // Nguồn GPIO (ví dụ: GPIOA, GPIOB, ...)
} EXTI_Config_t;

typedef struct {
	EXTI_Config_t Config;
	EXTI_TypeDef_t* Constance;
} EXTI_Handle_t;

void EXTI_Init(EXTI_Handle_t* hEXTI);
#endif /* EXTI_DRIVER_H_ */
