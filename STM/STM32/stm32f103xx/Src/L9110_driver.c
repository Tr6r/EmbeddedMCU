/*
 * L9110_driver.c
 *
 *  Created on: May 6, 2025
 *      Author: tr6r2
 */

#include <L9110_driver.h>


void L9110_Run(L9110_Handle_t *Instance, L9110_Direction_t Dir, uint8_t pwm_8bit)
{
    // Set chiều bằng Digital pin
    if (Dir == L9110_DIRECTION_FORWARD) {
        GPIO_WritePin(Instance->Digital_Pin, GPIO_HIGH);
    } else {
        GPIO_WritePin(Instance->Digital_Pin, GPIO_LOW);
    }

    // Scale từ 0–255 sang 0–ARR
    uint16_t ccr_val = ((uint32_t)pwm_8bit * Instance->hTIM->Config.AutoReload) / 255;

    // Đảo duty vì L9110 logic ngược
    ccr_val = Instance->hTIM->Config.AutoReload - ccr_val;

    switch (Instance->hTIM->Config.Channel) {
        case TIM1_CHANNEL_1:
            Instance->hTIM->Instance->CCR1 = ccr_val;
            break;
        case TIM1_CHANNEL_2:
            Instance->hTIM->Instance->CCR2 = ccr_val;
            break;
        case TIM1_CHANNEL_3:
            Instance->hTIM->Instance->CCR3 = ccr_val;
            break;
        case TIM1_CHANNEL_4:
            Instance->hTIM->Instance->CCR4 = ccr_val;
            break;
    }
}



void L9110_Stop(L9110_Handle_t *Instance)
{
    // 1. Trỏ nhanh đến TIM và channel
    TIMER_Advance_TypeDef_t *hTIM = Instance->hTIM->Instance;
    TIM_Advance_Channel_t ch = Instance->hTIM->Config.Channel;

    // 2. Dừng PWM (set CCR = 0)
    switch (ch) {
        case TIM1_CHANNEL_1: hTIM->CCR1 = 0; break;
        case TIM1_CHANNEL_2: hTIM->CCR2 = 0; break;
        case TIM1_CHANNEL_3: hTIM->CCR3 = 0; break;
        case TIM1_CHANNEL_4: hTIM->CCR4 = 0; break;
        default: /* nothing */ break;
    }

    // 3. Option: về trạng thái an toàn → digital LOW
    GPIO_WritePin(Instance->Digital_Pin, GPIO_LOW);
}
void L9110_Straight_Dis(L9110_Handle_t *Instance, L9110_Direction_t Dir,
		uint16_t pwm,TIMER2_5_TypeDef_t *EncInstance, uint16_t Enc) {
	Instance->hTIM->Instance->CNT = 0;
	EncInstance->CNT = 0;
	if (Dir == L9110_DIRECTION_FORWARD) {
		if (Instance->hTIM->Config.Channel == TIM_CHANNEL_1) {
			Instance->hTIM->Instance->CCR1 = pwm;  // PWM ra kênh 1
			Instance->hTIM->Instance->CCR2 = 0;    // Kênh 2 tắt
		} else if (Instance->hTIM->Config.Channel == TIM_CHANNEL_3) {
			Instance->hTIM->Instance->CCR3 = pwm;  // PWM ra kênh 1
			Instance->hTIM->Instance->CCR4 = 0;    // Kênh 2 tắt
		}

	} else if (Dir == L9110_DIRECTION_REVERSE) {
		if (Instance->hTIM->Config.Channel == TIM_CHANNEL_1) {
			Instance->hTIM->Instance->CCR1 = 0;  // PWM ra kênh 1
			Instance->hTIM->Instance->CCR2 = pwm;    // Kênh 2 tắt
		} else if (Instance->hTIM->Config.Channel == TIM_CHANNEL_3) {
			Instance->hTIM->Instance->CCR3 = 0;  // PWM ra kênh 1
			Instance->hTIM->Instance->CCR4 = pwm;    // Kênh 2 tắt
		}
	}
	// Đợi đến khi encoder_total đạt 666

	while (abs((int16_t)(EncInstance->CNT)) < Enc);
	// Sau khi đạt 666, dừng động cơ

	L9110_Straight(Instance, Dir == L9110_DIRECTION_FORWARD ? L9110_DIRECTION_REVERSE :L9110_DIRECTION_FORWARD, 100);  // PWM 80%
	for (volatile int i = 0; i < 100000; i++);  // Delay nhỏ, có thể tăng giảm

	L9110_Stop(Instance);

}



