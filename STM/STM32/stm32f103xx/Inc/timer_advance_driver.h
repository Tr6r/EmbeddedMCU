/*
 * timer_advance_driver.h
 *
 *  Created on: Aug 31, 2025
 *      Author: tr6r2
 */

#ifndef TIMER_ADVANCE_DRIVER_H_
#define TIMER_ADVANCE_DRIVER_H_

#include "stm32f103xx.h"
#include "stdio.h"
#include "exti_driver.h"

/* --- Feature chọn cho Timer1 --- */
typedef enum {
    TIM1_FEATURE_NONE = 0,
    TIM1_FEATURE_PWM,
    TIM1_FEATURE_ENCODER
} TIM_Advance_Feature_t;

typedef enum {
    TIM1_CHANNEL_1 = 0,
    TIM1_CHANNEL_2,
    TIM1_CHANNEL_3,
    TIM1_CHANNEL_4
} TIM_Advance_Channel_t;

typedef enum {
    TIM1_OCMODE_PWM1 = 0x6,
    TIM1_OCMODE_PWM2 = 0x7
} TIM_Advance_OCMode_t;

/* --- Config struct --- */
typedef struct {
	TIM_Advance_Feature_t Feature;
    uint16_t Prescaler;
    uint32_t AutoReload;
    TIM_Advance_Channel_t Channel;
    TIM_Advance_OCMode_t OCMode;
    uint16_t CompareValue;
} TIMER_Advance_Config_t;

/* --- Handle --- */
typedef struct {
	TIMER_Advance_Config_t Config;
    TIMER_Advance_TypeDef_t *Instance;
} TIMER_Advance_Handle_t;

/* --- API --- */
void TIMER_Advance_Init(TIMER_Advance_Handle_t *hTimerAdvance);
void TIMER_Advance_StartPWM(TIMER_Advance_Handle_t *hTimerAdvance);
void TIMER_Advance_SetDuty(TIMER_Advance_Handle_t *hTimerAdvance, TIM_Advance_Channel_t channel, uint16_t duty);
void TIMER_Advance_StartEncoder(TIMER_Advance_Handle_t *hTimerAdvance);
int16_t TIMER_Advance_GetCount(TIMER_Advance_Handle_t *hTimerAdvance);


#endif /* TIMER_ADVANCE_DRIVER_H_ */
