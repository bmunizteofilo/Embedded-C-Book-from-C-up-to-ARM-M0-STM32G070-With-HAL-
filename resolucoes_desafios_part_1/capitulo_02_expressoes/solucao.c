#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define BIT(n) (1u << (n))
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#define MODE_MASK (0x70u)
#define MODE_SHIFT (4u)

/**
 * @brief Verifica se um bit especifico esta em nivel logico alto.
 *
 * @param reg Valor do registrador analisado.
 * @param bit Indice do bit a ser testado.
 *
 * @return 1 quando o bit esta setado; caso contrario, retorna 0.
 */
static int bit_is_set_u32(uint32_t reg, uint8_t bit) { return (bit < 32u) ? (((reg & BIT(bit)) != 0u) ? 1 : 0) : 0; }
/**
 * @brief Seta um bit de um registrador de 32 bits.
 *
 * @param reg Valor original do registrador.
 * @param bit Indice do bit que deve ser ativado.
 *
 * @return Novo valor do registrador com o bit solicitado em 1.
 */
static uint32_t set_bit_u32(uint32_t reg, uint8_t bit) { return (bit < 32u) ? (reg | BIT(bit)) : reg; }
/**
 * @brief Limpa um bit de um registrador de 32 bits.
 *
 * @param reg Valor original do registrador.
 * @param bit Indice do bit que deve ser zerado.
 *
 * @return Novo valor do registrador com o bit solicitado em 0.
 */
static uint32_t clear_bit_u32(uint32_t reg, uint8_t bit) { return (bit < 32u) ? (reg & ~BIT(bit)) : reg; }
/**
 * @brief Inverte o estado de um bit de um registrador.
 *
 * @param reg Valor original do registrador.
 * @param bit Indice do bit que deve ser alternado.
 *
 * @return Novo valor do registrador apos a operacao de toggle.
 */
static uint32_t toggle_bit_u32(uint32_t reg, uint8_t bit) { return (bit < 32u) ? (reg ^ BIT(bit)) : reg; }
/**
 * @brief Extrai o campo de modo armazenado no registrador.
 *
 * @param reg Valor do registrador contendo o campo de modo.
 *
 * @return Valor de 3 bits correspondente ao modo configurado.
 */
static uint8_t get_mode(uint32_t reg) { return (uint8_t)((reg & MODE_MASK) >> MODE_SHIFT); }
/**
 * @brief Atualiza o campo de modo de um registrador.
 *
 * @param reg Valor original do registrador.
 * @param mode Novo modo desejado; apenas os 3 bits menos significativos sao usados.
 *
 * @return Novo valor do registrador com o campo de modo atualizado.
 */
static uint32_t set_mode(uint32_t reg, uint8_t mode) { mode &= 0x07u; return (reg & ~MODE_MASK) | (((uint32_t)mode << MODE_SHIFT) & MODE_MASK); }
/**
 * @brief Verifica se um valor inteiro esta dentro de um intervalo inclusivo.
 *
 * @param v Valor a ser testado.
 * @param min Limite inferior do intervalo.
 * @param max Limite superior do intervalo.
 *
 * @return 1 se o valor estiver no intervalo; caso contrario, 0.
 */
static int in_range_i32(int32_t v, int32_t min, int32_t max) { return (min <= v && v <= max) ? 1 : 0; }
/**
 * @brief Converte uma leitura ADC de 12 bits para milivolts.
 *
 * @param adc Codigo convertido pelo ADC, no intervalo de 0 a 4095.
 * @param vref_mv Tensao de referencia do ADC em milivolts.
 *
 * @return Valor aproximado da tensao correspondente em milivolts.
 */
static uint32_t adc12_to_mv(uint16_t adc, uint32_t vref_mv) { return ((uint32_t)adc * vref_mv) / 4095u; }
/**
 * @brief Limita um valor inteiro de 32 bits ao intervalo de um int16_t.
 *
 * @param v Valor a ser saturado.
 *
 * @return Valor saturado entre -32768 e 32767.
 */
static int16_t clamp_i16(int32_t v) { return (v > 32767) ? 32767 : ((v < -32768) ? -32768 : (int16_t)v); }

static int g_fail = 0;
#define CHECK_U32(a, e) do { uint32_t A=(uint32_t)(a), E=(uint32_t)(e); if (A!=E) { g_fail++; printf("FAIL %u != %u\n", (unsigned)A, (unsigned)E); } } while (0)
#define CHECK_I32(a, e) do { int32_t A=(int32_t)(a), E=(int32_t)(e); if (A!=E) { g_fail++; printf("FAIL %ld != %ld\n", (long)A, (long)E); } } while (0)

/**
 * @brief Executa uma bateria simples de testes das expressoes implementadas.
 *
 * @return 0 quando todos os testes passam; caso contrario, retorna 1.
 */
int main(void)
{
    int a[10];
    CHECK_I32(bit_is_set_u32(0x10u, 4), 1);
    CHECK_I32(bit_is_set_u32(0x10u, 3), 0);
    CHECK_U32(set_bit_u32(0x00u, 1), 0x02u);
    CHECK_U32(clear_bit_u32(0xFFu, 7), 0x7Fu);
    CHECK_U32(toggle_bit_u32(0x00u, 0), 0x01u);
    CHECK_I32(get_mode(0x30u), 3);
    CHECK_U32(set_mode(0x00u, 7), 0x70u);
    CHECK_U32(set_mode(0xFFu, 0), 0x8Fu);
    CHECK_I32(in_range_i32(10, 0, 20), 1);
    CHECK_I32(in_range_i32(-5, -10, -1), 1);
    CHECK_I32(in_range_i32(21, 0, 20), 0);
    CHECK_U32(adc12_to_mv(0, 3300u), 0u);
    CHECK_U32(adc12_to_mv(4095, 3300u), 3300u);
    CHECK_I32(clamp_i16(40000), 32767);
    CHECK_I32(clamp_i16(-40000), -32768);
    CHECK_I32(clamp_i16(1234), 1234);
    CHECK_U32(ARRAY_LEN(a), 10u);
    printf("Cap.2: %s\n", (g_fail == 0) ? "PASS" : "FAIL");
    return (g_fail == 0) ? 0 : 1;
}
