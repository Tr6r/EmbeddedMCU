/*
 * analog_driver.c
 *
 *  Created on: May 12, 2025
 *      Author: tr6r2
 */

#include <analog_driver.h>

void ADC_Init(ADC_Handle_t *hADC) {
	uint8_t sampleTime = hADC->Config.sampleTime;

	// Bật clock
	if (hADC->Instance == ADC1)
		ADC1_EN_CLOCK();
	else
		ADC2_EN_CLOCK();

	// Cấu hình sample time cho từng kênh
	for (uint8_t i = 0; i < hADC->Config.numChannels; i++) {
		uint8_t channel = hADC->Config.channel[i];

		if (channel <= 9) {
			hADC->Instance->SMPR2 &= ~(0x7 << (channel * 3));
			hADC->Instance->SMPR2 |= (sampleTime << (channel * 3));
		} else {
			uint8_t ch = channel - 10;
			hADC->Instance->SMPR1 &= ~(0x7 << (ch * 3));
			hADC->Instance->SMPR1 |= (sampleTime << (ch * 3));
		}
	}

	// Bật ADC (ADON 2 lần)
	hADC->Instance->CR2 |= (1 << 0);
	for (volatile int j = 0; j < 1000; j++)
		;
	hADC->Instance->CR2 |= (1 << 0);
}

uint16_t ADC_ReadChannel(ADC_TypeDef_t *ADCx, ADC_Channel_t channel) {
	// Cấu hình kênh cần đọc vào vị trí đầu tiên của chuỗi (SQR3)
	ADCx->SQR3 = channel;

	// Chỉ định số lần chuyển đổi là 1 (L = 0)
	ADCx->SQR1 &= ~(0xF << 20);  // Clear L[3:0]
	ADCx->SQR1 |= (0 << 20);     // L = 0 (1 conversion)

	// Bắt đầu chuyển đổi
	ADCx->CR2 |= (1 << 30);      // SWSTART

	// Chờ kết thúc
	while (!(ADCx->SR & (1 << 1)))
		;  // EOC (End of conversion)

	// Đọc giá trị
	return ADCx->DR;
}

