/*
 * exti_driver.c
 *
 *  Created on: Apr 16, 2025
 *      Author: tr6r2
 */

#include "exti_driver.h"

void EXTI_Init(EXTI_Handle_t* hEXTI)
{
    // Bước 1: Cấu hình trigger (rising, falling, or both)
    if (hEXTI->Config.Trigger == EXTI_TRIGGER_RISING) {
        hEXTI->Constance->RTSR |= (1 << hEXTI->Config.EXTI_Line);  // Cấu hình Rising Edge trigger
        hEXTI->Constance->FTSR &= ~(1 << hEXTI->Config.EXTI_Line); // Tắt Falling Edge trigger
    }
    else if (hEXTI->Config.Trigger == EXTI_TRIGGER_FALLING) {
        hEXTI->Constance->RTSR &= ~(1 << hEXTI->Config.EXTI_Line); // Tắt Rising Edge trigger
        hEXTI->Constance->FTSR |= (1 << hEXTI->Config.EXTI_Line);  // Cấu hình Falling Edge trigger
    }
    else if (hEXTI->Config.Trigger == EXTI_TRIGGER_BOTH) {
        hEXTI->Constance->RTSR |= (1 << hEXTI->Config.EXTI_Line);  // Cấu hình Rising Edge trigger
        hEXTI->Constance->FTSR |= (1 << hEXTI->Config.EXTI_Line);  // Cấu hình Falling Edge trigger
    }

    // Bước 2: Bật ngắt EXTI (Kích hoạt ngắt trong IMR)
    hEXTI->Constance->IMR |= (1 << hEXTI->Config.EXTI_Line);  // Kích hoạt ngắt cho Line

    if (hEXTI->Config.EXTI_Line <= 4) {
        NVIC_EnableIRQ(EXTI0_IRQn + hEXTI->Config.EXTI_Line);
    }
    else if (hEXTI->Config.EXTI_Line <= 9) {
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    else {
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}
void NVIC_EnableIRQ(uint32_t IRQn) {
    NVIC_ISER0 |= (1 << IRQn);  // Set bit IRQn trong NVIC_ISER0 để kích hoạt ngắt
}
