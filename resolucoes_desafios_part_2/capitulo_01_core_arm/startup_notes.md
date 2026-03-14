# Startup Notes

## Vector table

- A vector table contem o MSP inicial no slot 0 e o `Reset_Handler` no slot 1.
- Os handlers de excecao e IRQ aparecem nos slots seguintes.
- No reset, o core le esses enderecos antes de qualquer codigo em `main()`.

## Stack frame automatico

Ao entrar em uma excecao, o Cortex-M0+ empilha automaticamente:

- `r0`
- `r1`
- `r2`
- `r3`
- `r12`
- `lr`
- `pc`
- `xpsr`

Esse frame permite retornar ao ponto interrompido e tambem serve como base para diagnostico em `HardFault`.

## EXC_RETURN

- Dentro de um handler, o registrador `LR` nao contem um retorno normal de funcao.
- Ele passa a carregar um valor especial chamado `EXC_RETURN`.
- Esse valor informa ao core como voltar da excecao e qual stack estava em uso.
