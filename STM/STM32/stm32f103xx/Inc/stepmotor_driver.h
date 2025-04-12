/*
 * stepmotor_driver.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tr6r2
 */

#ifndef STEPMOTOR_DRIVER_H_
#define STEPMOTOR_DRIVER_H_

#include <stdio.h>
#include "stm32f103xx.h"
#include "gpiox_driver.h"
#include "timerx_driver.h"

typedef struct{
	TIMER2_5_Handle_t *TIM;
	GPIO_Handle_t *STEP;
	GPIO_Handle_t *DIR;

}STEPMOTOR_Handle_t;

STEPMOTOR_Handle_t StepMotor_Init(TIMER2_5_Handle_t *TIM,GPIO_Handle_t *STEP,GPIO_Handle_t *DIR );
void StepMotor_Rotate(STEPMOTOR_Handle_t* hStepMotor, uint16_t angle, uint8_t delay);
void StepMotor_SetDirection(STEPMOTOR_Handle_t* hStepMotor, uint8_t direction);

#endif /* STEPMOTOR_DRIVER_H_ */
