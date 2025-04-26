/*
 * i2c_driver.h
 *
 *  Created on: Apr 22, 2025
 *      Author: tr6r2
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <stm32f103xx.h>
#include <usart_driver.h>

typedef enum {
	I2C_ACK_DISABLE = 0, I2C_ACK_ENABLE
} I2C_Acknowledge_t;

typedef enum {
	I2C_DUTYCYCLE_2 = 0, I2C_DUTYCYCLE_16_9
} I2C_DutyCycle_t;

typedef enum {
	I2C_ADDRESSINGMODE_7BIT = 0, I2C_ADDRESSINGMODE_10BIT
} I2C_AddressingMode_t;

typedef struct {
	uint32_t ClockSpeed;                     // Tốc độ I2C: 100kHz, 400kHz...
	I2C_AddressingMode_t AddressingMode;    // 7-bit hoặc 10-bit
	uint8_t OwnAddress;                     // Địa chỉ thiết bị (thường 7-bit)
	I2C_Acknowledge_t Acknowledge;          // ENABLE hoặc DISABLE ACK
	I2C_DutyCycle_t DutyCycle;              // Fast mode duty cycle
} I2C_Config_t;

typedef struct {
	I2C_TypeDef_t *Instance;   // Con trỏ tới I2Cx (I2C1, I2C2...)
	I2C_Config_t Config;        // Cấu hình khởi tạo
	uint8_t *pTxBuffer;  // Con trỏ buffer truyền
	uint8_t *pRxBuffer;  // Con trỏ buffer nhận
	uint32_t TxLen;       // Độ dài dữ liệu truyền
	uint32_t RxLen;       // Độ dài dữ liệu nhận
	uint8_t State;       // Trạng thái truyền/nhận
} I2C_Handle_t;

I2C_Handle_t I2C_Init(I2C_TypeDef_t *Instance, uint32_t ClockSpeed,
		I2C_AddressingMode_t AddressingMode, uint8_t OwnAddress,
		I2C_Acknowledge_t Acknowledge, I2C_DutyCycle_t DutyCycle) ;
void I2C_ScanDevices(I2C_Handle_t *hi2c, USART_Handle_t *hUsartx);


#endif /* I2C_DRIVER_H_ */
