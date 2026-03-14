#include <stdint.h>
#include "stm32g0xx.h"
#include "core_cm0plus.h"
#include "core_arm.h"

volatile uint32_t g_systick_ticks = 0u;
volatile uint32_t g_nmi_hits = 0u;
volatile uint32_t g_hardfault_hits = 0u;
volatile uint32_t g_shared_counter = 0u;
volatile uint32_t g_last_ipsr = 0u;
volatile uint32_t g_last_xpsr = 0u;
volatile uint32_t g_last_primask = 0u;

static void shared_counter_increment_atomic(void)
{
    uint32_t key = core_arm_irq_lock();
    g_shared_counter++;
    core_arm_irq_unlock(key);
}

void SysTick_Handler(void)
{
    g_systick_ticks++;
    g_last_ipsr = core_arm_get_ipsr();
    g_last_xpsr = core_arm_get_xpsr();
}

void NMI_Handler(void)
{
    g_nmi_hits++;
}

void HardFault_Handler(void)
{
    g_hardfault_hits++;
    while (1) {
        __NOP();
    }
}

int main(void)
{
    SystemCoreClockUpdate();

    g_last_primask = core_arm_get_primask();
    (void)core_arm_systick_init(SystemCoreClock, 1000u);

    for (;;) {
        shared_counter_increment_atomic();
        g_last_primask = core_arm_get_primask();
        __NOP();
    }
}
