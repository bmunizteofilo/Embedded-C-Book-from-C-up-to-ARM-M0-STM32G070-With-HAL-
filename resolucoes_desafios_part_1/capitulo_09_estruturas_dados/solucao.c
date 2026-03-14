#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define Q_CAP 8
#define H_CAP 8
#define NODE_CAP 8
#define EVT_ADD_ID 1
#define EVT_REMOVE_ID 2
#define EVT_LOG_ONLY 3

typedef struct { uint8_t type; uint16_t data; } event_t;
typedef struct { event_t buf[Q_CAP]; size_t head, tail, count; } queue_t;
typedef struct { event_t buf[H_CAP]; size_t count; } stack_t;
typedef struct node_s { uint16_t id; struct node_s *next; uint8_t used; } node_t;

static queue_t g_q;
static stack_t g_hist;
static node_t g_nodes[NODE_CAP];
static node_t *g_list = NULL;

/**
 * @brief Insere um evento na fila circular global.
 *
 * @param e Evento a ser enfileirado.
 *
 * @return 0 em sucesso ou -1 quando a fila estiver cheia.
 */
static int q_push(event_t e) { if (g_q.count >= Q_CAP) return -1; g_q.buf[g_q.tail] = e; g_q.tail = (g_q.tail + 1u) % Q_CAP; g_q.count++; return 0; }
/**
 * @brief Remove o evento mais antigo da fila circular global.
 *
 * @param e Ponteiro para receber o evento removido.
 *
 * @return 0 em sucesso ou -1 quando a fila estiver vazia ou o ponteiro for invalido.
 */
static int q_pop(event_t *e) { if (!e || g_q.count == 0u) return -1; *e = g_q.buf[g_q.head]; g_q.head = (g_q.head + 1u) % Q_CAP; g_q.count--; return 0; }
/**
 * @brief Registra um evento no historico em formato de pilha simples.
 *
 * @param e Evento a ser armazenado.
 */
static void hist_push(event_t e) { if (g_hist.count < H_CAP) g_hist.buf[g_hist.count++] = e; }
/**
 * @brief Reserva um no livre do pool estatico.
 *
 * @return Ponteiro para o no reservado ou `NULL` se nao houver espaco.
 */
static node_t *alloc_node(void) { for (size_t i = 0; i < NODE_CAP; i++) if (!g_nodes[i].used) { g_nodes[i].used = 1u; g_nodes[i].next = NULL; return &g_nodes[i]; } return NULL; }
/**
 * @brief Libera um no previamente reservado do pool estatico.
 *
 * @param n Ponteiro para o no a ser liberado.
 */
static void free_node(node_t *n) { if (n) { n->used = 0u; n->next = NULL; } }
/**
 * @brief Adiciona um identificador na lista ligada de ativos.
 *
 * @param id Identificador a ser inserido.
 *
 * @return 0 em sucesso ou -1 se nao houver nos disponiveis.
 */
static int list_add(uint16_t id) { node_t *n, *cur = g_list; while (cur) { if (cur->id == id) return 0; cur = cur->next; } n = alloc_node(); if (!n) return -1; n->id = id; n->next = g_list; g_list = n; return 0; }
/**
 * @brief Remove um identificador da lista ligada de ativos.
 *
 * @param id Identificador a ser removido.
 *
 * @return 0 em sucesso ou -1 quando o identificador nao existir.
 */
static int list_remove(uint16_t id) { node_t *cur = g_list, *prev = NULL; while (cur) { if (cur->id == id) { if (prev) prev->next = cur->next; else g_list = cur->next; free_node(cur); return 0; } prev = cur; cur = cur->next; } return -1; }
/**
 * @brief Conta quantos elementos existem atualmente na lista ligada.
 *
 * @return Quantidade de nos ativos na lista.
 */
static size_t list_count(void) { size_t n = 0; node_t *cur = g_list; while (cur) { n++; cur = cur->next; } return n; }

/**
 * @brief Processa eventos de uma fila e atualiza estruturas de dados auxiliares.
 *
 * @return Sempre retorna 0 ao final da demonstracao.
 */
int main(void)
{
    event_t e;
    q_push((event_t){EVT_ADD_ID, 10});
    q_push((event_t){EVT_ADD_ID, 20});
    q_push((event_t){EVT_LOG_ONLY, 123});
    q_push((event_t){EVT_REMOVE_ID, 10});
    q_push((event_t){EVT_ADD_ID, 30});

    while (q_pop(&e) == 0) {
        switch (e.type) {
            case EVT_ADD_ID: (void)list_add(e.data); break;
            case EVT_REMOVE_ID: (void)list_remove(e.data); break;
            case EVT_LOG_ONLY: printf("log_only=%u\n", e.data); break;
            default: break;
        }
        hist_push(e);
    }

    printf("active_count=%u\n", (unsigned)list_count());
    printf("history_count=%u\n", (unsigned)g_hist.count);
    return 0;
}
