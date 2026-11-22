# Membros da equipe e artefatos

Trabalho prático de **Técnicas de Programação 1 (CIC0197)** — Universidade de Brasília.

| Nome | Matrícula | GitHub |
|---|---|---|
| Pedro Barreto Cavalcante do Amaral | 232038433 | [pedrobamaral](https://github.com/pedrobamaral) |
| Augusto Queiroz | 232024302 | [augustoqas](https://github.com/augustoqas) |
| Flavio dos Santos Barros de Matos | 252033452 | [flavio-dematos](https://github.com/flavio-dematos) |

A autoria abaixo vem do histórico do repositório (`git blame`). Cada arquivo `.hpp` tem o seu `.cpp` correspondente em `sources/`, exceto `interfaces.hpp`, que só declara classes abstratas.

## Pedro Barreto Cavalcante do Amaral (232038433)

- **Domínios:** `Email`, `Senha`, `Nome`.
- **Entidade:** `Pessoa`.
- **Interfaces entre módulos:** `headers/interfaces.hpp`.
- **Camada de serviço:**
  - contêiner `ContainerPessoa`;
  - controladoras `CntrServicoAutenticacao` e `CntrServicoPessoa`.
- **Camada de apresentação:**
  - controladoras `CntrApresentacaoControle` (menus), `CntrApresentacaoAutenticacao` e `CntrApresentacaoPessoa`;
  - classe de apoio `Tela`;
  - exceção `ExcecaoSessaoEncerrada`.
- **Integração:** `main.cpp` e o projeto Code::Blocks `TP1-Kanban.cbp`.
- **Modelo de arquitetura:** `docs/arquitetura/` (PDF, diagrama UML e o gerador).
- **Regras do enunciado** (issue #43): o gestor de um projeto é quem o criou (só ele altera o projeto e cria, altera ou exclui seus quadros e cartões) e o dia do `Timestamp` pode ter 1 ou 2 dígitos.
- **Testes automatizados:**
  - estrutura em `testes/` (`rodar.bat`, `rodar.cpp`, `teste.hpp`, `README.md`);
  - testes de Email, Senha, Nome, Pessoa, serviço de Pessoa e Tela;
  - testes das regras da issue #43 (Timestamp e serviços de Projeto, Quadro e Cartão) e roteiro `34_gestor_do_projeto`;
  - teste da formatação das métricas na tela de Cartão (`teste_formatarminutos.cpp`);
  - roteiros de Pessoa, login, fim de entrada, cancelamento e a demonstração `90_demo`.
- **Vídeo do teste fumaça** (issue #25), seguindo o guia `docs/teste_fumaca.md`.
- **Documentação:** `README.md` e este arquivo.
- **Organização:** planejamento e divisão das tarefas nas issues do GitHub.

## Augusto Queiroz (232024302)

- **Domínios:** `Identificador`, `Texto`, `Timestamp`, `Estado`.
- **Entidades:** `Projeto`, `Quadro`.
- **Camada de serviço:**
  - contêiner `ContainerQuadro`;
  - controladoras `CntrServicoProjeto` e `CntrServicoQuadro`.
- **Camada de apresentação:** controladoras `CntrApresentacaoProjeto` e `CntrApresentacaoQuadro`.
- **Testes automatizados:**
  - testes de Identificador, Texto, Timestamp, Estado, Projeto, Quadro e dos serviços de Projeto e Quadro;
  - roteiros de Projeto, Quadro e do fluxo completo de um cartão.
- **Documentação Doxygen** (issue #24): `Doxyfile`, documentação HTML gerada em `docs/doxygen/` e página principal `docs/principal.md`.
- **Teste fumaça** (issue #25): roteiro `testes/roteiros/91_fumaca` e guia `docs/teste_fumaca.md`.
- **Modelo de arquitetura:** atualização de `docs/arquitetura/` conforme a implementação.

## Flavio dos Santos Barros de Matos (252033452)

- **Domínios:** `Limite`, `Papel`, `Prioridade`, `Tamanho`.
- **Entidade:** `CartaoAtividade`.
- **Camada de serviço:** controladora `CntrServicoCartao` (mover, tempo de ciclo, lead time e WIP).
- **Camada de apresentação:** controladora `CntrApresentacaoCartao`.
- **Testes automatizados:**
  - testes de Limite, Papel, Prioridade, Tamanho, CartaoAtividade e do serviço de Cartão;
  - roteiros de Cartão.

## Artefatos compartilhados

| Artefato | Autores |
|---|---|
| `main.cpp` e `testes/main_teste.cpp` (montagem do sistema) | Pedro, Augusto e Flavio |
| `ContainerProjeto` | Pedro (versão inicial) e Augusto (versão completa) |
| `ContainerCartao` | Pedro (versão inicial) e Flavio (versão completa) |
