#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Preenche um buffer com zeros.
 *
 * @param buf Ponteiro para o buffer de destino.
 * @param len Quantidade de bytes a serem limpos.
 */
static void buf_clear_u8(uint8_t *buf, size_t len) { if (!buf) return; for (size_t i = 0; i < len; i++) buf[i] = 0u; }
/**
 * @brief Encontra o maior valor presente em um buffer.
 *
 * @param buf Ponteiro para o vetor de entrada.
 * @param len Quantidade de elementos validos no vetor.
 *
 * @return Maior elemento encontrado ou 0 em caso de entrada invalida.
 */
static uint8_t buf_max_u8(const uint8_t *buf, size_t len) { if (!buf || len == 0u) return 0u; uint8_t m = buf[0]; for (size_t i = 1; i < len; i++) if (buf[i] > m) m = buf[i]; return m; }
/**
 * @brief Encontra o menor valor presente em um buffer.
 *
 * @param buf Ponteiro para o vetor de entrada.
 * @param len Quantidade de elementos validos no vetor.
 *
 * @return Menor elemento encontrado ou 0 em caso de entrada invalida.
 */
static uint8_t buf_min_u8(const uint8_t *buf, size_t len) { if (!buf || len == 0u) return 0u; uint8_t m = buf[0]; for (size_t i = 1; i < len; i++) if (buf[i] < m) m = buf[i]; return m; }
/**
 * @brief Conta quantas vezes um valor aparece em um buffer.
 *
 * @param buf Ponteiro para o vetor de entrada.
 * @param len Quantidade de elementos validos no vetor.
 * @param value Valor procurado.
 *
 * @return Numero de ocorrencias do valor informado.
 */
static size_t buf_count_u8(const uint8_t *buf, size_t len, uint8_t value) { size_t c = 0; if (!buf) return 0; for (size_t i = 0; i < len; i++) if (buf[i] == value) c++; return c; }
/**
 * @brief Calcula o comprimento de uma string terminada em nulo.
 *
 * @param s String de entrada.
 *
 * @return Quantidade de caracteres antes do terminador `\0`.
 */
static size_t my_strlen(const char *s) { size_t n = 0; if (!s) return 0; while (s[n] != '\0') n++; return n; }
/**
 * @brief Copia uma string para um buffer com limite de tamanho.
 *
 * @param dst Buffer de destino.
 * @param src String de origem.
 * @param dst_size Capacidade total do buffer de destino.
 *
 * @return Comprimento total da string de origem.
 */
static size_t my_strlcpy(char *dst, const char *src, size_t dst_size) { size_t n = my_strlen(src), i = 0; if (dst && dst_size > 0u) { while (src && src[i] != '\0' && i + 1u < dst_size) { dst[i] = src[i]; i++; } dst[i] = '\0'; } return n; }
/**
 * @brief Compara duas strings lexicograficamente.
 *
 * @param a Primeira string.
 * @param b Segunda string.
 *
 * @return Valor negativo, zero ou positivo conforme a ordenacao entre as strings.
 */
static int my_strcmp(const char *a, const char *b) { size_t i = 0; if (!a || !b) return (a == b) ? 0 : (a ? 1 : -1); while (a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) i++; return ((unsigned char)a[i] - (unsigned char)b[i]); }
/**
 * @brief Preenche todas as posicoes de um tabuleiro 3x3 com um simbolo.
 *
 * @param board Matriz que representa o tabuleiro.
 * @param fill Caractere usado para preencher cada celula.
 */
static void board_clear(char board[3][3], char fill) { for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) board[r][c] = fill; }
/**
 * @brief Conta quantas celulas do tabuleiro possuem um simbolo especifico.
 *
 * @param board Matriz que representa o tabuleiro.
 * @param sym Simbolo procurado.
 *
 * @return Quantidade de ocorrencias encontradas.
 */
static int board_count(const char board[3][3], char sym) { int n = 0; for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) if (board[r][c] == sym) n++; return n; }
/**
 * @brief Verifica se existe uma linha completa com o simbolo informado.
 *
 * @param board Matriz que representa o tabuleiro.
 * @param sym Simbolo que deve preencher uma linha inteira.
 *
 * @return 1 se alguma linha estiver completa; caso contrario, 0.
 */
static int board_has_row(const char board[3][3], char sym) { for (int r = 0; r < 3; r++) if (board[r][0] == sym && board[r][1] == sym && board[r][2] == sym) return 1; return 0; }
/**
 * @brief Imprime um menu de opcoes em formato de lista indexada.
 *
 * @param menu Matriz contendo os textos de cada item.
 * @param items Quantidade de itens validos a serem exibidos.
 */
static void menu_print(char menu[][16], size_t items) { for (size_t i = 0; i < items; i++) printf("[%u] %s\n", (unsigned)i, menu[i]); }

/**
 * @brief Demonstra operacoes com buffers, strings e matrizes.
 *
 * @return Sempre retorna 0 ao final da demonstracao.
 */
int main(void)
{
    uint8_t buf[8] = {5, 2, 9, 2, 7, 2, 1, 9};
    char dst[5];
    char board[3][3];
    char menu[4][16] = {"Start", "Stop", "Settings", "Exit"};
    printf("max=%u min=%u count2=%u\n", buf_max_u8(buf, 8), buf_min_u8(buf, 8), (unsigned)buf_count_u8(buf, 8, 2));
    buf_clear_u8(buf, 8);
    printf("zeros=%u\n", (unsigned)buf_count_u8(buf, 8, 0));
    printf("strlen(ABC)=%u\n", (unsigned)my_strlen("ABC"));
    my_strlcpy(dst, "HELLO", sizeof(dst));
    printf("dst=%s cmp=%d\n", dst, my_strcmp("ABC", "ABD"));
    board_clear(board, '.');
    board[1][0] = 'X'; board[1][1] = 'X'; board[1][2] = 'X';
    printf("board_count(X)=%d row(X)=%d\n", board_count(board, 'X'), board_has_row(board, 'X'));
    menu_print(menu, 4);
    return 0;
}
