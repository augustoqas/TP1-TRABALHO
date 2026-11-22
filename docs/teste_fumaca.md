# Teste fumaça (smoke test)

Um caso de teste de **sucesso** para cada funcionalidade do sistema: criar conta, autenticar
e os serviços 1 a 26. A entrada completa está em
[`testes/roteiros/91_fumaca.txt`](../testes/roteiros/91_fumaca.txt) e o resultado esperado em
[`91_fumaca.esperado`](../testes/roteiros/91_fumaca.esperado); o `testes\rodar.bat` confere o
roteiro automaticamente.

## Como executar

Compile o código integrado (`main.cpp` + `sources/`) e rode o programa:

```
g++ -std=c++11 -Wall -Wextra -Iheaders main.cpp sources/*.cpp -o kanban
kanban
```

Para o vídeo, digite as entradas da tabela abaixo, na ordem. Para rodar tudo de uma vez:
`kanban < testes\roteiros\91_fumaca.txt`.

> **Métricas:** tempo de ciclo e lead time são medidos em minutos reais. Se tudo for feito no
> mesmo minuto, o resultado é `0.0 minutos` (ainda é sucesso). No vídeo, espere 1 ou 2 minutos antes do
> passo 23 (mover para FAZENDO) e de novo antes do passo 24 (mover para FEITO): assim o tempo de
> ciclo (23 → 24) e o lead time (18 → 24) aparecem diferentes de zero.

As exclusões ficam no fim e seguem a ordem cartão → quadro → projeto → pessoa, porque o sistema
recusa excluir o que ainda tem vínculo. Por isso cada uma usa um item extra (CRT200, QDR200,
PRJ200 e carla), e os itens principais continuam existindo para as métricas.

## Passo a passo

