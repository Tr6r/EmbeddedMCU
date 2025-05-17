/*
 * L9110_driver.h
 *
 *  Created on: May 6, 2025
 *      Author: tr6r2
 */

#ifndef L9110_DRIVER_H_
#define L9110_DRIVER_H_

#include <stm32f103xx.h>
#include <timerx_driver.h>
#include <gpiox_driver.h>
typedef enum {
    L9110_DIRECTION_FORWARD = 0,  // Cùng chiều
    L9110_DIRECTION_REVERSE = 1   // Ngược chiều
} L9110_Direction_t;

typedef struct
{
    GPIO_Handle_t *PWMA_Pin;
    GPIO_Handle_t *PWMB_Pin;
    TIMER2_5_Handle_t *hTIM;
} L9110_Handle_t;

void L9110_Straight(L9110_Handle_t *Instance,L9110_Direction_t Dir, uint16_t pwm);
void L9110_Stop(L9110_Handle_t *Instance);
void L9110_Straight_Dis(L9110_Handle_t *Instance, L9110_Direction_t Dir, uint16_t pwm, volatile int32_t *encoder_total);


#endif /* L9110_DRIVER_H_ */
