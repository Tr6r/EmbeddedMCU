/*
 * timer_advance_driver.c
 *
 *  Created on: Aug 31, 2025
 *      Author: tr6r2
 */


#include <timer_advance_driver.h>
static void _clear_oc_config(TIMER_Advance_TypeDef_t *TIMx, TIM_Advance_Channel_t ch)
{
    if (ch == TIM1_CHANNEL_1) {
        TIMx->CCMR1 &= ~((0x7U << 4) | (1U << 3));   // clear OC1M (bits 6:4) and OC1PE (bit3)
    } else if (ch == TIM1_CHANNEL_2) {
        TIMx->CCMR1 &= ~((0x7U << 12) | (1U << 11)); // clear OC2M (bits 14:12) and OC2PE (bit11)
    } else if (ch == TIM1_CHANNEL_3) {
        TIMx->CCMR2 &= ~((0x7U << 4) | (1U << 3));   // clear OC3M and OC3PE
    } else { // CH4
        TIMx->CCMR2 &= ~((0x7U << 12) | (1U << 11)); // clear OC4M and OC4PE
    }
}

/* Set OCxM and OCxPE for a channel */
static void _set_oc_config(TIMER_Advance_TypeDef_t *TIMx, TIM_Advance_Channel_t ch, uint32_t ocmode)
{
    if (ch == TIM1_CHANNEL_1) {
        TIMx->CCMR1 |= ((ocmode & 0x7U) << 4);
        TIMx->CCMR1 |= (1U << 3);   // OC1PE
    } else if (ch == TIM1_CHANNEL_2) {
        TIMx->CCMR1 |= ((ocmode & 0x7U) << 12);
        TIMx->CCMR1 |= (1U << 11);  // OC2PE
    } else if (ch == TIM1_CHANNEL_3) {
        TIMx->CCMR2 |= ((ocmode & 0x7U) << 4);
        TIMx->CCMR2 |= (1U << 3);   // OC3PE
    } else {
        TIMx->CCMR2 |= ((ocmode & 0x7U) << 12);
        TIMx->CCMR2 |= (1U << 11);  // OC4PE
    }
}
void TIMER_Advance_Init(TIMER_Advance_Handle_t *hTimerAdvance)
{
    if (hTimerAdvance == NULL || hTimerAdvance->Instance == NULL) return;

    TIMER_Advance_TypeDef_t *TIMx = hTimerAdvance->Instance;

    if (TIMx == TIMER1)
        TIMER1_EN_CLOCK();



    if (hTimerAdvance->Config.Feature == TIM1_FEATURE_PWM) {
    	/* Basic prescaler and ARR */
		TIMx->PSC = hTimerAdvance->Config.Prescaler;
		TIMx->ARR = hTimerAdvance->Config.AutoReload;

		/* Ensure counter reset */
		TIMx->CNT = 0;
        TIM_Advance_Channel_t ch = hTimerAdvance->Config.Channel;

        _clear_oc_config(TIMx, ch);
        _set_oc_config(TIMx, ch, (uint32_t)hTimerAdvance->Config.OCMode);

        switch (ch) {
            case TIM1_CHANNEL_1:
                TIMx->CCR1 = hTimerAdvance->Config.CompareValue;
                TIMx->CCER |= (1U << 0);   // CC1E
                break;
            case TIM1_CHANNEL_2:
                TIMx->CCR2 = hTimerAdvance->Config.CompareValue;
                TIMx->CCER |= (1U << 4);   // CC2E
                break;
            case TIM1_CHANNEL_3:
                TIMx->CCR3 = hTimerAdvance->Config.CompareValue;
                TIMx->CCER |= (1U << 8);   // CC3E
                break;
            case TIM1_CHANNEL_4:
                TIMx->CCR4 = hTimerAdvance->Config.CompareValue;
                TIMx->CCER |= (1U << 12);  // CC4E
                break;
        }

        TIMx->CR1 |= (1U << 7);  // ARPE
        TIMx->EGR |= (1U << 0);  // UG
        TIMx->BDTR |= (1U << 15); // MOE
        TIMx->CR1 |= (1U << 0);  // CEN
    }
    else if (hTimerAdvance->Config.Feature == TIM1_FEATURE_ENCODER) {
        /* Encoder config will be in StartEncoder */
        TIM_TypeDef *TIMx = hTimerAdvance->Instance;

        // 1. Clear & set mode (SMS)
        TIMx->SMCR &= ~(0x7);
        TIMx->SMCR |= hTimerAdvance->Config.SMode;

        // 2. Xử lý channel
        if (hTimerAdvance->Config.Channel == TIM1_CHANNEL_1
            || hTimerAdvance->Config.Channel == TIM1_CHANNEL_2) {
            // --- Encoder dùng CH1 + CH2 ---
            TIMx->CCMR1 &= ~((3 << 0) | (3 << 8));  // clear CC1S, CC2S
            TIMx->CCMR1 |= (1 << 0) | (1 << 8);     // CC1S=01, CC2S=01

            // lọc input (IC1F, IC2F)
            TIMx->CCMR1 |= (0b0011 << 4);   // IC1F
            TIMx->CCMR1 |= (0b0011 << 12);  // IC2F

            // enable CC1E + CC2E
            TIMx->CCER |= (1 << 0) | (1 << 4);

        } else if (hTimerAdvance->Config.Channel == TIM1_CHANNEL_3
                   || hTimerAdvance->Config.Channel == TIM1_CHANNEL_4) {
            // --- Encoder dùng CH3 + CH4 ---
            TIMx->CCMR2 &= ~((3 << 0) | (3 << 8));  // clear CC3S, CC4S
            TIMx->CCMR2 |= (1 << 0) | (1 << 8);     // CC3S=01, CC4S=01

            // lọc input (IC3F, IC4F)
            TIMx->CCMR2 |= (0b0011 << 4);   // IC3F
            TIMx->CCMR2 |= (0b0011 << 12);  // IC4F

            // enable CC3E + CC4E
            TIMx->CCER |= (1 << 8) | (1 << 12);
        }

        // 3. Không đảo pha (nếu cần đảo thì set CCxP)
        TIMx->CCER &= ~((1 << 1) | (1 << 5) | (1 << 9) | (1 << 13));

        // 4. Set ARR và PSC
        TIMx->ARR = hTimerAdvance->Config.Arr;
        TIMx->PSC = hTimerAdvance->Config.Prs;

        // 5. Reset counter + bật timer
        TIMx->CNT = 0;
        TIMx->CR1 |= (1 << 0); // CEN=1
    }

}

