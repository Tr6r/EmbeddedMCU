/*
 * rcc_driver.c
 *
 *  Created on: Apr 10, 2025
 *      Author: tr6r2
 */

#include "rcc_driver.h"

void RCC_ConfigClock(RCC_Handle_t* hRcc) {

    if (hRcc == NULL) return;

    // 1. Kiểm tra và bật nguồn xung
    if (hRcc->Config.clk_src == RCC_CLK_HSE || hRcc->Config.pll_src == RCC_PLL_SRC_HSE) {
        RCC->CR |= RCC_CR_HSEON;
        while (!(RCC->CR & RCC_CR_HSERDY));
    } else if (hRcc->Config.clk_src == RCC_CLK_HSI || hRcc->Config.pll_src == RCC_PLL_SRC_HSI_DIV2) {
        RCC->CR |= RCC_CR_HSION;
        while (!(RCC->CR & RCC_CR_HSIRDY));
    } else {
        return;
    }

    if (hRcc->Config.clk_src == RCC_CLK_PLL) {
         if (hRcc->Config.pll_mul < 0b0000 || hRcc->Config.pll_mul > 0b1111) return;

         // Set PLL nguồn
         uint32_t input_clk = 0;
         if (hRcc->Config.pll_src == RCC_PLL_SRC_HSE) {
             RCC->CFGR |= RCC_CFGR_PLLSRC;
             input_clk = 8000000; // ví dụ HSE là 8 MHz
         } else if (hRcc->Config.pll_src == RCC_PLL_SRC_HSI_DIV2) {
             RCC->CFGR &= ~RCC_CFGR_PLLSRC;
             input_clk = 8000000 / 2;
         } else {
             return;
         }

         // Set PLL multiplier
         RCC->CFGR &= ~(0xF << 18);
         RCC->CFGR |= (hRcc->Config.pll_mul << 18);

         RCC->CR |= RCC_CR_PLLON;
         while (!(RCC->CR & RCC_CR_PLLRDY));

         SystemCoreClock = input_clk * (hRcc->Config.pll_mul + 2); // theo datasheet: x2~x16 tương ứng 0b0000 ~ 0b1110
     }
     else if (hRcc->Config.clk_src == RCC_CLK_HSE) {
         SystemCoreClock = 8000000;
     }
     else if (hRcc->Config.clk_src == RCC_CLK_HSI) {
         SystemCoreClock = 8000000;
     }
     else {
         return;
     }


    // 3. AHB, APB1, APB2 prescaler
    if (hRcc->Config.ahb_prescaler > 0xF || hRcc->Config.apb1_prescaler > 0x7 || hRcc->Config.apb2_prescaler > 0x7)
        return;

    RCC->CFGR &= ~(0xF << 4);  // AHB
    RCC->CFGR |= (hRcc->Config.ahb_prescaler << 4);

    RCC->CFGR &= ~(0x7 << 8);  // APB1
    RCC->CFGR |= (hRcc->Config.apb1_prescaler << 8);

    RCC->CFGR &= ~(0x7 << 11); // APB2
    RCC->CFGR |= (hRcc->Config.apb2_prescaler << 11);

    // 4. Set clock chính
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= (hRcc->Config.clk_src << 0);

    // 5. Chờ xác nhận
    while (((RCC->CFGR >> 2) & 0x3) != hRcc->Config.clk_src);

    uint8_t ahb_shift = (hRcc->Config.ahb_prescaler >= 0x8) ? (hRcc->Config.ahb_prescaler - 0x7) : 0;
    AHB_Clock = SystemCoreClock >> ahb_shift;

    uint8_t apb1_shift = (hRcc->Config.apb1_prescaler >= 0x4) ? (hRcc->Config.apb1_prescaler - 0x3) : 0;
    APB1_Clock = AHB_Clock >> apb1_shift;

    uint8_t apb2_shift = (hRcc->Config.apb2_prescaler >= 0x4) ? (hRcc->Config.apb2_prescaler - 0x3) : 0;
    APB2_Clock = AHB_Clock >> apb2_shift;
}

