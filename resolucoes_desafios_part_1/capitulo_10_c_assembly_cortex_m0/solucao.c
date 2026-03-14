#include <stdint.h>
#include <stdio.h>
#include "core_cm0.h"

static volatile uint32_t reg_v = 0;
static uint32_t reg_nv = 0;
static volatile uint32_t g_counter = 0;

/**
 * @brief Executa uma instrucao NOP no processador Cortex-M0.
 */
static void cpu_nop(void) { __NOP(); }
/**
 * @brief Executa a instrucao WFI para aguardar interrupcao.
 */
static void cpu_wfi(void) { __WFI(); }
/**
 * @brief Desabilita globalmente as interrupcoes mascaraveis.
 */
static void cpu_disable_irq(void) { __disable_irq(); }
/**
 * @brief Reabilita globalmente as interrupcoes mascaraveis.
 */
static void cpu_enable_irq(void) { __enable_irq(); }
/**
 * @brief Le o valor atual do registrador PRIMASK.
 *
 * @return Conteudo atual de PRIMASK.
 */
static uint32_t cpu_get_primask(void) { return __get_PRIMASK(); }
/**
 * @brief Escreve um novo valor no registrador PRIMASK.
 *
 * @param v Valor a ser gravado em PRIMASK.
 */
static void cpu_set_primask(uint32_t v) { __set_PRIMASK(v); }
/**
 * @brief Executa uma barreira de memoria do tipo Data Memory Barrier.
 */
static void cpu_dmb(void) { __DMB(); }
/**
 * @brief Executa uma barreira de memoria do tipo Data Synchronization Barrier.
 */
static void cpu_dsb(void) { __DSB(); }
/**
 * @brief Executa uma barreira de pipeline do tipo Instruction Synchronization Barrier.
 */
static void cpu_isb(void) { __ISB(); }

/**
 * @brief Entra em secao critica salvando o estado atual de interrupcoes.
 *
 * @return Chave contendo o valor anterior de PRIMASK.
 */
static uint32_t cpu_irq_lock(void)
{
    uint32_t key = cpu_get_primask();
    cpu_disable_irq();
    cpu_dmb();
    return key;
}

/**
 * @brief Restaura o estado de interrupcoes salvo anteriormente.
 *
 * @param key Valor de PRIMASK retornado por `cpu_irq_lock`.
 */
static void cpu_irq_unlock(uint32_t key)
{
    cpu_dmb();
    cpu_set_primask(key);
}

/**
 * @brief Simula uma rotina de interrupcao incrementando um contador global.
 */
static void isr_sim(void) { g_counter++; }

/**
 * @brief Demonstra o uso de intrinsics CMSIS e secao critica em Cortex-M0.
 *
 * @return Sempre retorna 0 ao final da demonstracao.
 */
int main(void)
{
    cpu_nop();
    g_counter = 0;
    for (int i = 0; i < 10; i++) {
        uint32_t key = cpu_irq_lock();
        g_counter++;
        isr_sim();
        cpu_irq_unlock(key);
    }
    for (int i = 0; i < 100; i++) {
        reg_v++;
        reg_nv++;
    }
    cpu_dsb();
    cpu_isb();
    printf("g_counter=%lu reg_v=%lu reg_nv=%lu primask=%lu\n",
           (unsigned long)g_counter,
           (unsigned long)reg_v,
           (unsigned long)reg_nv,
           (unsigned long)cpu_get_primask());
    /* cpu_wfi(); */
    cpu_enable_irq();
    return 0;
}
