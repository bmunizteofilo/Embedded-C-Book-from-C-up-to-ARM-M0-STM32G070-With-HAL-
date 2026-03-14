#include <stdio.h>
#include <stdint.h>

#ifndef LOG_ENABLE
#define LOG_ENABLE 1
#endif

#define BIT(n) (1u << (n))
#define SET_BITS(reg, m) ((reg) |= (m))
#define CLR_BITS(reg, m) ((reg) &= ~(m))

#if defined(BOARD_NUCLEO_F070RB)
#define BOARD_NAME "NUCLEO_F070RB"
#define UART_DEFAULT_BAUD (115200u)
#elif defined(BOARD_FRDM_MCXC444)
#define BOARD_NAME "FRDM_MCXC444"
#define UART_DEFAULT_BAUD (9600u)
#else
#define BOARD_NAME "GENERICA"
#define UART_DEFAULT_BAUD (57600u)
#endif

#if LOG_ENABLE
#define LOG(fmt, ...) printf("[LOG] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG(...) do { } while (0)
#endif

/**
 * @brief Exercita macros de compilacao condicional e manipulacao de bits.
 *
 * @return Sempre retorna 0 ao final da execucao.
 */
int main(void)
{
    uint32_t reg = 0u;
    printf("board=%s baud=%lu\n", BOARD_NAME, (unsigned long)UART_DEFAULT_BAUD);
    LOG("LOG_ENABLE=%d", LOG_ENABLE);
    SET_BITS(reg, BIT(3));
    LOG("reg=%lu", (unsigned long)reg);
    CLR_BITS(reg, BIT(3));
    LOG("reg=%lu", (unsigned long)reg);

#define LOCAL_TIMEOUT_MS (1000u)
#undef LOCAL_TIMEOUT_MS
#define LOCAL_TIMEOUT_MS (250u)
    printf("timeout=%lu\n", (unsigned long)LOCAL_TIMEOUT_MS);
    return 0;
}
