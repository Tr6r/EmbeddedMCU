/*
 * gpiox_driver.h
 *
 *  Created on: Apr 4, 2025
 *      Author: tr6r2
 */

#ifndef GPIOX_DRIVER_H_
#define GPIOX_DRIVER_H_

#include "stm32f103xx.h"

typedef enum {
    GPIO_MODE_INPUT = 0x00,
    GPIO_MODE_OUTPUT = 0x01,
    GPIO_MODE_AF = 0x02,
    GPIO_MODE_ANALOG = 0x03
} GPIO_Mode_t;

typedef enum {
    GPIO_SPEED_LOW = 0x02,
    GPIO_SPEED_MEDIUM = 0x01,
    GPIO_SPEED_HIGH = 0x03,
} GPIO_Speed_t;

typedef enum {
    GPIO_PUSH_PULL =0,
    GPIO_OPENDRAIN,
} GPIO_Pull_t;


typedef struct
{
    GPIO_Mode_t Mode;
    uint8_t Pin;
    GPIO_Speed_t Speed;
    GPIO_Pull_t Pull;

} GPIO_Config_t;

typedef struct
{
    GPIO_Config_t GPIO_Config;
    GPIO_TypeDef *Instance;
} GPIO_Handle_t;

GPIO_Handle_t GPIO_Init(GPIO_TypeDef* Instance, uint8_t Pin,GPIO_Mode_t Mode, GPIO_Speed_t Speed,GPIO_Pull_t Pull );
void GPIO_SetPin(GPIO_Handle_t *xGPIO);
void GPIO_ClearPin(GPIO_Handle_t *xGPIO);



#endif /* GPIOX_DRIVER_H_ */
