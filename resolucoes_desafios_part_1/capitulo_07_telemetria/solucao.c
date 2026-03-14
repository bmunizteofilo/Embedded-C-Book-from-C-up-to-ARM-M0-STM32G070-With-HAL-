#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define TP_OK 0
#define TP_ERR_NULL -1
#define TP_ERR_LEN -2
#define TP_ERR_CRC -3
#define TP_ERR_FMT -4
#define TP_HEADER 0xA5u
#define TP_VERSION 1u
#define TP_FLAG_ALARM (1u << 0)
#define TP_FLAG_CHG (1u << 1)
#define TP_MODE_SHIFT 2u
#define TP_MODE_MASK (3u << TP_MODE_SHIFT)

typedef enum { MODE_IDLE = 0, MODE_RUN = 1, MODE_ERROR = 2 } TpMode;
typedef struct { uint8_t header; uint8_t version; uint16_t seq; int16_t temp_c10; uint16_t vbatt_mv; uint8_t flags; uint16_t crc16; } TelemetryPacket;

/**
 * @brief Calcula um CRC simplificado somando todos os bytes do payload.
 *
 * @param data Ponteiro para os bytes de entrada.
 * @param len Quantidade de bytes considerados no calculo.
 *
 * @return Soma truncada para 16 bits.
 */
static uint16_t tp_crc16_simple(const uint8_t *data, size_t len) { uint32_t sum = 0; for (size_t i = 0; i < len; i++) sum += data[i]; return (uint16_t)(sum & 0xFFFFu); }
/**
 * @brief Inicializa um pacote de telemetria com valores padrao.
 *
 * @param p Pacote a ser inicializado.
 *
 * @return `TP_OK` em sucesso ou `TP_ERR_NULL` se o ponteiro for invalido.
 */
static int tp_init(TelemetryPacket *p) { if (!p) return TP_ERR_NULL; p->header = TP_HEADER; p->version = TP_VERSION; p->seq = 0u; p->temp_c10 = 0; p->vbatt_mv = 0u; p->flags = 0u; p->crc16 = 0u; return TP_OK; }
/**
 * @brief Atualiza os campos variaveis principais do pacote.
 *
 * @param p Pacote a ser atualizado.
 * @param seq Numero de sequencia.
 * @param temp_c10 Temperatura em decimos de grau Celsius.
 * @param vbatt_mv Tensao da bateria em milivolts.
 *
 * @return `TP_OK` em sucesso ou `TP_ERR_NULL` se o ponteiro for invalido.
 */
static int tp_set_values(TelemetryPacket *p, uint16_t seq, int16_t temp_c10, uint16_t vbatt_mv) { if (!p) return TP_ERR_NULL; p->seq = seq; p->temp_c10 = temp_c10; p->vbatt_mv = vbatt_mv; return TP_OK; }
/**
 * @brief Ativa uma ou mais flags do pacote.
 *
 * @param p Pacote a ser modificado.
 * @param mask Mascara com os bits que devem ser setados.
 *
 * @return `TP_OK` em sucesso ou `TP_ERR_NULL` se o ponteiro for invalido.
 */
static int tp_set_flag(TelemetryPacket *p, uint8_t mask) { if (!p) return TP_ERR_NULL; p->flags |= mask; return TP_OK; }
/**
 * @brief Limpa uma ou mais flags do pacote.
 *
 * @param p Pacote a ser modificado.
 * @param mask Mascara com os bits que devem ser limpos.
 *
 * @return `TP_OK` em sucesso ou `TP_ERR_NULL` se o ponteiro for invalido.
 */
static int tp_clear_flag(TelemetryPacket *p, uint8_t mask) { if (!p) return TP_ERR_NULL; p->flags &= (uint8_t)(~mask); return TP_OK; }
/**
 * @brief Grava o modo operacional dentro do campo de flags.
 *
 * @param p Pacote a ser atualizado.
 * @param mode Novo modo operacional.
 *
 * @return `TP_OK` em sucesso ou `TP_ERR_NULL` se o ponteiro for invalido.
 */
