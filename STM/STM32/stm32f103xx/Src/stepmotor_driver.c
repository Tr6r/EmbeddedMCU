/*
 * stepmotor_driver.c
 *
 *  Created on: Apr 8, 2025
 *      Author: tr6r2
 */


#include "stepmotor_driver.h"

STEPMOTOR_Handle_t StepMotor_Init(TIMER2_5_Handle_t *TIM,GPIO_Handle_t *STEP,GPIO_Handle_t *DIR)
{
	STEPMOTOR_Handle_t hStepMotor;
	hStepMotor.TIM = TIM;
	hStepMotor.STEP = STEP;
	hStepMotor.DIR = DIR;

	return hStepMotor;
}
void StepMotor_Rotate(STEPMOTOR_Handle_t* hStepMotor, uint16_t angle, uint8_t delay)
{
	uint16_t step = ((uint32_t)angle * 200) / 360;

	for (uint16_t i = 0; i < step; i++) {
		GPIO_WritePin(hStepMotor->STEP, 1);
		TIMER2_5_Delay(hStepMotor->TIM, delay);
		GPIO_WritePin(hStepMotor->STEP, 0);
		TIMER2_5_Delay(hStepMotor->TIM, delay);
	}
}

void StepMotor_SetDirection(STEPMOTOR_Handle_t* hStepMotor, uint8_t direction)
{
    GPIO_WritePin(hStepMotor->DIR, direction ? 1 : 0);
}
