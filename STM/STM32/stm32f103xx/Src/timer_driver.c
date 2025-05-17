/*
 * timer_driver.c
 *
 *  Created on: Apr 5, 2025
 *      Author: tr6r2
 */
#include "timerx_driver.h"

TIMER2_5_Handle_t TIMER2_5_Init_Delay(TIMER2_5_TypeDef_t *Instance,
		uint32_t Prs) {
	TIMER2_5_Handle_t hTIM2_5;

	hTIM2_5.Instance = Instance;
	hTIM2_5.Config.Prs = Prs;

	Instance->CR1 &= ~(1 << 0);

	// Enable clock
	if (Instance == TIMER2)
		TIMER2_EN_CLOCK();
	else if (Instance == TIMER3)
		TIMER3_EN_CLOCK();
	else if (Instance == TIMER4)
		TIMER4_EN_CLOCK();
	else if (Instance == TIMER5)
		TIMER5_EN_CLOCK();

	// Set prescaler and auto-reload
	Instance->PSC = Prs;

	Instance->CNT = 0;
	// Enable counter
//	Instance->CR1 |= (1 << 0); // CEN = 1

	return hTIM2_5;
}
void TIM2_5_Init(TIMER2_5_Handle_t *hTimerx) {
	if (hTimerx->Instance == TIMER2)
		TIMER2_EN_CLOCK();
	else if (hTimerx->Instance == TIMER3)
		TIMER3_EN_CLOCK();
	else if (hTimerx->Instance == TIMER4)
		TIMER4_EN_CLOCK();
	else if (hTimerx->Instance == TIMER5)
		TIMER5_EN_CLOCK();

	switch (hTimerx->Config.Feature) {
	case TIM_FEATURE_DELAY:

		hTimerx->Instance->PSC = hTimerx->Config.Prs;

		hTimerx->Instance->CNT = 0;

		break;
	case TIM_FEATURE_ENCODER_MODE:

		hTimerx->Instance->SMCR &= ~(0x7);            // Xóa 3 bit SMS (bit 2:0)
		hTimerx->Instance->SMCR |= hTimerx->Config.SMode; // Gán mode encoder

		// 3. Đặt CC1S = 01 và CC2S = 01 để nhận tín hiệu input (IC1, IC2)
		if (hTimerx->Config.Channel == TIM_CHANNEL_1
				|| hTimerx->Config.Channel == TIM_CHANNEL_2) {
			// Xử lý encoder trên CC1 + CC2
			hTimerx->Instance->CCMR1 &= ~((3 << 0) | (3 << 8)); // Clear CC1S + CC2S
			hTimerx->Instance->CCMR1 |= (1 << 0) | (1 << 8); // CC1S = 01, CC2S = 01 (input)

			hTimerx->Instance->CCMR1 |= (0b0011 << 4); // IC1F = fSAMPLING=fdts/2, N=8
			hTimerx->Instance->CCMR1 |= (0b0011 << 12); // IC2F = fSAMPLING=fdts/2, N=8

			hTimerx->Instance->CCER |= (1 << 0) | (1 << 1) | (1 << 4)
					| (1 << 5);    // CC1E = 1, CC2E = 1

		} else if (hTimerx->Config.Channel == TIM_CHANNEL_3
				|| hTimerx->Config.Channel == TIM_CHANNEL_4) {
			// Xử lý encoder trên CC3 + CC4
			hTimerx->Instance->CCMR2 &= ~((3 << 0) | (3 << 8)); // Clear CC3S + CC4S
			hTimerx->Instance->CCMR2 |= (1 << 0) | (1 << 8); // CC3S = 01, CC4S = 01 (input)
			hTimerx->Instance->CCMR2 |= (0b0011 << 4); // IC1F = fSAMPLING=fdts/2, N=8
			hTimerx->Instance->CCMR2 |= (0b0011 << 12); // IC2F = fSAMPLING=fdts/2, N=8
			hTimerx->Instance->CCER |= (1 << 8) | (1 << 9) | (1 << 12)
					| (1 << 13);    // CC1E = 1, CC2E = 1

		}

		// 4. Không đảo pha - nếu cần đảo thì set bit CC1P / CC2P
//		hTimerx->Instance->CCER &= ~((1 << 1) | (1 << 5)); // CC1P = 0, CC2P = 0

		// 6. Đặt ARR và PSC
		hTimerx->Instance->ARR = hTimerx->Config.Arr;
		hTimerx->Instance->PSC = hTimerx->Config.Prs;
//		hTimerx->Instance->DIER |= (1<<0);

		// 7. Reset counter
		hTimerx->Instance->CR1 |= (1 << 0);
		hTimerx->Instance->CNT = 0;
//		NVIC_EnableIRQ(TIM2_IRQn);

		break;
	case TIM_FEATURE_PWM:
		hTimerx->Instance->PSC = hTimerx->Config.Prs; // (72 MHz / (71 + 1)) = 1 MHz (Timer frequency)
		hTimerx->Instance->ARR = hTimerx->Config.Arr; // (1 MHz / 50) = 20kHz (PWM frequency)

		// Cấu hình PWM Mode 1 cho TIM2 CH1
		hTimerx->Instance->CCMR1 &= ~0xFF;
		hTimerx->Instance->CCMR2 &= ~0xFF;

		if (hTimerx->Config.Channel == TIM_CHANNEL_1) {
			// Channel 1 (CCMR1)
			hTimerx->Instance->CCMR1 |= (hTimerx->Config.OCMode << 4) | (hTimerx->Config.OCMode << 12); // OC1M = 110 (PWM Mode 1) cho Channel 1
			hTimerx->Instance->CCMR1 |= (1 << 3) | (1 << 11); // OC1PE = 1 (PWM with preload) cho Channel 1
			hTimerx->Instance->CCER |= (1 << 0) | (1 << 4); // CC1E = 1 (Enable output) cho Channel 1
		}

		else if (hTimerx->Config.Channel == TIM_CHANNEL_3) {
			// Channel 3 (CCMR2)
			hTimerx->Instance->CCMR2 |= (hTimerx->Config.OCMode << 4) | (hTimerx->Config.OCMode << 12); // OC3M = 110 (PWM Mode 1) cho Channel 3
			hTimerx->Instance->CCMR2 |= (1 << 3)| (1 << 11); // OC3PE = 1 (PWM with preload) cho Channel 3
			hTimerx->Instance->CCER |= (1 << 8)| (1 << 4); // CC3E = 1 (Enable output) cho Channel 3

		}


		// Enable Auto-reload Preload (ARPE)
		hTimerx->Instance->CR1 |= (1 << 7);     // ARPE = 1

		// Start Timer (Enable counter)
		hTimerx->Instance->CR1 |= (1 << 0);     // CEN = 1 (Enable counter)

		// Force update to load all registers (required after changes to ARR or PSC)
		hTimerx->Instance->EGR |= 1;           // UG = 1 (Generate update event)
		break;
		// Có thể thêm các case khác nếu cần
	default:
		// Xử lý trường hợp mặc định nếu không khớp với bất kỳ giá trị nào
		break;
	}

}