| # | Funcionalidade | Onde | O que digitar | Resultado esperado |
|---|---|---|---|---|
| 1 | Criar conta (serviço 1) | Menu inicial → 1 | `ana@unb.br`, `Ana Maria`, `Pw123`, `GESTOR` | `[OK] Conta criada com sucesso` |
| 2 | Criar conta (desenvolvedor) | Menu inicial → 1 | `bruno@unb.br`, `Bruno Lima`, `Br456`, `DESENVOLVEDOR` | `[OK] Conta criada com sucesso` |
| 3 | Autenticar | Menu inicial → 2 | `ana@unb.br`, `Pw123` | `[OK] Autenticado com sucesso` |
| 4 | Criar pessoa (serviço 1) | Pessoas (1) → 1 | `carla@unb.br`, `Carla Dias`, `Cd333`, `DESENVOLVEDOR` | `[OK] Pessoa criada com sucesso` |
| 5 | Ler pessoa (serviço 2) | Pessoas → 2 | `bruno@unb.br` | Dados de Bruno Lima |
| 6 | Atualizar pessoa (serviço 3) | Pessoas → 3 | `carla@unb.br`, `Carla Souza`, `Cd999`, `DESENVOLVEDOR` | `[OK] Pessoa atualizada com sucesso` |
| 7 | Listar pessoas (serviço 5) | Pessoas → 5 | — | ana, bruno e carla |
| | Voltar | Pessoas → 6 | | |
| 8 | Criar projeto (serviço 6) | Projetos (2) → 1 | `PRJ100`, `Sistema Kanban`, `Trabalho de TP1.`, `01-MAR-2026-08:00`, `30-JUN-2026-18:00` | `[OK] Projeto criado com sucesso` |
| 9 | Criar projeto extra | Projetos → 1 | `PRJ200`, `Projeto Extra`, `Sera excluido.`, `01-MAR-2026-08:00`, `30-ABR-2026-18:00` | `[OK] Projeto criado com sucesso` |
| 10 | Ler projeto (serviço 7) | Projetos → 2 | `PRJ100` | Dados de Sistema Kanban |
| 11 | Atualizar projeto (serviço 8) | Projetos → 3 | `PRJ100`, `Kanban Final`, `Versao final.`, `01-MAR-2026-08:00`, `31-JUL-2026-18:00` | `[OK] Projeto atualizado com sucesso` |
| 12 | Listar projetos (serviço 10) | Projetos → 5 | — | PRJ100 e PRJ200 |
| | Voltar | Projetos → 6 | | |
| 13 | Criar quadro (serviço 11) | Quadros (3) → 1 | `PRJ100`, `QDR100`, `Sprint um`, `3` | `[OK] Quadro criado com sucesso` |
| 14 | Criar quadro extra | Quadros → 1 | `PRJ100`, `QDR200`, `Quadro extra`, `2` | `[OK] Quadro criado com sucesso` |
| 15 | Ler quadro (serviço 12) | Quadros → 2 | `QDR100` | Dados de Sprint um, limite 3 |
| 16 | Atualizar quadro (serviço 13) | Quadros → 3 | `QDR100`, `Sprint final`, `5` | `[OK] Quadro atualizado com sucesso` |
| 17 | Listar quadros (serviço 15) | Quadros → 5 | `PRJ100` | QDR100 e QDR200 |
| | Voltar | Quadros → 6 | | |
| 18 | Criar cartão (serviço 16) | Cartões (4) → 1 | `QDR100`, `CRT100`, `Tela login`, `Fazer a tela.`, `ALTA`, `MEDIO`, `bruno@unb.br` | `[OK] Cartao criado com sucesso na coluna A FAZER` |
| 19 | Criar cartão extra | Cartões → 1 | `QDR100`, `CRT200`, `Tela menu`, `Fazer o menu.`, `BAIXA`, `PEQUENO`, `bruno@unb.br` | `[OK] Cartao criado com sucesso na coluna A FAZER` |
| 20 | Ler cartão (serviço 17) | Cartões → 2 | `CRT100` | Dados do cartão, estado A FAZER |
| 21 | Atualizar cartão (serviço 18) | Cartões → 3 | `CRT100`, `Tela de login`, `Tela com senha.`, `MEDIA`, `GRANDE` | `[OK] Cartao atualizado com sucesso` |
| 22 | Listar cartões (serviço 20) | Cartões → 5 | `QDR100` | CRT100 e CRT200 |
| 23 | Mover cartão (serviço 21) | Cartões → 6 | `CRT100` | `[OK] Cartao movido para FAZENDO` |
| 24 | Mover cartão | Cartões → 6 | `CRT100` | `[OK] Cartao movido para FEITO` |
| 25 | Mover cartão extra | Cartões → 6 | `CRT200` | `[OK] Cartao movido para FAZENDO` |
| 26 | Ler tempo de ciclo (serviço 22) | Cartões → 7 | `CRT100` | Tempo de ciclo em minutos |
| 27 | Ler tempo de ciclo médio (serviço 23) | Cartões → 8 | `QDR100` | Média em minutos |
| 28 | Ler lead time (serviço 24) | Cartões → 9 | `CRT100` | Lead time em minutos |
| 29 | Ler lead time médio (serviço 25) | Cartões → 10 | `QDR100` | Média em minutos |
| 30 | Ler WIP (serviço 26) | Cartões → 11 | `QDR100` | `WIP: 1` (CRT200 em FAZENDO) |
| 31 | Excluir cartão (serviço 19) | Cartões → 4 | `CRT200`, `S` | `[OK] Cartao excluido com sucesso` |
| | Voltar | Cartões → 12 | | |
| 32 | Excluir quadro (serviço 14) | Quadros (3) → 4 | `QDR200`, `S` | `[OK] Quadro excluido com sucesso` |
| 33 | Excluir projeto (serviço 9) | Projetos (2) → 4 | `PRJ200`, `S` | `[OK] Projeto excluido com sucesso` |
| 34 | Excluir pessoa (serviço 4) | Pessoas (1) → 4 | `carla@unb.br`, `S` | `[OK] Pessoa excluida com sucesso` |
| | Sair | Menu principal → 5, Menu inicial → 3 | | `Sessao encerrada`, `Sistema encerrado` |

## Cobertura

| Funcionalidade | Passo |
|---|---|
| Criar conta / autenticar | 1, 3 |
| Pessoa: criar, ler, atualizar, excluir, listar (1 a 5) | 4, 5, 6, 34, 7 |
| Projeto: criar, ler, atualizar, excluir, listar (6 a 10) | 8, 10, 11, 33, 12 |
| Quadro: criar, ler, atualizar, excluir, listar (11 a 15) | 13, 15, 16, 32, 17 |
| Cartão: criar, ler, atualizar, excluir, listar (16 a 20) | 18, 20, 21, 31, 22 |
| Cartão: mover (21) | 23, 24 |
| Cartão: tempo de ciclo e médio, lead time e médio, WIP (22 a 26) | 26, 27, 28, 29, 30 |
