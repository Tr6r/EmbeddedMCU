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
#define WWDG_IRQn                   0
#define PVD_IRQn                    1
#define TAMPER_IRQn                 2
#define RTC_IRQn                    3
#define FLASH_IRQn                  4
#define RCC_IRQn                    5
#define EXTI0_IRQn                  6
#define EXTI1_IRQn                  7
#define EXTI2_IRQn                  8
#define EXTI3_IRQn                  9
#define EXTI4_IRQn                  10
#define DMA1_Channel1_IRQn          11
#define DMA1_Channel2_IRQn          12
#define DMA1_Channel3_IRQn          13
#define DMA1_Channel4_IRQn          14
#define DMA1_Channel5_IRQn          15
#define DMA1_Channel6_IRQn          16
#define DMA1_Channel7_IRQn          17
#define ADC1_2_IRQn                 18
#define USB_HP_CAN_TX_IRQn          19
#define USB_LP_CAN_RX0_IRQn         20
#define CAN_RX1_IRQn                21
#define CAN_SCE_IRQn                22
#define EXTI9_5_IRQn                23
#define TIM1_BRK_IRQn               24
#define TIM1_UP_IRQn                25
#define TIM1_TRG_COM_IRQn           26
#define TIM1_CC_IRQn                27
#define TIM2_IRQn                   28
#define TIM3_IRQn                   29
#define TIM4_IRQn                   30
#define I2C1_EV_IRQn                31
#define I2C1_ER_IRQn                32
#define I2C2_EV_IRQn                33
#define I2C2_ER_IRQn                34
#define SPI1_IRQn                   35
#define SPI2_IRQn                   36
#define USART1_IRQn                 37
#define USART2_IRQn                 38
#define USART3_IRQn                 39
#define EXTI15_10_IRQn              40
#define RTCAlarm_IRQn               41
#define USBWakeUp_IRQn              42
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
