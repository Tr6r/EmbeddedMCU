/*
 * L9110_driver.c
 *
 *  Created on: May 6, 2025
 *      Author: tr6r2
 */


#include <L9110_driver.h>

void L9110_Straight(L9110_Handle_t *Instance, L9110_Direction_t Dir, uint16_t pwm)
{
    if (Dir == L9110_DIRECTION_FORWARD)
    {
        // Đặt DIR = LOW, PWM = PWM value
        GPIO_WritePin(Instance->Dir_Pin, GPIO_LOW); // hoặc 0
    }
    else if (Dir == L9110_DIRECTION_REVERSE)
    {
        // Đặt DIR = HIGH, PWM = PWM value
        GPIO_WritePin(Instance->Dir_Pin, GPIO_HIGH); // hoặc 1
    }

    // Cập nhật giá trị Compare cho PWM
    switch (Instance->hTIM->Config.Channel)
    {
        case TIM_CHANNEL_1:
            Instance->hTIM->Instance->CCR1 = pwm;
            break;
        case TIM_CHANNEL_2:
            Instance->hTIM->Instance->CCR2 = pwm;
            break;
        case TIM_CHANNEL_3:
            Instance->hTIM->Instance->CCR3 = pwm;
            break;
        case TIM_CHANNEL_4:
            Instance->hTIM->Instance->CCR4 = pwm;
            break;
    }
}

void L9110_Stop(L9110_Handle_t *Instance)
{
    // Set PWM compare value to 0 to stop the motor
    switch (Instance->hTIM->Config.Channel)
    {
        case TIM_CHANNEL_1:
            Instance->hTIM->Instance->CCR1 = 0;
            break;
        case TIM_CHANNEL_2:
            Instance->hTIM->Instance->CCR2 = 0;
            break;
        case TIM_CHANNEL_3:
            Instance->hTIM->Instance->CCR3 = 0;
            break;
        case TIM_CHANNEL_4:
            Instance->hTIM->Instance->CCR4 = 0;
            break;
    }
}

void L9110_Straight_Dis(L9110_Handle_t *Instance, L9110_Direction_t Dir, uint16_t pwm, volatile int32_t *encoder_total)
{
    // Chạy motor theo hướng Dir
    if (Dir == L9110_DIRECTION_FORWARD)
    {
        GPIO_WritePin(Instance->Dir_Pin, GPIO_HIGH);  // hoặc LOW tùy mạch
    }
    else if (Dir == L9110_DIRECTION_REVERSE)
    {
        GPIO_WritePin(Instance->Dir_Pin, GPIO_LOW);   // hoặc HIGH tùy mạch
    }

    // Bắt đầu cấp PWM
    switch (Instance->hTIM->Config.Channel)
    {
        case TIM_CHANNEL_1:
            Instance->hTIM->Instance->CCR1 = pwm;
            break;
        case TIM_CHANNEL_2:
            Instance->hTIM->Instance->CCR2 = pwm;
            break;
        case TIM_CHANNEL_3:
            Instance->hTIM->Instance->CCR3 = pwm;
            break;
        case TIM_CHANNEL_4:
            Instance->hTIM->Instance->CCR4 = pwm;
            break;
    }

    // Đợi đến khi encoder_total đạt 666
    while (*encoder_total < 666);

    // Sau khi đạt 666, dừng động cơ
    L9110_Stop(Instance);
}
