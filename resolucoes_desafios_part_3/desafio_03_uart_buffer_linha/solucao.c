#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <string.h>

#define UART_RX_BUF_LEN 64u

extern UART_HandleTypeDef huart2;

static volatile uint8_t g_rx_byte = 0u;
static volatile uint8_t g_line_ready = 0u;
static volatile uint8_t g_rx_overflow = 0u;
static char g_line_buf[UART_RX_BUF_LEN];
static uint32_t g_line_len = 0u;

static void uart_start_rx(void)
{
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&g_rx_byte, 1u);
}

static void uart_process_line(void)
{
    if (g_line_ready == 0u) {
        return;
    }

    g_line_ready = 0u;

    if (strcmp(g_line_buf, "LED ON\n") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else if (strcmp(g_line_buf, "LED OFF\n") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }

    g_line_len = 0u;
    g_line_buf[0] = '\0';
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        if (g_line_ready == 0u) {
            if (g_line_len < (UART_RX_BUF_LEN - 1u)) {
                g_line_buf[g_line_len++] = (char)g_rx_byte;
                g_line_buf[g_line_len] = '\0';

                if (g_rx_byte == '\n') {
                    g_line_ready = 1u;
                }
            } else {
                g_rx_overflow = 1u;
                g_line_len = 0u;
                g_line_buf[0] = '\0';
            }
        }

        uart_start_rx();
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    uart_start_rx();

    for (;;) {
        uart_process_line();
        (void)g_rx_overflow;
    }
}
