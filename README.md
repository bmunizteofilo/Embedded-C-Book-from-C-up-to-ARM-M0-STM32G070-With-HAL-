# Linguagem C para Embedded System + STM32 com HAL

Este repositório organiza o material-base do livro em três partes principais, além das resoluções separadas dos desafios. O objetivo é reunir texto, exemplos e exercícios em uma estrutura simples de navegar e fácil de evoluir ao longo da escrita.

## Estrutura do projeto

- `part_1.txt`
  Texto da Parte 1, focada nos fundamentos da linguagem C aplicados a sistemas embarcados.

- `part_2.txt`
  Texto da Parte 2, focada no núcleo ARM Cortex-M0+ e nos conceitos de baixo nível do STM32G070RB antes dos periféricos.

- `part_3.txt`
  Texto da Parte 3, focada nos periféricos do STM32G070RB com HAL, uso prático e padrões de projeto.

- `resolucoes_desafios_part_1/`
  Resoluções separadas dos desafios da Parte 1, organizadas por capítulo.

- `resolucoes_desafios_part_2/`
  Resoluções separadas dos desafios da Parte 2.

- `resolucoes_desafios_part_3/`
  Resoluções separadas dos desafios da Parte 3.

- `*.docx` e `*.pdf`
  Versões exportadas do conteúdo do livro.

## O que cada parte cobre

### Parte 1 - Linguagem C

A Parte 1 constrói a base de linguagem usada no restante do material. O foco não é apenas sintaxe, mas o comportamento real do C em firmware:

- tipos, variáveis, escopo, `static`, `const` e memória
- expressões, operadores, precedência e conversões
- controle de fluxo
- matrizes, strings e ponteiros
- funções, estruturas, unions, enumerações e pré-processador
- estruturas de dados básicas e introdução a C com assembly no Cortex-M0

Essa parte prepara o leitor para escrever código embarcado com mais clareza, previsibilidade e segurança.

### Parte 2 - Núcleo ARM Cortex-M0+

A Parte 2 trata do processador e do ambiente core-side do STM32G070RB, antes de entrar nos periféricos da ST:

- registradores do núcleo
- modos de execução, exceções e prioridades
- PRIMASK, SysTick, NVIC e SCB
- vector table, startup e reset
- HardFault, stack frame e `EXC_RETURN`
- linker script, layout de memória e otimização de compilação

Essa parte faz a ponte entre linguagem C e firmware bare-metal real.

### Parte 3 - Periféricos STM32G070RB com HAL

A Parte 3 cobre os periféricos mais importantes do STM32G070RB com abordagem prática e foco em uso profissional:

- GPIO, EXTI e debounce
- DMA e buffers
- ADC
- USART/UART
- SPI
- I2C
- timers, PWM, input capture e encoder
- RTC, backup registers e tamper
- watchdog
- Flash interna
- clocks
- interrupções e eventos

O foco desta parte é entender o hardware, configurar corretamente no CubeMX/CubeIDE e aplicar padrões de firmware reutilizáveis.

## Sobre as pastas de resolucao

As resoluções foram separadas do texto principal para manter o livro limpo e, ao mesmo tempo, preservar material prático para estudo e validação.

Em geral, cada pasta de resolução contém:

- um `README.md` local com o contexto do desafio
- um ou mais arquivos `.c` com a solução
- organização por capítulo ou por desafio, dependendo da parte

## Fluxo de leitura sugerido

1. Ler a Parte 1 para consolidar a base de C.
2. Avançar para a Parte 2 para entender o núcleo ARM e o ambiente de execução.
3. Entrar na Parte 3 para trabalhar os periféricos e o uso prático com HAL.
4. Consultar as pastas `resolucoes_desafios_part_*` ao final de cada bloco de estudo.

## Observacao

Os arquivos `.txt` são a base editorial em evolução. As versões `.docx` e `.pdf` servem como exportações do material em formatos de distribuição e revisão.
