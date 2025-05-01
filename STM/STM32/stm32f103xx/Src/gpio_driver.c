/*
 * gpio_driver.c
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */

#include "gpiox_driver.h"

GPIO_Handle_t GPIO_Init(GPIO_TypeDef *Instance, GPIO_Pin_t Pin,
		GPIO_Mode_t Mode, GPIO_CNF_t CNF) {



	GPIO_Handle_t hGPIO;

	// Cấu hình Instance và Pin
	hGPIO.Instance = Instance;
	hGPIO.GPIO_Config.Pin = Pin;
	hGPIO.GPIO_Config.Mode = Mode;
	hGPIO.GPIO_Config.CNF = CNF;

	// Enable clock cho GPIO
	if (Instance == GPIOA)
		GPIOA_EN_CLOCK();
	else if (Instance == GPIOB)
		GPIOB_EN_CLOCK();
	else if (Instance == GPIOC)
		GPIOC_EN_CLOCK();
	else if (Instance == GPIOD)
		GPIOD_EN_CLOCK();
	else if (Instance == GPIOE)
		GPIOE_EN_CLOCK();
	else if (Instance == GPIOF)
		GPIOF_EN_CLOCK();
	else if (Instance == GPIOG)
		GPIOG_EN_CLOCK();

	if (Mode == GPIO_MODE_INPUT) {
		uint8_t pinPos = (Pin % 8) * 4;

		if (Pin < 8) {
			Instance->CRL &= ~(0xF << pinPos);        // Clear config
			Instance->CRL |=  (0x8 << pinPos);        // Input with pull-up/down
			Instance->CRL &= ~(0b11 << 2);

			// Set CNF0 = 10 (Input with pull-up / pull-down)
			Instance->CRL |=  (0b10 << 2);
			Instance->ODR |= (1 << 0);


		} else {
			Instance->CRH &= ~(0xF << pinPos);        // Clear config
			Instance->CRH |=  (0x8 << pinPos);        // Input with pull-up/down
	        Instance->CRH |= (CNF << ((Pin - 8) * 4 + 2)); // Set CNF (2 bit)

		}
	} else {
		if (Pin < 8) {
			// Cấu hình CRL cho Pin 0-7
			Instance->CRL &= ~(0xF << (Pin * 4));  // Clear previous settings

			Instance->CRL |= (Mode << (Pin * 4)); // Set mode (2 bit)

			Instance->CRL |= (CNF << (Pin * 4 + 2)); // Set CNF (2 bit)
		} else {
			// Cấu hình CRH cho Pin 8-15
			Instance->CRH &= ~(0xF << ((Pin - 8) * 4)); // Clear previous settings
			Instance->CRH |= (Mode << ((Pin - 8) * 4)); // Set mode (2 bit)
			Instance->CRH |= (CNF << ((Pin - 8) * 4 + 2)); // Set CNF (2 bit)
		}

	}
	// Xác định chế độ (Mode) cho pin

	// Trả về GPIO Handle
	return hGPIO;
}
void GPIO_WritePin(GPIO_Handle_t *xGPIO, GPIO_State_t State) {
	if (State == GPIO_LOW)
		xGPIO->Instance->ODR &= ~(1 << xGPIO->GPIO_Config.Pin); // Enable pull-up
	else
		xGPIO->Instance->ODR |= (1 << xGPIO->GPIO_Config.Pin); // Enable pull-up

}
void GPIO_Toggle(GPIO_TypeDef *xGPIO, GPIO_Pin_t Pin)
{
    // Kiểm tra giá trị hiện tại của chân GPIO trong ODR
    if (xGPIO->ODR == 0)
    {
        xGPIO->ODR |=(1<<Pin);   // Đặt bit của chân để set thành HIGH

        // Nếu chân đang ở trạng thái HIGH, set thành LOW
    }
    else
    {
        // Nếu chân đang ở trạng thái LOW, set thành HIGH
        xGPIO->ODR &= ~(1<<Pin);  // Xóa bit của chân để set thành LOW

    }
}
