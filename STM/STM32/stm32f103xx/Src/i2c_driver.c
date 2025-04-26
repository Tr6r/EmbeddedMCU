/*
 * i2c_driver.c
 *
 *  Created on: Apr 22, 2025
 *      Author: tr6r2
 */

#include<i2c_driver.h>
#include<rcc_driver.h>

I2C_Handle_t I2C_Init(I2C_TypeDef_t *Instance, uint32_t ClockSpeed,
		I2C_AddressingMode_t AddressingMode, uint8_t OwnAddress,
		I2C_Acknowledge_t Acknowledge, I2C_DutyCycle_t DutyCycle) {
	I2C_Handle_t hI2cx;
	hI2cx.Instance = Instance;

	// Bật Clock cho I2C
	if (Instance == I2C1) {
		I2C1_EN_CLOCK();
	} else if (Instance == I2C2) {
		I2C2_EN_CLOCK();
	}

	// Reset I2C trước khi cấu hình
	Instance->CR1 |= (1 << 15);
	Instance->CR1 &= ~(1 << 15);

	uint32_t pclk1 = APB1_Clock / 1000000U;

	Instance->CR1 &= ~(1 << 0); // PE = 0

	// TRISE
	if (ClockSpeed <= 100000)
		Instance->TRISE = pclk1 + 1; // Standard mode
	else
		Instance->TRISE = ((pclk1 * 300) / 1000) + 1; // Fast mode ~300ns

	// Sau khi cấu hình TRISE, bật lại PE
	Instance->CR1 |= (1 << 0); // PE = 1

	Instance->CR2 = pclk1 & 0x3F; // CR2 chỉ dùng 6 bit thấp

	// Cấu hình CCR (Clock Control Register)
	uint16_t ccr_val = 0;
	if (ClockSpeed <= 100000) {
		// Standard mode
		ccr_val = (APB1_Clock / (ClockSpeed << 1));
		DutyCycle = I2C_DUTYCYCLE_2;
		Instance->CCR &= ~(1 << 15); // SM mode
	} else {
		// Fast mode
		Instance->CCR |= (1 << 15); // FM mode
		if (DutyCycle == I2C_DUTYCYCLE_16_9) {
			Instance->CCR |= (1 << 14); // Duty = 16/9
			ccr_val = (APB1_Clock / (ClockSpeed * 25));
		} else {
			Instance->CCR &= ~(1 << 14); // Duty = 2
			ccr_val = (APB1_Clock / (ClockSpeed * 3));
		}
	}
	Instance->CCR = ccr_val;

	// Acknowledge enable/disable
	if (Acknowledge == I2C_ACK_ENABLE)
		Instance->CR1 |= (1 << 10); // ACK bit
	else
		Instance->CR1 &= ~(1 << 10);

	// Enable I2C peripheral

	// Lưu lại config cho handle
	hI2cx.Config.ClockSpeed = ClockSpeed;
	hI2cx.Config.Acknowledge = Acknowledge;
	hI2cx.Config.AddressingMode = AddressingMode;
	hI2cx.Config.OwnAddress = OwnAddress;
	hI2cx.Config.DutyCycle = DutyCycle;

	return hI2cx;
}
void I2C_Write(I2C_Handle_t *hi2c, uint8_t Address, uint8_t *data, uint16_t size) {
    // Bắt đầu giao tiếp I2C: Gửi tín hiệu START
    hi2c->Instance->CR1 |= (1 << 8);  // Set START bit (Bit 8 của CR1)

    // Đợi tín hiệu START được xác nhận (SB = 1)
    while (!(hi2c->Instance->SR1 & (1 << 0)));  // Kiểm tra SB (Start Bit) trong SR1

    // Gửi địa chỉ I2C + bit 0 (Để chỉ ra là ghi dữ liệu)
    hi2c->Instance->DR = (Address << 1) & 0xFE;  // Địa chỉ I2C (Shift left 1 và bỏ bit viết)

    // Đợi đến khi địa chỉ được gửi thành công (ADDR bit trong SR1)
    while (!(hi2c->Instance->SR1 & (1 << 1)));  // Kiểm tra ADDR bit trong SR1

    // Xóa cờ ADDR trong SR2
    volatile uint32_t temp = hi2c->Instance->SR2;

    // Gửi dữ liệu
    for (uint16_t i = 0; i < size; i++) {
        hi2c->Instance->DR = data[i];  // Gửi dữ liệu vào Data Register

        // Đợi cho đến khi Data Register trống (TXE = 1)
        while (!(hi2c->Instance->SR1 & (1 << 7)));  // Kiểm tra TXE (Transmit Data Register Empty)
    }

    // Gửi tín hiệu STOP
    hi2c->Instance->CR1 |= (1 << 9);  // Set STOP bit (Bit 9 của CR1)

    // Đợi đến khi tín hiệu STOP được xác nhận
    while (hi2c->Instance->CR1 & (1 << 9));  // Kiểm tra Stop Bit trong CR1
}

void I2C_ScanDevices(I2C_Handle_t *hi2c, USART_Handle_t *hUsartx) {
	USART_SendString(hUsartx, "Scanning I2C devices...\r\n");

	for (uint8_t addr = 1; addr < 127; addr++) {
		// 1. Gửi START
		hi2c->Instance->CR1 |= (1 << 8); // START = Bit 8
		while (!(hi2c->Instance->SR1 & (1 << 0)))
			; // SB = Bit 0

		// 2. Gửi địa chỉ (ghi mode)
		hi2c->Instance->SR1; // Clear SB
		hi2c->Instance->DR = addr << 1; // 7-bit address, write bit = 0

		// 3. Đợi phản hồi: ADDR = bit 1, hoặc AF = bit 10
		while (!(hi2c->Instance->SR1 & ((1 << 1) | (1 << 10))))
			;

		if (hi2c->Instance->SR1 & (1 << 1)) {
			// Nếu nhận được phản hồi (ADDR)
			(void) hi2c->Instance->SR1; // Clear ADDR
			(void) hi2c->Instance->SR2;

			USART_SendString(hUsartx, "Found at: 0x");
			USART_SendHex(hUsartx, addr);
			USART_SendString(hUsartx, "\r\n");
		}

		// 4. Gửi STOP
		hi2c->Instance->CR1 |= (1 << 9); // STOP = Bit 9

		// 5. Clear lỗi AF nếu có (ACK fail)
		hi2c->Instance->SR1 &= ~(1 << 10); // Clear AF

		// Delay nhỏ để tránh bus lock
		for (volatile int i = 0; i < 1000; i++)
			;
	}

	USART_SendString(hUsartx, "Scan done.\r\n");
}

