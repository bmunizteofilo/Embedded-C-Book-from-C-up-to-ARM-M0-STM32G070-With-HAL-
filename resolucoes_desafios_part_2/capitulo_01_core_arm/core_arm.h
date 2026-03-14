#ifndef CORE_ARM_H
#define CORE_ARM_H

#include <stdint.h>

/**
 * @brief Le o valor atual do registrador IPSR.
 *
 * @return Numero da excecao ativa; retorna 0 em thread mode.
 */
uint32_t core_arm_get_ipsr(void);

/**
 * @brief Le o valor atual do registrador xPSR.
 *
 * @return Valor bruto do xPSR.
 */
uint32_t core_arm_get_xpsr(void);

/**
 * @brief Le o valor atual do registrador PRIMASK.
 *
 * @return Valor bruto de PRIMASK.
 */
uint32_t core_arm_get_primask(void);

/**
 * @brief Entra em secao critica salvando o estado atual de PRIMASK.
 *
 * @return Valor anterior de PRIMASK, usado posteriormente no unlock.
 */
uint32_t core_arm_irq_lock(void);

/**
 * @brief Restaura o estado anterior de PRIMASK.
 *
 * @param key Valor retornado por core_arm_irq_lock().
 */
void core_arm_irq_unlock(uint32_t key);

/**
 * @brief Configura o SysTick para gerar interrupcoes periodicas.
 *
 * @param core_clk_hz Frequencia atual do clock do core.
 * @param tick_hz Frequencia desejada para o tick.
 *
 * @return 0 em sucesso ou -1 em caso de parametros invalidos.
 */
int core_arm_systick_init(uint32_t core_clk_hz, uint32_t tick_hz);

/**
 * @brief Desabilita o SysTick.
 */
void core_arm_systick_stop(void);

#endif
