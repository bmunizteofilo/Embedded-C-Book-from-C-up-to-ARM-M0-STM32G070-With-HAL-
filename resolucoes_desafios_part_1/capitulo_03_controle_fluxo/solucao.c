#include <stdio.h>

#define ST_BOOT 0
#define ST_IDLE 1
#define ST_RUN 2
#define ST_ERROR 3

#define ERR_NONE 0
#define ERR_SENSOR 1
#define ERR_TIMEOUT 2
#define ERR_OVERFLOW 3

#define CMD_NONE 0
#define CMD_START 1
#define CMD_STOP 2
#define CMD_RESET 3

/**
 * @brief Simula a leitura de um comando externo em um passo especifico.
 *
 * @param step Iteracao atual da simulacao.
 *
 * @return Codigo do comando associado ao passo informado.
 */
static int read_command_sim(int step)
{
    switch (step) {
        case 2: return CMD_START;
        case 10: return CMD_STOP;
        case 15: return CMD_START;
        case 20: return CMD_RESET;
        default: return CMD_NONE;
    }
}

/**
 * @brief Converte o identificador numerico de estado em texto legivel.
 *
 * @param state Estado atual da maquina de estados.
 *
 * @return Ponteiro para uma string constante descrevendo o estado.
 */
static const char *state_to_str(int state)
{
    return (state == ST_ERROR) ? "ERROR" :
           (state == ST_BOOT)  ? "BOOT"  :
           (state == ST_IDLE)  ? "IDLE"  :
           (state == ST_RUN)   ? "RUN"   : "UNKNOWN";
}

/**
 * @brief Converte o codigo de erro em uma descricao textual.
 *
 * @param err Codigo de erro a ser traduzido.
 *
 * @return Ponteiro para uma string constante representando o erro.
 */
static const char *err_to_str(int err)
{
    switch (err) {
        case ERR_NONE: return "NONE";
        case ERR_SENSOR: return "SENSOR";
        case ERR_TIMEOUT: return "TIMEOUT";
        case ERR_OVERFLOW: return "OVERFLOW";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Simula tentativas de inicializacao do sistema.
 *
 * @param attempt Numero da tentativa de inicializacao.
 *
 * @return 1 quando a inicializacao e considerada bem-sucedida; senao, 0.
 */
static int init_sim_try(int attempt) { return (attempt >= 2) ? 1 : 0; }
/**
 * @brief Gera uma leitura de sensor sintetica para a simulacao.
 *
 * @param step Iteracao atual da simulacao.
 *
 * @return Valor inteiro representando a leitura do sensor.
 */
static int sensor_read_sim(int step) { return (step * 123) % 5000; }

/**
 * @brief Executa a simulacao da maquina de estados do sistema.
 *
 * @return 0 ao final da simulacao; retorna 1 apenas em estado invalido.
 */
int main(void)
{
    int state = ST_BOOT, error_code = ERR_NONE, sensor_value = 0;
    for (int step = 0; step <= 30; step++) {
        int cmd = read_command_sim(step);
        printf("[STEP %02d] state=%s err=%s cmd=%d\n", step, state_to_str(state), err_to_str(error_code), cmd);
        switch (state) {
            case ST_BOOT: {
                int attempt = 0, ok = 0;
                do {
                    attempt++;
                    ok = init_sim_try(attempt);
                    printf("  boot try=%d -> %s\n", attempt, ok ? "OK" : "FAIL");
                    if (ok) { break; }
                } while (attempt < 3);
                if (ok) { state = ST_IDLE; error_code = ERR_NONE; }
                else { state = ST_ERROR; error_code = ERR_TIMEOUT; }
                break;
            }
            case ST_IDLE:
                if (cmd == CMD_START) { state = ST_RUN; }
                else if (cmd == CMD_RESET) { state = ST_BOOT; }
                break;
            case ST_RUN:
                sensor_value = sensor_read_sim(step);
                printf("  sensor=%d\n", sensor_value);
                if (sensor_value < 100) { state = ST_ERROR; error_code = ERR_SENSOR; break; }
                else if (sensor_value > 4500) { state = ST_ERROR; error_code = ERR_OVERFLOW; break; }
                if (cmd == CMD_STOP) { state = ST_IDLE; break; }
                if (cmd == CMD_RESET) { state = ST_BOOT; break; }
                if (cmd == CMD_NONE) { continue; }
                break;
            case ST_ERROR:
                printf("  error=%s\n", err_to_str(error_code));
                if (cmd == CMD_RESET) { error_code = ERR_NONE; state = ST_BOOT; }
                break;
            default:
                return 1;
        }
    }
    return 0;
}
