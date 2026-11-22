# TP1 - Sistema Kanban

Sistema de gestao de projetos pelo metodo Kanban, feito para a disciplina
Tecnicas de Programacao 1 (CIC0197) da Universidade de Brasilia.

Cada projeto tem quadros, e cada quadro organiza cartoes de atividade nas
colunas A FAZER, FAZENDO e FEITO. O limite de WIP do quadro e o numero maximo de
cartoes em FAZENDO ao mesmo tempo.

## Camadas

O software segue o estilo em camadas. Os modulos so se comunicam pelas classes
abstratas declaradas em interfaces.hpp.

| Camada | Classes | Papel |
|---|---|---|
| Apresentacao | CntrApresentacaoControle, CntrApresentacaoAutenticacao, CntrApresentacaoPessoa, CntrApresentacaoProjeto, CntrApresentacaoQuadro, CntrApresentacaoCartao | Menus em modo texto e validacao da entrada pelos dominios (helpers em Tela) |
| Servico | CntrServicoAutenticacao, CntrServicoPessoa, CntrServicoProjeto, CntrServicoQuadro, CntrServicoCartao | Regras de negocio e permissoes (GESTOR e DESENVOLVEDOR) |
| Armazenamento | ContainerPessoa, ContainerProjeto, ContainerQuadro, ContainerCartao | Dados em memoria (std::map) |

## Dominios e entidades

- **Dominios** validam o proprio formato: Email, Nome, Senha, Papel, Identificador,
  Texto, Timestamp, Limite, Prioridade, Estado e Tamanho. O setValor lanca
  std::invalid_argument quando o valor e invalido.
- **Entidades** sao compostas por dominios: Pessoa, Projeto, Quadro e CartaoAtividade.

## Servicos

| Entidade | Servicos |
|---|---|
| Pessoa | 1 a 5: criar, ler, atualizar, excluir e listar |
| Projeto | 6 a 10: criar, ler, atualizar, excluir e listar |
| Quadro | 11 a 15: criar, ler, atualizar, excluir e listar |
| Cartao de atividade | 16 a 26: criar, ler, atualizar, excluir, listar, mover, tempo de ciclo (e medio), lead time (e medio) e WIP |

A montagem do sistema (conteineres, servicos, telas e menu) fica em main.cpp.
