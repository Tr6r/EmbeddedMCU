/*
 * gpio_driver.c
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */


#include "gpiox_driver.h"

GPIO_Handle_t GPIO_Init(GPIO_TypeDef* Instance, uint8_t Pin, GPIO_Mode_t Mode, GPIO_Speed_t Speed, GPIO_Pull_t Pull)
{
    GPIO_Handle_t hGPIO;

    // Cấu hình Instance và Pin
    hGPIO.Instance = Instance;
    hGPIO.GPIO_Config.Pin = Pin;
    hGPIO.GPIO_Config.Mode = Mode;
    hGPIO.GPIO_Config.Speed = Speed;
    hGPIO.GPIO_Config.Pull = Pull;

    // Enable clock cho GPIO
    if (Instance == GPIOA) GPIOA_EN_CLOCK();
    else if (Instance == GPIOB) GPIOB_EN_CLOCK();
    else if (Instance == GPIOC) GPIOC_EN_CLOCK();
    else if (Instance == GPIOD) GPIOD_EN_CLOCK();
    else if (Instance == GPIOE) GPIOE_EN_CLOCK();
    else if (Instance == GPIOF) GPIOF_EN_CLOCK();
    else if (Instance == GPIOG) GPIOG_EN_CLOCK();


    // Xác định chế độ (Mode) cho pin
    if (Mode == GPIO_MODE_INPUT)
    {

    }
    else if (Mode == GPIO_MODE_OUTPUT)
    {

    	if (Pin < 8)
    	    {
    			Instance->CRL |= (Speed << (Pin * 4));  // Set new speed bits
    	        Instance->CRL |= (Pull << (Pin * 4 + 2));  // Set new speed bits


    	    }
    	    else
    	    {
    	        // Pin từ 8 đến 15 sử dụng CRH
    	        Instance->CRH |= (Speed << ((Pin - 8) * 4));   // Set new mode bits
    	        Instance->CRH &= ~(1 << ((Pin - 8) * 4 + 2));  // Set new speed bits

    	        Instance->CRH |= (Pull << ((Pin - 8) * 4 + 2));  // Set new speed bits

    	    }
    }

    // Trả về GPIO Handle
    return hGPIO;
}
void GPIO_ClearPin(GPIO_Handle_t *xGPIO){
	xGPIO->Instance->ODR |= (1 << xGPIO->GPIO_Config.Pin);  // Enable pull-up

}

void GPIO_SetPin(GPIO_Handle_t *xGPIO){
	xGPIO->Instance->ODR &= ~(1 << xGPIO->GPIO_Config.Pin);  // Enable pull-up

}

