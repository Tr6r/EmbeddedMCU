/*
 * timerx_driver.h
 *
 *  Created on: Apr 5, 2025
 *      Author: tr6r2
 */

#ifndef TIMERX_DRIVER_H_
#define TIMERX_DRIVER_H_

#include"stm32f103xx.h"
#include"stdio.h"
typedef enum {
    TIM_MODE_UP,       // Đếm lên
    TIM_MODE_DOWN,     // Đếm xuống (ít dùng với TIM2-5)
    TIM_MODE_CENTER    // Center-aligned mode (cho PWM nâng cao)
} TIM_Mode_t;

typedef enum {
    TIM_CHANNEL_1 = 0,
    TIM_CHANNEL_2 = 1,
    TIM_CHANNEL_3 = 2,
    TIM_CHANNEL_4 = 3
} TIM_Channel_t;

typedef enum {
    TIM_OCMODE_FROZEN       = 0x0,
    TIM_OCMODE_ACTIVE       = 0x1,
    TIM_OCMODE_INACTIVE     = 0x2,
    TIM_OCMODE_TOGGLE       = 0x3,
    TIM_OCMODE_FORCE_LOW    = 0x4,
    TIM_OCMODE_FORCE_HIGH   = 0x5,
    TIM_OCMODE_PWM1         = 0x6,
    TIM_OCMODE_PWM2         = 0x7
} TIM_OCMode_t;

typedef struct
{
	TIM_Mode_t Mode;
	uint16_t Prs;
	uint16_t Arr;
	TIM_Channel_t Channel;
	TIM_OCMode_t OCMode;
	uint16_t  CompareValue;


} TIMER2_5_Config_t;

typedef struct
{
	TIMER2_5_Config_t Config;
	TIMER2_5_TypeDef_t* Instance;
}TIMER2_5_Handle_t;


TIMER2_5_Handle_t TIMER2_5_Init_Delay(TIMER2_5_TypeDef_t* Instance, uint16_t Prs);
void TIMER2_5_Delay(TIMER2_5_Handle_t* hTimerx, uint16_t Arr);
//TIMER2_5_Handle_t Init_StepMotor_Base(TIMER2_5_TypeDef_t* Instance, uint16_t Prs, uint16_t Arr);
//void Init_StepMotorChannel(TIMER2_5_TypeDef_t* Instance, TIM_Channel_t Channel,TIM_OCMode_t OCMode, uint16_t CompareValue);
//
//void StepMotor_Rotate(TIMER2_5_Handle_t* htim, TIM_Channel_t channel, float angle);

#endif /* TIMERX_DRIVER_H_ */
