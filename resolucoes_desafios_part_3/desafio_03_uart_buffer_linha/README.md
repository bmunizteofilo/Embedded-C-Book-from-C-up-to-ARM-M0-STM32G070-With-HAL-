# Desafio 3 - UART com recepcao de linha

Resolucao objetiva para recepcao UART byte a byte por interrupcao, acumulando dados ate `\\n`.

Arquivos:

- `solucao.c`: callback RX, rearmamento da recepcao e processamento fora da interrupcao.
