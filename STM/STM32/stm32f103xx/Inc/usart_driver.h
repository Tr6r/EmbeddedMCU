/*
 * usart_driver.h
 *
 *  Created on: Apr 20, 2025
 *      Author: tr6r2
 */

#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include"stm32f103xx.h"

typedef enum {
	USART_WORDLENGTH_8B = 0,
	USART_WORDLENGTH_9B
} USART_WordLength_t;

typedef enum {
	USART_STOPBITS_1 = 0,
	USART_STOPBITS_2
} USART_StopBits_t;

typedef enum {
	USART_PARITY_NONE = 0,
	USART_PARITY_EVEN,
	USART_PARITY_ODD
} USART_Parity_t;

typedef enum {
	USART_MODE_RX = 1,
	USART_MODE_TX,
	USART_MODE_TX_RX
} USART_Mode_t;

typedef enum {
	USART_HW_NONE = 0,
	USART_HW_RTS,
	USART_HW_CTS,
	USART_HW_RTS_CTS
} USART_HWFlowControl_t;

typedef struct {
	uint32_t BaudRate;
	USART_WordLength_t WordLength;
	USART_StopBits_t StopBits;
	USART_Parity_t Parity;
	USART_Mode_t Mode;
	USART_HWFlowControl_t HWFlowControl;
} USART_Config_t;

typedef struct {
	USART_Config_t Config;
	USART_TypeDef_t *Instance;
} USART_Handle_t;

typedef enum {
	USART_BAUD_1200 = 1200,
	USART_BAUD_2400 = 2400,
	USART_BAUD_4800 = 4800,
	USART_BAUD_9600 = 9600,
	USART_BAUD_19200 = 19200,
	USART_BAUD_38400 = 38400,
	USART_BAUD_57600 = 57600,
	USART_BAUD_115200 = 115200,
	USART_BAUD_230400 = 230400,
	USART_BAUD_460800 = 460800,
	USART_BAUD_921600 = 921600
} USART_Baudrate_t;

USART_Handle_t USART_Init(USART_TypeDef_t *Instance, USART_Baudrate_t BaudRate,
		USART_WordLength_t WordLength, USART_StopBits_t StopBits,
		USART_Parity_t Parity, USART_Mode_t Mode,
		USART_HWFlowControl_t HWFlowControl);


void USART_SendData(USART_Handle_t *Instance, uint8_t data);
void USART_SendString(USART_Handle_t *Instance, const char *str);

#endif /* USART_DRIVER_H_ */
