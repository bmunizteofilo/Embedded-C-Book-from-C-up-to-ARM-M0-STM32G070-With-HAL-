#include <stdio.h>
#include <stddef.h>

#define CMD_ID_START 0
#define CMD_ID_STOP 1
#define CMD_ID_RESET 2
#define CMD_ID_COUNT 3
#define CMD_ID_UNKNOWN (-1)

typedef int (*cmd_fn_t)(void);
typedef void (*unknown_cb_t)(const char *rx);

static unknown_cb_t g_unknown_cb = NULL;
static int g_start_cnt, g_stop_cnt, g_reset_cnt, g_unk_cnt;

/**
 * @brief Mede o comprimento util de uma string recebida da UART.
 *
 * @param s Ponteiro para a string de entrada.
 *
 * @return Quantidade de caracteres antes de `\0`, `\r` ou `\n`.
 */
static size_t my_strlen(const char *s) { size_t n = 0; if (!s) return 0; while (s[n] != '\0' && s[n] != '\r' && s[n] != '\n') n++; return n; }
/**
 * @brief Compara duas strings caractere a caractere.
 *
 * @param a Primeira string.
 * @param b Segunda string.
 *
 * @return Diferenca entre os primeiros caracteres distintos.
 */
static int my_strcmp(const char *a, const char *b) { size_t i = 0; while (a[i] && b[i] && a[i] == b[i]) i++; return ((unsigned char)a[i] - (unsigned char)b[i]); }
/**
 * @brief Trata o comando START e atualiza o contador correspondente.
 *
 * @return 0 para indicar execucao bem-sucedida.
 */
static int cmd_start(void) { g_start_cnt++; printf("START\n"); return 0; }
/**
 * @brief Trata o comando STOP e atualiza o contador correspondente.
 *
 * @return 0 para indicar execucao bem-sucedida.
 */
static int cmd_stop(void) { g_stop_cnt++; printf("STOP\n"); return 0; }
/**
 * @brief Trata o comando RESET e atualiza o contador correspondente.
 *
 * @return 0 para indicar execucao bem-sucedida.
 */
static int cmd_reset(void) { g_reset_cnt++; printf("RESET\n"); return 0; }
/**
 * @brief Callback padrao para comandos nao reconhecidos.
 *
 * @param rx String recebida que nao correspondeu a nenhum comando valido.
 */
static void on_unknown(const char *rx) { g_unk_cnt++; printf("UNKNOWN: %s\n", rx ? rx : "(null)"); }
/**
 * @brief Registra o callback usado para tratar comandos desconhecidos.
 *
 * @param cb Funcao chamada quando um comando invalido for detectado.
 */
static void register_unknown_cb(unknown_cb_t cb) { g_unknown_cb = cb; }
/**
 * @brief Seleciona qual buffer de recepcao deve ficar ativo.
 *
 * @param active Endereco do ponteiro que recebera o buffer selecionado.
 * @param use_a Quando diferente de zero, seleciona `rx_a`; caso contrario, `rx_b`.
 * @param rx_a Primeiro buffer candidato.
 * @param rx_b Segundo buffer candidato.
 */
static void select_active_rx(char **active, int use_a, char *rx_a, char *rx_b) { if (!active) return; *active = use_a ? rx_a : rx_b; }
/**
 * @brief Converte uma string recebida em um identificador de comando.
 *
 * @param rx Buffer contendo o texto recebido.
 *
 * @return Identificador do comando ou `CMD_ID_UNKNOWN` quando invalido.
 */
static int parse_cmd(const char *rx)
{
    char local[16];
    size_t n, i;
    const char *p = rx;
    if (!p) return CMD_ID_UNKNOWN;
    n = my_strlen(p);
    if (n >= sizeof(local)) return CMD_ID_UNKNOWN;
    for (i = 0; i < n; i++) local[i] = p[i];
    local[n] = '\0';
    if (my_strcmp(local, "START") == 0) return CMD_ID_START;
    if (my_strcmp(local, "STOP") == 0) return CMD_ID_STOP;
    if (my_strcmp(local, "RESET") == 0) return CMD_ID_RESET;
    return CMD_ID_UNKNOWN;
}
/**
 * @brief Despacha um comando textual para a funcao tratadora apropriada.
 *
 * @param rx String contendo o comando recebido.
 *
 * @return Valor retornado pelo handler ou -1 em caso de comando desconhecido.
 */
static int dispatch_cmd(const char *rx) { static cmd_fn_t table[CMD_ID_COUNT] = { cmd_start, cmd_stop, cmd_reset }; int id = parse_cmd(rx); if (id >= 0 && id < CMD_ID_COUNT) return table[id](); if (g_unknown_cb) g_unknown_cb(rx); return -1; }

/**
 * @brief Exercita o parser e o despachante de comandos simulados.
 *
 * @return Sempre retorna 0 ao final da demonstracao.
 */
int main(void)
{
    char rx_a[64] = "START";
    char rx_b[64] = "STOP";
    char *active = NULL;
    register_unknown_cb(on_unknown);
    printf("parse START=%d STOP=%d RESET=%d ABC=%d\n", parse_cmd("START"), parse_cmd("STOP"), parse_cmd("RESET"), parse_cmd("ABC"));
    select_active_rx(&active, 1, rx_a, rx_b); printf("active=%s\n", active);
    select_active_rx(&active, 0, rx_a, rx_b); printf("active=%s\n", active);
    (void)dispatch_cmd("START");
    (void)dispatch_cmd("STOP");
    (void)dispatch_cmd("RESET");
    (void)dispatch_cmd("UNKNOWN_CMD");
    printf("cnt start=%d stop=%d reset=%d unknown=%d\n", g_start_cnt, g_stop_cnt, g_reset_cnt, g_unk_cnt);
    return 0;
}