void TIMER2_5_Delay(TIMER2_5_Handle_t *hTimerx, uint32_t Arr) {
	// Cấu hình giá trị ARR (Auto-reload register) cho thời gian delay
	hTimerx->Instance->ARR = Arr;
	hTimerx->Instance->SR &= ~(1 << 0);  // Clear the UIF flag

	// Đặt lại CNT về 0
	hTimerx->Instance->CNT = 0;
	hTimerx->Instance->CR1 |= (1 << 0); // CEN = 1

	// Đợi cho đến khi CNT đạt ARR (timer tràn)
	while (!(hTimerx->Instance->SR & (1 << 0))) {
	}

}

uint32_t Read_Encoder(TIMER2_5_TypeDef_t *Timerx, TIM_Channel_t channel) {
	uint32_t encoder_value = 0;

	// Kiểm tra kênh nào đang được chọn và lấy giá trị tương ứng từ CNT
	switch (channel) {
	case TIM_CHANNEL_1:
		// Đọc giá trị từ CC1
		encoder_value = (int32_t) (Timerx->CCR1);
		break;

	case TIM_CHANNEL_2:
		// Đọc giá trị từ CC2
		encoder_value = (int32_t) (Timerx->CCR2);
		break;

	case TIM_CHANNEL_3:
		// Đọc giá trị từ CC3
		encoder_value = (int32_t) (Timerx->CCR3);
		break;

	case TIM_CHANNEL_4:
		// Đọc giá trị từ CC4
		encoder_value = (int32_t) (Timerx->CCR4);
		break;

	default:
		// Nếu không phải kênh hợp lệ, trả về 0
		encoder_value = 0;
		break;
	}

	// Trả về giá trị encoder
	return encoder_value;
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

