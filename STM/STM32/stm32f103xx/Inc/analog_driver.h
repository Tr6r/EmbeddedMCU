/*
 * analog_driver.h
 *
 *  Created on: May 12, 2025
 *      Author: tr6r2
 */

#ifndef ANALOG_DRIVER_H_
#define ANALOG_DRIVER_H_

#include "stm32f103xx.h"
#define ADC_MAX_CHANNELS 8

typedef enum {
    ADC_CHANNEL_0  = 0,
    ADC_CHANNEL_1  = 1,
    ADC_CHANNEL_2  = 2,
    ADC_CHANNEL_3  = 3,
    ADC_CHANNEL_4  = 4,
    ADC_CHANNEL_5  = 5,
    ADC_CHANNEL_6  = 6,
    ADC_CHANNEL_7  = 7,
    ADC_CHANNEL_8  = 8,
    ADC_CHANNEL_9  = 9,
    ADC_CHANNEL_10 = 10,
    ADC_CHANNEL_11 = 11,
    ADC_CHANNEL_12 = 12,
    ADC_CHANNEL_13 = 13,
    ADC_CHANNEL_14 = 14,
    ADC_CHANNEL_15 = 15,
    ADC_CHANNEL_16 = 16,
    ADC_CHANNEL_17 = 17
} ADC_Channel_t;

typedef enum {
    ADC_SAMPLE_1CYCLE5   = 0,
    ADC_SAMPLE_7CYCLES5  = 1,
    ADC_SAMPLE_13CYCLES5 = 2,
    ADC_SAMPLE_28CYCLES5 = 3,
    ADC_SAMPLE_41CYCLES5 = 4,
    ADC_SAMPLE_55CYCLES5 = 5,
    ADC_SAMPLE_71CYCLES5 = 6,
    ADC_SAMPLE_239CYCLES5 = 7
} ADC_SampleTime_t;



typedef struct {
    ADC_Channel_t channel[ADC_MAX_CHANNELS];  // mảng channel cố định
    uint8_t numChannels;                       // số lượng channel đang dùng
    ADC_SampleTime_t sampleTime;               // thời gian lấy mẫu chung (hoặc theo channel)
} ADC_Config_t;

typedef struct {
	ADC_Config_t   Config;
	ADC_TypeDef_t *Instance;
} ADC_Handle_t;

void ADC_Init(ADC_Handle_t *hADC);
uint16_t ADC_ReadChannel(ADC_TypeDef_t *ADCx, ADC_Channel_t channel);
#endif /* ANALOG_DRIVER_H_ */
