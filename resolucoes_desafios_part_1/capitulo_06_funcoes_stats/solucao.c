#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define ST_OK 0
#define ST_ERR_NULL -1
#define ST_ERR_LEN -2

/**
 * @brief Calcula o menor valor de um vetor de `uint16_t`.
 *
 * @param buf Vetor de entrada.
 * @param len Quantidade de elementos validos.
 * @param out_min Ponteiro para armazenar o menor valor encontrado.
 *
 * @return `ST_OK` em sucesso ou um codigo de erro em caso de falha.
 */
static int stats_min_u16(const uint16_t *buf, size_t len, uint16_t *out_min) { if (!buf || !out_min) return ST_ERR_NULL; if (len == 0u) return ST_ERR_LEN; uint16_t v = buf[0]; for (size_t i = 1; i < len; i++) if (buf[i] < v) v = buf[i]; *out_min = v; return ST_OK; }
/**
 * @brief Calcula o maior valor de um vetor de `uint16_t`.
 *
 * @param buf Vetor de entrada.
 * @param len Quantidade de elementos validos.
 * @param out_max Ponteiro para armazenar o maior valor encontrado.
 *
 * @return `ST_OK` em sucesso ou um codigo de erro em caso de falha.
 */
static int stats_max_u16(const uint16_t *buf, size_t len, uint16_t *out_max) { if (!buf || !out_max) return ST_ERR_NULL; if (len == 0u) return ST_ERR_LEN; uint16_t v = buf[0]; for (size_t i = 1; i < len; i++) if (buf[i] > v) v = buf[i]; *out_max = v; return ST_OK; }
/**
 * @brief Calcula a media aritmetica inteira de um vetor.
 *
 * @param buf Vetor de entrada.
 * @param len Quantidade de elementos validos.
 * @param out_mean Ponteiro para armazenar a media calculada.
 *
 * @return `ST_OK` em sucesso ou um codigo de erro em caso de falha.
 */
static int stats_mean_u16(const uint16_t *buf, size_t len, uint32_t *out_mean) { uint64_t sum = 0; if (!buf || !out_mean) return ST_ERR_NULL; if (len == 0u) return ST_ERR_LEN; for (size_t i = 0; i < len; i++) sum += buf[i]; *out_mean = (uint32_t)(sum / len); return ST_OK; }
/**
 * @brief Calcula simultaneamente o menor e o maior valor do vetor.
 *
 * @param buf Vetor de entrada.
 * @param len Quantidade de elementos validos.
 * @param out_min Ponteiro para armazenar o menor valor.
 * @param out_max Ponteiro para armazenar o maior valor.
 *
 * @return `ST_OK` em sucesso ou um codigo de erro em caso de falha.
 */
static int stats_minmax_u16(const uint16_t *buf, size_t len, uint16_t *out_min, uint16_t *out_max) { if (!buf || !out_min || !out_max) return ST_ERR_NULL; if (len == 0u) return ST_ERR_LEN; uint16_t mn = buf[0], mx = buf[0]; for (size_t i = 1; i < len; i++) { if (buf[i] < mn) mn = buf[i]; if (buf[i] > mx) mx = buf[i]; } *out_min = mn; *out_max = mx; return ST_OK; }
/**
 * @brief Calcula a raiz quadrada inteira de um valor de 32 bits.
 *
 * @param x Valor de entrada.
 *
 * @return Maior inteiro cujo quadrado nao excede `x`.
 */
static uint32_t isqrt_u32(uint32_t x) { uint32_t lo = 0, hi = 65535, ans = 0; while (lo <= hi) { uint32_t mid = lo + ((hi - lo) / 2u); uint64_t sq = (uint64_t)mid * (uint64_t)mid; if (sq <= x) { ans = mid; lo = mid + 1u; } else { hi = mid - 1u; } } return ans; }
/**
 * @brief Calcula o valor RMS inteiro de um vetor de amostras.
 *
 * @param buf Vetor de entrada.
 * @param len Quantidade de elementos validos.
 * @param out_rms Ponteiro para armazenar o resultado RMS.
 *
 * @return `ST_OK` em sucesso ou um codigo de erro em caso de falha.
 */
static int stats_rms_u16(const uint16_t *buf, size_t len, uint32_t *out_rms) { uint64_t sum_sq = 0; if (!buf || !out_rms) return ST_ERR_NULL; if (len == 0u) return ST_ERR_LEN; for (size_t i = 0; i < len; i++) sum_sq += (uint64_t)buf[i] * (uint64_t)buf[i]; *out_rms = isqrt_u32((uint32_t)(sum_sq / len)); return ST_OK; }

/**
 * @brief Demonstra o uso das funcoes estatisticas sobre um vetor fixo.
 *
 * @return Sempre retorna 0 ao final da execucao.
 */
int main(void)
{
    uint16_t buf[] = {100, 200, 300, 400};
    uint16_t mn = 0, mx = 0;
    uint32_t mean = 0, rms = 0;
    printf("min=%d status=%d\n", mn, stats_min_u16(buf, 4, &mn));
    printf("max=%d status=%d\n", mx, stats_max_u16(buf, 4, &mx));
    printf("mean=%lu status=%d\n", (unsigned long)mean, stats_mean_u16(buf, 4, &mean));
    printf("minmax status=%d\n", stats_minmax_u16(buf, 4, &mn, &mx));
    printf("mn=%u mx=%u\n", mn, mx);
    printf("rms status=%d\n", stats_rms_u16(buf, 4, &rms));
    printf("rms=%lu\n", (unsigned long)rms);
    return 0;
}