static int tp_set_mode(TelemetryPacket *p, TpMode mode) { if (!p) return TP_ERR_NULL; p->flags = (uint8_t)((p->flags & ~TP_MODE_MASK) | ((((uint8_t)mode) << TP_MODE_SHIFT) & TP_MODE_MASK)); return TP_OK; }
/**
 * @brief Le o modo operacional armazenado no pacote.
 *
 * @param p Pacote de telemetria analisado.
 *
 * @return Modo operacional extraido do campo de flags.
 */
static TpMode tp_get_mode(const TelemetryPacket *p) { if (!p) return MODE_IDLE; return (TpMode)((p->flags & TP_MODE_MASK) >> TP_MODE_SHIFT); }

/**
 * @brief Serializa um pacote de telemetria em um buffer bruto.
 *
 * @param p Pacote de origem.
 * @param out Buffer de saida.
 * @param out_len Capacidade do buffer de saida.
 *
 * @return `TP_OK` em sucesso ou um codigo de erro apropriado.
 */
static int tp_pack(const TelemetryPacket *p, uint8_t *out, size_t out_len)
{
    uint16_t crc;
    if (!p || !out) return TP_ERR_NULL;
    if (out_len < 11u) return TP_ERR_LEN;
    out[0] = p->header; out[1] = p->version;
    out[2] = (uint8_t)(p->seq & 0xFFu); out[3] = (uint8_t)(p->seq >> 8);
    out[4] = (uint8_t)(p->temp_c10 & 0xFF); out[5] = (uint8_t)(((uint16_t)p->temp_c10) >> 8);
    out[6] = (uint8_t)(p->vbatt_mv & 0xFFu); out[7] = (uint8_t)(p->vbatt_mv >> 8);
    out[8] = p->flags;
    crc = tp_crc16_simple(out, 9u);
    out[9] = (uint8_t)(crc & 0xFFu);
    out[10] = (uint8_t)(crc >> 8);
    return TP_OK;
}

/**
 * @brief Desserializa um buffer bruto para uma estrutura de telemetria.
 *
 * @param p Estrutura de destino.
 * @param in Buffer contendo o pacote serializado.
 * @param in_len Quantidade de bytes disponiveis em `in`.
 *
 * @return `TP_OK` em sucesso ou um codigo de erro apropriado.
 */
static int tp_unpack(TelemetryPacket *p, const uint8_t *in, size_t in_len)
{
    uint16_t crc_rx, crc_calc;
    if (!p || !in) return TP_ERR_NULL;
    if (in_len < 11u) return TP_ERR_LEN;
    crc_rx = (uint16_t)in[9] | ((uint16_t)in[10] << 8);
    crc_calc = tp_crc16_simple(in, 9u);
    if (crc_rx != crc_calc) return TP_ERR_CRC;
    if (in[0] != TP_HEADER || in[1] != TP_VERSION) return TP_ERR_FMT;
    p->header = in[0]; p->version = in[1];
    p->seq = (uint16_t)in[2] | ((uint16_t)in[3] << 8);
    p->temp_c10 = (int16_t)((uint16_t)in[4] | ((uint16_t)in[5] << 8));
    p->vbatt_mv = (uint16_t)in[6] | ((uint16_t)in[7] << 8);
    p->flags = in[8]; p->crc16 = crc_rx;
    return TP_OK;
}

/**
 * @brief Demonstra a montagem e a validacao de um pacote de telemetria.
 *
 * @return Sempre retorna 0 ao final da demonstracao.
 */
int main(void)
{
    TelemetryPacket p, q;
    uint8_t raw[16] = {0};
    tp_init(&p);
    tp_set_values(&p, 10, 253, 12345);
    tp_set_flag(&p, TP_FLAG_ALARM);
    tp_clear_flag(&p, TP_FLAG_CHG);
    tp_set_mode(&p, MODE_RUN);
    tp_pack(&p, raw, sizeof(raw));
    printf("packed:");
    for (int i = 0; i < 11; i++) printf(" %02X", raw[i]);
    printf("\n");
    printf("unpack=%d\n", tp_unpack(&q, raw, 11));
    printf("seq=%u temp=%d vbatt=%u flags=0x%02X mode=%d\n", q.seq, q.temp_c10, q.vbatt_mv, q.flags, (int)tp_get_mode(&q));
    return 0;
}
