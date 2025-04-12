/*
 * timer_driver.c
 *
 *  Created on: Apr 5, 2025
 *      Author: tr6r2
 */
#include "timerx_driver.h"

TIMER2_5_Handle_t TIMER2_5_Init_Delay(TIMER2_5_TypeDef_t* Instance, uint16_t Prs)
{
	TIMER2_5_Handle_t hTIM2_5;

    hTIM2_5.Instance = Instance;
    hTIM2_5.Config.Prs = Prs;


    Instance->CR1 &= ~(1 << 0);

    // Enable clock
    if (Instance == TIMER2) TIMER2_EN_CLOCK();
    else if (Instance == TIMER3) TIMER3_EN_CLOCK();
    else if (Instance == TIMER4) TIMER4_EN_CLOCK();
    else if (Instance == TIMER5) TIMER5_EN_CLOCK();

    // Set prescaler and auto-reload
    Instance->PSC = Prs;

    Instance->CNT = 0;
    // Enable counter
    Instance->CR1 |= (1 << 0); // CEN = 1

    return hTIM2_5;
}

void TIMER2_5_Delay(TIMER2_5_Handle_t* hTimerx, uint16_t Arr)
{
    // Cấu hình giá trị ARR (Auto-reload register) cho thời gian delay
	hTimerx->Instance->ARR = Arr;
    hTimerx->Instance->SR &= ~(1 << 0);  // Clear the UIF flag

    // Đặt lại CNT về 0
	hTimerx->Instance->CNT = 0;

    // Đợi cho đến khi CNT đạt ARR (timer tràn)
    while (!(hTimerx->Instance->SR & (1 << 0))) {
    }

}
//
//TIMER2_5_Handle_t Init_StepMotor_Base(TIMER2_5_TypeDef_t* Instance, uint16_t Prs, uint16_t Arr)
//{
//    	TIMER2_5_Handle_t htim;
//
//	   if (Instance == TIMER2) TIMER2_EN_CLOCK();
//	    else if (Instance == TIMER3) TIMER3_EN_CLOCK();
//	    else if (Instance == TIMER4) TIMER4_EN_CLOCK();
//	    else if (Instance == TIMER5) TIMER5_EN_CLOCK();
//	   htim.Instance = Instance;
//	   htim.Config.Prs= Prs;
//	   htim.Config.Arr = Arr;
//	   htim.Instance->CR1 &= ~(3 << 5); // CMS = 00
//	   htim.Instance->CR1 &= ~(1 << 4); // DIR = 0 (up)
//	   htim.Instance->PSC = Prs;
//	   htim.Instance->ARR = Arr;
//	   htim.Instance->CR1 |= (1 << 7); // ARPE
//	   htim.Instance->EGR |= (1 << 0); // UG
//	   htim.Instance->CR1 |= (1 << 0);  // CEN = 1
//	    return htim;
//
//	}
//
//void Init_StepMotorChannel(TIMER2_5_TypeDef_t* Instance, TIM_Channel_t Channel,TIM_OCMode_t OCMode, uint16_t CompareValue)
//{
//    switch (Channel)
//    {
//        case TIM_CHANNEL_1:
//            Instance->CCR1 = CompareValue;
//            Instance->CCMR1 &= ~(0xFF);
//            Instance->CCMR1 |= (OCMode << 4);
//            Instance->CCMR1 |= (1 << 3);
//            Instance->CCER|= (1 << 0);
//            break;
//
//        case TIM_CHANNEL_2:
//            Instance->CCR2 = CompareValue;
//            Instance->CCMR1 &= ~(0xFF << 8);
//            Instance->CCMR1 |= (OCMode << 12);
//            Instance->CCMR1 |= (1 << 11);
//            Instance->CCER &= ~(1 << 4);
//            break;
//
//        case TIM_CHANNEL_3:
//            Instance->CCR3 = CompareValue;
//            Instance->CCMR2 &= ~(0xFF);
//            Instance->CCMR2 |= (OCMode << 4);
//            Instance->CCMR2 |= (1 << 3);
//            Instance->CCER &= ~(1 << 8);
//            break;
//
//        case TIM_CHANNEL_4:
//            Instance->CCR4 = CompareValue;
//            Instance->CCMR2 &= ~(0xFF << 8);
//            Instance->CCMR2 |= (OCMode << 12);
//            Instance->CCMR2 |= (1 << 11);
//            Instance->CCER &= ~(1 << 12);
//            break;
//    }
//}
//
//void StepMotor_Rotate(TIMER2_5_Handle_t* htim, TIM_Channel_t channel, float angle)
//{
//    const uint16_t steps_per_rev = 200;  // Số bước cho một vòng quay
//    const uint8_t microstep = 32;        // Microstep 1/32
//
//    uint16_t total_steps = (angle / 360.0f) * (steps_per_rev * microstep);
//
//    volatile uint32_t* ccr;
//    if (htim == NULL || htim->Instance == NULL) {
//        // Báo lỗi hoặc return sớm
//        return;
//    }
//    // Xác định địa chỉ CCRx tương ứng với channel
//    switch (channel) {
//        case TIM_CHANNEL_1:
//            ccr = &(htim->Instance->CCR1);
//            htim->Instance->CCER |= (1 << 0);   // Bật Channel 1
//            break;
//        case TIM_CHANNEL_2:
//            ccr = &(htim->Instance->CCR2);
//            htim->Instance->CCER |= (1 << 4);   // Bật Channel 2
//            break;
//        case TIM_CHANNEL_3:
//            ccr = &(htim->Instance->CCR3);
//            htim->Instance->CCER |= (1 << 8);   // Bật Channel 3
//            break;
//        case TIM_CHANNEL_4:
//            ccr = &(htim->Instance->CCR4);
//            htim->Instance->CCER |= (1 << 12);  // Bật Channel 4
//            break;
//        default:
//            return; // Channel không hợp lệ
//    }
//
//    for (uint16_t i = 0; i < total_steps; i++) {
//        *ccr = htim->Config.Arr / 2;  // Set duty cycle 50% để tạo xung HIGH
//        while (!(htim->Instance->SR & (1 << 1)));  // Chờ đến khi update event (UIF) xảy ra
//        htim->Instance->SR &= ~(1 << 1); // Clear flag
//
//        *ccr = 0;  // Set duty = 0 để tạo xung LOW
//        while (!(htim->Instance->SR & (1 << 1)));
//        htim->Instance->SR &= ~(1 << 1); // Clear flag
//    }
//
//    // Tắt channel sau khi hoàn tất
//    switch (channel) {
//        case TIM_CHANNEL_1:
//            htim->Instance->CCER &= ~(1 << 0);   // Tắt Channel 1
//            break;
//        case TIM_CHANNEL_2:
//            htim->Instance->CCER &= ~(1 << 4);   // Tắt Channel 2
//            break;
//        case TIM_CHANNEL_3:
//            htim->Instance->CCER &= ~(1 << 8);   // Tắt Channel 3
//            break;
//        case TIM_CHANNEL_4:
//            htim->Instance->CCER &= ~(1 << 12);  // Tắt Channel 4
//            break;
//        default:
//            return; // Channel không hợp lệ
//    }
//}
//

