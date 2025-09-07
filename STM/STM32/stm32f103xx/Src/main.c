#include <stm32f103xx.h>
#include <gpiox_driver.h>
#include <analog_driver.h>
#include <rcc_driver.h>
#include <timerx_driver.h>

#define NUM_CHANNELS 5
#define AVERAGE_SAMPLE 5

GPIO_Handle_t AnalogPins[NUM_CHANNELS];
RCC_Handle_t hRcc;
ADC_Handle_t hAdc;
TIMER2_5_Handle_t Timer2;

uint16_t adcValues[NUM_CHANNELS] = {0};
float distance_cm[NUM_CHANNELS] = {0.0f};
float filteredDistance[NUM_CHANNELS] = {0.0f};
float alpha = 0.3f; // hệ số lọc EMA

// Lookup table
float LookupDistance(uint16_t adcValue)
{
    const uint16_t adc_table[] = {1500, 1120, 860, 700, 500, 380, 250, 160, 90};
    const float distance_table[] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    const int table_size = sizeof(adc_table) / sizeof(adc_table[0]);

    if (adcValue >= adc_table[0]) return 2.0f;
    if (adcValue <= adc_table[table_size - 1]) return 10.0f;

    for (int i = 0; i < table_size - 1; i++) {
        if (adcValue <= adc_table[i] && adcValue >= adc_table[i+1]) {
            float ratio = (float)(adcValue - adc_table[i+1]) / (adc_table[i] - adc_table[i+1]);
            float distance = distance_table[i+1] + ratio * (distance_table[i] - distance_table[i+1]);
            distance = (float)((int)(distance * 100 + 0.5f)) / 100.0f; // làm tròn 2 chữ số
            return distance;
        }
    }
    return -1.0f; // lỗi
}

// Hàm EMA filter
float SmoothFilter(float previous, float current, float alpha)
{
    return alpha * current + (1.0f - alpha) * previous;
}

// Đọc trung bình ADC cho tất cả kênh
void Read_ADC_AllChannelsAverage(void)
{
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        uint32_t sum = 0;
        for (int i = 0; i < AVERAGE_SAMPLE; i++) {
            sum += ADC_ReadChannel(ADC1, (ADC_Channel_t)ch);
        }
        adcValues[ch] = (uint16_t)(sum / AVERAGE_SAMPLE);
    }
}

// Chuyển ADC -> cm
void Convert_ADC_to_Distance(void)
{
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        float d = LookupDistance(adcValues[ch]);
        filteredDistance[ch] = SmoothFilter(filteredDistance[ch], d, alpha);
        distance_cm[ch] = filteredDistance[ch];
    }
}

// Cấu hình GPIO analog
void IR_Init(void)
{
    for (int i = 0; i < NUM_CHANNELS; i++) {
        AnalogPins[i] = GPIO_Init(GPIOA, (1 << i), GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG);
    }

    hAdc.Instance = ADC1;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        hAdc.Config.channel[i] = (ADC_Channel_t)i;
    }
    hAdc.Config.numChannels = NUM_CHANNELS;
    hAdc.Config.sampleTime = ADC_SAMPLE_71CYCLES5;
    ADC_Init(&hAdc);
}

void SysClock_Init(void)
{
    hRcc.Config.clk_src = RCC_CLK_PLL;
    hRcc.Config.pll_src = RCC_PLL_SRC_HSE;
    hRcc.Config.pll_mul = RCC_PLL_MUL_9;
    hRcc.Config.ahb_prescaler = RCC_AHB_DIV_1;
    hRcc.Config.apb1_prescaler = RCC_APB_DIV_2;
    hRcc.Config.apb2_prescaler = RCC_APB_DIV_1;
    RCC_ConfigClock(&hRcc);
}

int main(void)
{
    SysClock_Init();
    IR_Init();
    Timer2 = TIMER2_5_Init_Delay(TIMER2, 719999);

    for (;;) {
        Read_ADC_AllChannelsAverage();
        Convert_ADC_to_Distance();

        // distance_cm[0..4] là A0..A4
        // làm PID hoặc xuất ra UART, LCD,...

        TIMER2_5_Delay(&Timer2, 99); // ~100ms
    }
}
