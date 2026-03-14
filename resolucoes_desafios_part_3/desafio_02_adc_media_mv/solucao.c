#include "stm32g0xx_hal.h"
#include <stdint.h>

#define ADC_SAMPLES 16u
#define ADC_MAX_12B 4095u
#define VREF_MV 3300u

extern ADC_HandleTypeDef hadc1;

static uint32_t adc_read_average_counts(ADC_HandleTypeDef *hadc)
{
    uint32_t sum = 0u;

    for (uint32_t i = 0u; i < ADC_SAMPLES; i++) {
        HAL_ADC_Start(hadc);
        HAL_ADC_PollForConversion(hadc, 10u);
        sum += HAL_ADC_GetValue(hadc);
        HAL_ADC_Stop(hadc);
    }

    return sum / ADC_SAMPLES;
}

static uint32_t adc_counts_to_mv(uint32_t counts)
{
    return (counts * VREF_MV) / ADC_MAX_12B;
}

int main(void)
{
    uint32_t avg_counts;
    uint32_t sensor_mv;

    HAL_Init();
    SystemClock_Config();
    MX_ADC1_Init();

    for (;;) {
        avg_counts = adc_read_average_counts(&hadc1);
        sensor_mv = adc_counts_to_mv(avg_counts);
        (void)sensor_mv;
        HAL_Delay(20u);
    }
}
