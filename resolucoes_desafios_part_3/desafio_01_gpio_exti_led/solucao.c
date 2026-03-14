#include "stm32g0xx_hal.h"
#include <stdint.h>

#define LED_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define BTN_GPIO_Port GPIOC
#define BTN_Pin GPIO_PIN_13
#define BTN_IRQn EXTI4_15_IRQn
#define DEBOUNCE_MS 50u

static volatile uint8_t g_btn_irq_flag = 0u;
static volatile uint32_t g_btn_irq_tick = 0u;

static void button_process(void)
{
    if (g_btn_irq_flag == 0u) {
        return;
    }

    if ((HAL_GetTick() - g_btn_irq_tick) < DEBOUNCE_MS) {
        return;
    }

    g_btn_irq_flag = 0u;

    if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BTN_Pin) {
        g_btn_irq_tick = HAL_GetTick();
        g_btn_irq_flag = 1u;
    }
}

void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_Pin);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    for (;;) {
        button_process();
    }
}
