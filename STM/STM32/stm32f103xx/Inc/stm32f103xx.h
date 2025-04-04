/*
 * stm32f103xx.h
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */

#ifndef STM32F103XX_H_
#define STM32F103XX_H_

#include<stdint.h>
#define FLASH_ADDRESS	  0x40000000U
//RCC_ADDRESS
#define RCC_BASE_ADDRESS 		 (FLASH_ADDRESS + 0x00021000U)

//GPIOx_ADDRESS
#define GPIOA_ADDRESS    (FLASH_ADDRESS + 0x00010800U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOB_ADDRESS    (FLASH_ADDRESS + 0x00010C00U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOC_ADDRESS    (FLASH_ADDRESS + 0x00011000U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOD_ADDRESS    (FLASH_ADDRESS + 0x00011400U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOE_ADDRESS    (FLASH_ADDRESS + 0x00011800U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOF_ADDRESS    (FLASH_ADDRESS + 0x00011C00U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U
#define GPIOG_ADDRESS    (FLASH_ADDRESS + 0x00012000U)  // hoặc viết rõ địa chỉ gốc: 0x40011000U


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
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *) RCC_BASE_ADDRESS)

#define GPIOA_EN_CLOCK()  (RCC->APB2ENR |= (1 << 2))  // Enable clock for GPIOA
#define GPIOB_EN_CLOCK()  (RCC->APB2ENR |= (1 << 3))  // Enable clock for GPIOB
#define GPIOC_EN_CLOCK()  (RCC->APB2ENR |= (1 << 4))  // Enable clock for GPIOC
#define GPIOD_EN_CLOCK()  (RCC->APB2ENR |= (1 << 5))  // Enable clock for GPIOD
#define GPIOE_EN_CLOCK()  (RCC->APB2ENR |= (1 << 6))  // Enable clock for GPIOE
#define GPIOF_EN_CLOCK()  (RCC->APB2ENR |= (1 << 7))  // Enable clock for GPIOF
#define GPIOG_EN_CLOCK()  (RCC->APB2ENR |= (1 << 8))  // Enable clock for GPIOG

#define GPIOA_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 2))  // Disable clock for GPIOA
#define GPIOB_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 3))  // Disable clock for GPIOB
#define GPIOC_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 4))  // Disable clock for GPIOC
#define GPIOD_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 5))  // Disable clock for GPIOD
#define GPIOE_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 6))  // Disable clock for GPIOE
#define GPIOF_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 7))  // Disable clock for GPIOF
#define GPIOG_DIS_CLOCK()  (RCC->APB2ENR &= ~(1 << 8))  // Disable clock for GPIOG

#endif /* STM32F103XX_H_ */
