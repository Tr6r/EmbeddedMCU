/*
 * stm32f103xx.h
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */

#ifndef STM32F103XX_H_
#define STM32F103XX_H_

#include <stddef.h>
#include<stdint.h>


#define PERIPHERAL_ADDRESS	  0x40000000U
//RCC_ADDRESS
#define RCC_BASE_ADDRESS 		 (PERIPHERAL_ADDRESS + 0x00021000U)

//GPIOx_ADDRESS
#define GPIOA_ADDRESS    (PERIPHERAL_ADDRESS + 0x00010800U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOB_ADDRESS    (PERIPHERAL_ADDRESS + 0x00010C00U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOC_ADDRESS    (PERIPHERAL_ADDRESS + 0x00011000U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOD_ADDRESS    (PERIPHERAL_ADDRESS + 0x00011400U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOE_ADDRESS    (PERIPHERAL_ADDRESS + 0x00011800U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOF_ADDRESS    (PERIPHERAL_ADDRESS + 0x00011C00U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOG_ADDRESS    (PERIPHERAL_ADDRESS + 0x00012000U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U

//AFIO_ADDRESS
#define AFIO_ADDRESS    (PERIPHERAL_ADDRESS + 0x00010000U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U

//Timerx_ADDRESS

#define TIMER2_ADDRESS   (PERIPHERAL_ADDRESS)
#define TIMER3_ADDRESS   (PERIPHERAL_ADDRESS + 0x00000400U)
#define TIMER4_ADDRESS   (PERIPHERAL_ADDRESS + 0x00000800U)
#define TIMER5_ADDRESS   (PERIPHERAL_ADDRESS + 0x00000C00U)

// EXTI ADDRESS
#define EXTI_ADDRESS  	 (PERIPHERAL_ADDRESS + 0x00010400U)

#define NVIC_ISER0 (*(volatile uint32_t*)0xE000E100)

//struct GPIO

typedef struct {
    volatile uint32_t CRL;     // Configuration register low
    volatile uint32_t CRH;     // Configuration register high
    volatile uint32_t IDR;     // Input data register
    volatile uint32_t ODR;     // Output data register
    volatile uint32_t BSRR;    // Bit set/reset register
    volatile uint32_t BRR;     // Bit reset register
    volatile uint32_t LCKR;    // Configuration lock register
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *) GPIOA_ADDRESS)
#define GPIOB ((GPIO_TypeDef *) GPIOB_ADDRESS)
#define GPIOC ((GPIO_TypeDef *) GPIOC_ADDRESS)
#define GPIOD ((GPIO_TypeDef *) GPIOD_ADDRESS)
#define GPIOE ((GPIO_TypeDef *) GPIOE_ADDRESS)
#define GPIOF ((GPIO_TypeDef *) GPIOF_ADDRESS)
#define GPIOG ((GPIO_TypeDef *) GPIOG_ADDRESS)

//ENUM DEFINE
typedef enum {
    EXTI_LINE_0  = 0,
    EXTI_LINE_1  = 1,
    EXTI_LINE_2  = 2,
    EXTI_LINE_3  = 3,
    EXTI_LINE_4  = 4,
    EXTI_LINE_5  = 5,
    EXTI_LINE_6  = 6,
    EXTI_LINE_7  = 7,
    EXTI_LINE_8  = 8,
    EXTI_LINE_9  = 9,
    EXTI_LINE_10 = 10,
    EXTI_LINE_11 = 11,
    EXTI_LINE_12 = 12,
    EXTI_LINE_13 = 13,
    EXTI_LINE_14 = 14,
    EXTI_LINE_15 = 15
} EXTI_Line_t;
typedef enum {
    GPIO_PORTSRC_A = 0,
    GPIO_PORTSRC_B = 1,
    GPIO_PORTSRC_C = 2,
    GPIO_PORTSRC_D = 3,
    GPIO_PORTSRC_E = 4
} GPIO_PortSrc_t;


// struct AFIO
typedef struct {
    volatile uint32_t EVCR;       // Event Control Register                (Offset: 0x00)
    volatile uint32_t MAPR;       // AF remap and debug I/O configuration (Offset: 0x04)
    volatile uint32_t EXTICR[4];  // External interrupt configuration      (Offset: 0x08 - 0x14)
    volatile uint32_t MAPR2;      // AF remap and debug I/O configuration 2 (Offset: 0x1C)
} AFIO_TypeDef_t;


#define AFIO ((AFIO_TypeDef_t *) AFIO_ADDRESS)


//RCC Define

extern uint32_t SystemCoreClock;
extern uint32_t APB1_Clock;
extern uint32_t APB2_Clock;
extern uint32_t AHB_Clock;

typedef struct {
    volatile uint32_t CR;            // Clock control register
    volatile uint32_t CFGR;          // Clock configuration register
    volatile uint32_t CIR;           // Clock interrupt register
    volatile uint32_t APB2RSTR;      // APB2 peripheral reset register
    volatile uint32_t APB1RSTR;      // APB1 peripheral reset register
    volatile uint32_t AHBENR;        // AHB peripheral clock enable register
    volatile uint32_t APB2ENR;       // APB2 peripheral clock enable register
    volatile uint32_t APB1ENR;       // APB1 peripheral clock enable register
    volatile uint32_t BDCR;          // Backup domain control register
    volatile uint32_t CSR;           // Control/status register
} RCC_TypeDef_t;

