#include "core_arm.h"
#include "core_cm0plus.h"

uint32_t core_arm_get_ipsr(void)
{
    return __get_IPSR();
}

uint32_t core_arm_get_xpsr(void)
{
    return __get_xPSR();
}

uint32_t core_arm_get_primask(void)
{
    return __get_PRIMASK();
}

uint32_t core_arm_irq_lock(void)
{
    uint32_t key = __get_PRIMASK();
    __disable_irq();
    __DMB();
    return key;
}

void core_arm_irq_unlock(uint32_t key)
{
    __DMB();
    __set_PRIMASK(key);
}

int core_arm_systick_init(uint32_t core_clk_hz, uint32_t tick_hz)
{
    uint32_t reload;

    if ((core_clk_hz == 0u) || (tick_hz == 0u)) {
        return -1;
    }

    reload = (core_clk_hz / tick_hz);
    if ((reload == 0u) || (reload > 0x01000000u)) {
        return -1;
    }

    SysTick->CTRL = 0u;
    SysTick->LOAD = reload - 1u;
    SysTick->VAL = 0u;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    return 0;
}

void core_arm_systick_stop(void)
{
    SysTick->CTRL = 0u;
}
