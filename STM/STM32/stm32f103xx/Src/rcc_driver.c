/*
 * rcc_driver.c
 *
 *  Created on: Apr 10, 2025
 *      Author: tr6r2
 */

#include "rcc_driver.h"

void RCC_ConfigClock(RCC_Handle_t* hRcc) {

    if (hRcc == NULL) return;

    // 1. Bật xung HSE hoặc HSI
    if (hRcc->Config.clk_src == RCC_CLK_HSE || hRcc->Config.pll_src == RCC_PLL_SRC_HSE) {
        RCC->CR |= (1 << 16); // HSEON
        while (!(RCC->CR & (1 << 17))); // HSERDY
    } else if (hRcc->Config.clk_src == RCC_CLK_HSI || hRcc->Config.pll_src == RCC_PLL_SRC_HSI_DIV2) {
        RCC->CR |= (1 << 0); // HSION
        while (!(RCC->CR & (1 << 1))); // HSIRDY
    } else {
        return;
    }

    // 2. Nếu dùng PLL
    if (hRcc->Config.clk_src == RCC_CLK_PLL) {
        if (hRcc->Config.pll_mul < 0b0000 || hRcc->Config.pll_mul > 0b1110) return;

        uint32_t input_clk = 0;

        if (hRcc->Config.pll_src == RCC_PLL_SRC_HSE) {
            RCC->CFGR |= (1 << 16); // PLLSRC = HSE
            input_clk = 8000000; // giả định HSE = 8MHz
        } else if (hRcc->Config.pll_src == RCC_PLL_SRC_HSI_DIV2) {
            RCC->CFGR &= ~(1 << 16); // PLLSRC = HSI/2
            input_clk = 8000000 / 2;
        } else {
            return;
        }

        RCC->CFGR &= ~(0xF << 18); // clear PLLMUL bits
        RCC->CFGR |= (hRcc->Config.pll_mul << 18);

        RCC->CR |= (1 << 24); // PLLON
        while (!(RCC->CR & (1 << 25))); // PLLRDY

        SystemCoreClock = input_clk * (hRcc->Config.pll_mul + 2);
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

    // 3. Thiết lập các prescaler
    if (hRcc->Config.ahb_prescaler > 0xF || hRcc->Config.apb1_prescaler > 0x7 || hRcc->Config.apb2_prescaler > 0x7)
        return;

    RCC->CFGR &= ~(0xF << 4); // AHB prescaler
    RCC->CFGR |= (hRcc->Config.ahb_prescaler << 4);

    RCC->CFGR &= ~(0x7 << 8); // APB1
    RCC->CFGR |= (hRcc->Config.apb1_prescaler << 8);

    RCC->CFGR &= ~(0x7 << 11); // APB2
    RCC->CFGR |= (hRcc->Config.apb2_prescaler << 11);

    // 4. Chọn nguồn clock chính
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= (hRcc->Config.clk_src << 0);

    while (((RCC->CFGR >> 2) & 0x3) != hRcc->Config.clk_src); // chờ SWS = clk_src

    // 5. Cập nhật clock nội bộ
    uint8_t ahb_shift = (hRcc->Config.ahb_prescaler >= 0x8) ? (hRcc->Config.ahb_prescaler - 0x7) : 0;
    AHB_Clock = SystemCoreClock >> ahb_shift;

    uint8_t apb1_shift = (hRcc->Config.apb1_prescaler >= 0x4) ? (hRcc->Config.apb1_prescaler - 0x3) : 0;
    APB1_Clock = AHB_Clock >> apb1_shift;

    uint8_t apb2_shift = (hRcc->Config.apb2_prescaler >= 0x4) ? (hRcc->Config.apb2_prescaler - 0x3) : 0;
    APB2_Clock = AHB_Clock >> apb2_shift;
}