#define RCC ((RCC_TypeDef_t *) RCC_BASE_ADDRESS)

//GPIOx_Conttrol_Clock
#define AFIO_EN_CLOCK()  (RCC->APB2ENR |= (1 << 0))

#define GPIOA_EN_CLOCK()  (RCC->APB2ENR |= (1 << 2))  // Enable clock for GPIOA
#define GPIOB_EN_CLOCK()  (RCC->APB2ENR |= (1 << 3))  // Enable clock for GPIOB
#define GPIOC_EN_CLOCK()  (RCC->APB2ENR |= (1 << 4))  // Enable clock for GPIOC
#define GPIOD_EN_CLOCK()  (RCC->APB2ENR |= (1 << 5))  // Enable clock for GPIOD
#define GPIOE_EN_CLOCK()  (RCC->APB2ENR |= (1 << 6))  // Enable clock for GPIOE
#define GPIOF_EN_CLOCK()  (RCC->APB2ENR |= (1 << 7))  // Enable clock for GPIOF
#define GPIOG_EN_CLOCK()  (RCC->APB2ENR |= (1 << 8))  // Enable clock for GPIOG

#define AFIO_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 0))

#define GPIOA_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 2))  // Disable clock for GPIOA
#define GPIOB_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 3))  // Disable clock for GPIOB
#define GPIOC_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 4))  // Disable clock for GPIOC
#define GPIOD_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 5))  // Disable clock for GPIOD
#define GPIOE_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 6))  // Disable clock for GPIOE
#define GPIOF_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 7))  // Disable clock for GPIOF
#define GPIOG_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 8))  // Disable clock for GPIOG

//Timerx_Conttrol_Clock
#define TIMER2_EN_CLOCK()  (RCC->APB1ENR |= (1 << 0))  // Enable clock for GPIOA
#define TIMER3_EN_CLOCK()  (RCC->APB1ENR |= (1 << 1))  // Enable clock for GPIOA
#define TIMER4_EN_CLOCK()  (RCC->APB1ENR |= (1 << 2))  // Enable clock for GPIOA
#define TIMER5_EN_CLOCK()  (RCC->APB1ENR |= (1 << 3))  // Enable clock for GPIOA

#define TIMER2_DIS_CLOCK()  (RCC->APB1ENR &= ~(1 << 0))  // Enable clock for GPIOA
#define TIMER3_DIS_CLOCK()  (RCC->APB1ENR &= ~(1 << 1))  // Enable clock for GPIOA
#define TIMER4_DIS_CLOCK()  (RCC->APB1ENR &= ~(1 << 2))  // Enable clock for GPIOA
#define TIMER5_DIS_CLOCK()  (RCC->APB1ENR &= ~(1 << 3))  // Enable clock for GPIOA
//struct TIMER2 -TIMER5
typedef struct
{
  volatile uint32_t CR1;       // 0x00: Control register 1
  volatile uint32_t CR2;       // 0x04: Control register 2
  volatile uint32_t SMCR;      // 0x08: Slave mode control register
  volatile uint32_t DIER;      // 0x0C: DMA/Interrupt enable register
  volatile uint32_t SR;        // 0x10: Status register
  volatile uint32_t EGR;       // 0x14: Event generation register
  volatile uint32_t CCMR1;     // 0x18: Capture/compare mode register 1
  volatile uint32_t CCMR2;     // 0x1C: Capture/compare mode register 2
  volatile uint32_t CCER;      // 0x20: Capture/compare enable register
  volatile uint32_t CNT;       // 0x24: Counter
  volatile uint32_t PSC;       // 0x28: Prescaler
  volatile uint32_t ARR;       // 0x2C: Auto-reload register
  uint32_t Reserved;
  volatile uint32_t CCR1;      // 0x34: Capture/compare register 1
  volatile uint32_t CCR2;      // 0x38: Capture/compare register 2
  volatile uint32_t CCR3;      // 0x3C: Capture/compare register 3
  volatile uint32_t CCR4;      // 0x40: Capture/compare register 4
  uint32_t Reserved2;
  volatile uint32_t DCR;       // 0x48: DMA control register
  volatile uint32_t DMAR;      // 0x4C: DMA address for full transfer
} TIMER2_5_TypeDef_t;


#define TIMER2 ((TIMER2_5_TypeDef_t *)TIMER2_ADDRESS)
#define TIMER3 ((TIMER2_5_TypeDef_t *)TIMER3_ADDRESS)
#define TIMER4 ((TIMER2_5_TypeDef_t *)TIMER4_ADDRESS)
#define TIMER5 ((TIMER2_5_TypeDef_t *)TIMER5_ADDRESS)


//Struct EXTI
typedef struct
{
    volatile uint32_t IMR;     // Interrupt mask register (0x00)
    volatile uint32_t EMR;     // Event mask register (0x04)
    volatile uint32_t RTSR;    // Rising trigger selection register (0x08)
    volatile uint32_t FTSR;    // Falling trigger selection register (0x0C)
    volatile uint32_t SWIER;   // Software interrupt event register (0x10)
    volatile uint32_t PR;      // Pending register (0x14)
} EXTI_TypeDef_t;
#define EXTI ((EXTI_TypeDef_t *)EXTI_ADDRESS)


#endif /* STM32F103XX_H_ */
