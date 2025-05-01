/*
 * usart_driver.c
 *
 *  Created on: Apr 20, 2025
 *      Author: tr6r2
 */

#include "usart_driver.h"

uint32_t USART_CalculateBRR(uint32_t pclk, uint32_t baudRate) {
	float usartDiv = (float) pclk / (16.0f * baudRate);
	uint32_t mantissa = (uint32_t) usartDiv;
	uint32_t fraction = (uint32_t) ((usartDiv - mantissa) * 16.0f);
	return (mantissa << 4) | (fraction & 0xF);
}

USART_Handle_t USART_Init(USART_TypeDef_t *Instance, USART_Baudrate_t BaudRate,
		USART_WordLength_t WordLength, USART_StopBits_t StopBits,
		USART_Parity_t Parity, USART_Mode_t Mode,
		USART_HWFlowControl_t HWFlowControl) {

	USART_Handle_t hUsartx;
	hUsartx.Instance = Instance;
	uint32_t pclk;

	uint32_t APB2_Clock;
	uint32_t APB1_Clock;
	if (Instance == USART1) {
		USART1_EN_CLOCK();
		pclk = APB2_Clock;
	} else if (Instance == USART2) {
		USART2_EN_CLOCK();
		pclk = APB1_Clock;
	} else if (Instance == USART3) {
		USART3_EN_CLOCK();
		pclk = APB1_Clock;
	} else if (Instance == UART4) {
		UART4_EN_CLOCK();
		pclk = APB1_Clock;
	} else if (Instance == UART5) {
		UART5_EN_CLOCK();
		pclk = APB1_Clock;
	}

	uint32_t brrValue = USART_CalculateBRR(pclk, BaudRate);
	hUsartx.Instance->BRR = 0x1D4C;
	// Cấu hình Word length
	if (WordLength == USART_WORDLENGTH_9B)
		hUsartx.Instance->CR1 |= (1 << 12); // M = 1: 9 bits
	else
		hUsartx.Instance->CR1 &= ~(1 << 12); // M = 0: 8 bits

	// Cấu hình Parity
	if (Parity == USART_PARITY_NONE) {
		hUsartx.Instance->CR1 &= ~(1 << 10); // Tắt parity
	} else {
		hUsartx.Instance->CR1 |= (1 << 10); // Bật parity
		if (Parity == USART_PARITY_ODD)
			hUsartx.Instance->CR1 |= (1 << 9); // ODD
		else
			hUsartx.Instance->CR1 &= ~(1 << 9); // EVEN
	}

	// Cấu hình Stop bits
	hUsartx.Instance->CR2 &= ~(3 << 12);         // Xoá trước
	hUsartx.Instance->CR2 |= (StopBits << 12);   // Gán Stop bits

	// Cấu hình Mode
	if (Mode & USART_MODE_TX)
		hUsartx.Instance->CR1 |= (1 << 3); // TE
	if (Mode & USART_MODE_RX)
		hUsartx.Instance->CR1 |= (1 << 2); // RE

	// Cấu hình Hardware Flow Control
	if (HWFlowControl == USART_HW_RTS)
		hUsartx.Instance->CR3 |= (1 << 8); // RTS enable
	else if (HWFlowControl == USART_HW_CTS)
		hUsartx.Instance->CR3 |= (1 << 9); // CTS enable
	else if (HWFlowControl == USART_HW_RTS_CTS)
		hUsartx.Instance->CR3 |= (1 << 8) | (1 << 9);

	// Bật USART
	hUsartx.Instance->CR1 |= (1 << 13); // UE = 1

	return hUsartx;
}

// Hàm gửi một byte qua USART sử dụng USART_Handle_t
void USART_SendData(USART_Handle_t *Instance, uint8_t data) {
	// Chờ cho đến khi TXE (Transmit Data Register Empty) có giá trị 1, tức là USART sẵn sàng gửi
	while (!(Instance->Instance->SR & (1 << 7))); // Đợi TXE (Bit 7 trong SR)

	// Gửi dữ liệu vào Data Register (DR)
	Instance->Instance->DR = data;
}

// Hàm gửi chuỗi ký tự qua USART sử dụng USART_Handle_t
void USART_SendString(USART_Handle_t *Instance, const char *str) {
	while (*str) {
		USART_SendData(Instance, *str); // Gửi từng ký tự trong chuỗi
		str++;
	}
}

void USART_SendHex(USART_Handle_t *Instance, uint8_t value) {
    const char hexChars[] = "0123456789ABCDEF";
    char str[3];
    str[0] = hexChars[(value >> 4) & 0x0F];
    str[1] = hexChars[value & 0x0F];
    str[2] = '\0';
    USART_SendString(Instance, str);
}

