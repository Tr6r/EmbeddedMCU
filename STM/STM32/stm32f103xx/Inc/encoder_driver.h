/*
 * encoder_driver.h
 *
 *  Created on: May 17, 2025
 *      Author: tr6r2
 */

#ifndef ENCODER_DRIVER_H_
#define ENCODER_DRIVER_H_

#include <stm32f103xx.h>
#include <timerx_driver.h>
typedef struct {
    int16_t count;             // Số xung hiện tại (CNT)
    int32_t last_count;        // Giá trị CNT trước đó (dùng để tính delta)
    int32_t total_count;       // Tổng xung đã tích lũy
    int32_t speed;             // Tốc độ tức thời (xung/đơn vị thời gian)
} ENC_Config_t;

typedef struct {
    TIMER2_5_Handle_t *htim;   // Timer đang dùng (ví dụ: TIM2, TIM3,...)
    ENC_Config_t Config;
} ENC_Handle_t;

int16_t ENC_Read(ENC_Handle_t *hEnc);


#endif /* ENCODER_DRIVER_H_ */
