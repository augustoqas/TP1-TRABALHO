# -*- coding: utf-8 -*-
"""
DADOS DO MODELO DE ARQUITETURA  -  edite este arquivo e rode:  python gerar.py

Tudo o que aparece no documento (diagrama UML, tabelas, textos) sai daqui.
O gerar.py so contem codigo: calcula o layout do diagrama sozinho, entao
NINGUEM precisa mexer em coordenadas.

Como fazer as alteracoes mais comuns
-----------------------------------
* Novo metodo numa interface: acrescente um dict em INTERFACES[...]["metodos"]
  (nome, id do servico ou None, assinatura C++). A tabela de interfaces, a
  faixa de servicos (ex.: "16-26") e o bloco de assinaturas se atualizam.
* Novo modulo: acrescente um dict em MODULOS (pacote, nivel, coluna...) e,
  se ele prove/requer interfaces, cite-o em INTERFACES (provida_por /
  requerida_por). Uma entidade nova pede tambem uma entrada em COLUNAS.
* Nova dependencia de armazenamento: acrescente em DEPENDENCIAS
  ("primaria" aparece no diagrama; "secundaria" so na tabela).

Regras do layout automatico
---------------------------
* "nivel" = linha do diagrama (0 = topo). Um consumidor deve ficar no nivel
  imediatamente acima do provedor (ball-and-socket vertical) e a origem de
  uma dependencia primaria no nivel imediatamente acima do destino.
* "coluna" = entidade da lista COLUNAS. Se o modulo nao tiver "coluna", ele
  ocupa automaticamente as colunas dos modulos com que se liga
  (ex.: Controle ocupa as 5; Pessoas ocupa Autenticacao e Pessoa).

Mini-marcacao nos textos: `codigo` vira fonte monoespacada e **x** vira negrito.
Os textos sao escapados para HTML automaticamente (pode usar <, >, &).
"""

# --------------------------------------------------------------------------
# Cabecalho do documento
# --------------------------------------------------------------------------
DOCUMENTO = {
    "titulo": "Modelo de Arquitetura",
    "subtitulo": "Sistema de gestão de projetos pelo método Kanban",
    "disciplina": "Técnicas de Programação 1 (CIC0197)",
    "instituicao": "Universidade de Brasília",
    "equipe": ["Augusto Queiroz", "Flávio de Matos", "Pedro Barreto Cavalcante do Amaral"],
    "local": "Brasília",
    "ano": "2026",
}

# --------------------------------------------------------------------------
# Colunas do diagrama (uma por entidade), da esquerda para a direita.
# --------------------------------------------------------------------------
COLUNAS = ["autenticacao", "pessoa", "projeto", "quadro", "cartao"]

# --------------------------------------------------------------------------
# Pacotes (camadas). "pai" aninha um pacote dentro de outro.
# As cores sao usadas no diagrama e no texto ("componente" = preenchimento das caixas).
# --------------------------------------------------------------------------
PACOTES = [
    {"id": "apresentacao", "nome": "Camada de Apresentação", "pai": None,
     "prefixo": "Apres.", "titulo_secao": "Camada de apresentação",
     "fundo": "#eaf0f6", "borda": "#5b7389", "forte": "#33506b", "componente": "#d9e4ef"},
    {"id": "servico", "nome": "Camada de Serviço", "pai": None,
     "prefixo": "Serviço", "titulo_secao": "Camada de serviço",
     "fundo": "#ebf3ec", "borda": "#5f8366", "forte": "#365c3e", "componente": "#d8e9db"},
    {"id": "armazenamento", "nome": "Armazenamento", "pai": "servico",
     "prefixo": "Armaz.", "titulo_secao": "Armazenamento (dentro da camada de serviço)",
     "fundo": "#f6f0e4", "borda": "#94815c", "forte": "#665534", "componente": "#ecdfc6"},
]

# --------------------------------------------------------------------------
# Modulos (componentes UML).  classe -> nome da classe C++ (fonte mono);
# descricao -> subtitulo em italico quando nao ha classe.
# rotulo (opcional) -> nome usado nas tabelas no lugar de "<prefixo do pacote> <nome>".
# --------------------------------------------------------------------------
MODULOS = [
    # ---------------- camada de apresentacao ----------------
    {"id": "apr_controle", "pacote": "apresentacao", "nivel": 0,
     "nome": "Controle", "rotulo": "Controle", "classe": "CntrApresentacaoControle",
     "resp": "Ponto de entrada da interação com o usuário. Exibe o menu inicial, que permite "
             "criar conta (via `IApresentacaoPessoa::cadastrar`) ou autenticar (via "
             "`IApresentacaoAutenticacao::executar`). Após a autenticação, exibe o menu principal "
             "e delega cada opção ao módulo correspondente por `executar`, passando o Email do "
             "usuário autenticado. Não acessa a camada de serviço."},
    {"id": "apr_aut", "pacote": "apresentacao", "nivel": 1, "coluna": "autenticacao",
     "nome": "Autenticação", "classe": "CntrApresentacaoAutenticacao",
     "resp": "Solicita email e senha, monta os domínios `Email` e `Senha` com `setValor` "
             "(tratando `std::invalid_argument`) e chama `IServicoAutenticacao::autenticar`. "
             "Em caso de sucesso devolve ao Controle o Email do usuário autenticado; caso "
             "contrário, informa a falha."},
    {"id": "apr_pessoa", "pacote": "apresentacao", "nivel": 1, "coluna": "pessoa",
     "nome": "Pessoa", "classe": "CntrApresentacaoPessoa",
     "resp": "Oferece a criação de conta sem sessão (`cadastrar`, serviço 1) e o menu de "
             "pessoas (serviços 1 a 5). Lê e valida email, nome, senha e papel, monta a "
             "entidade `Pessoa`, chama `IServicoPessoa` e exibe os dados, a lista de emails ou "
             "a mensagem de recusa."},
    {"id": "apr_projeto", "pacote": "apresentacao", "nivel": 1, "coluna": "projeto",
     "nome": "Projeto", "classe": "CntrApresentacaoProjeto",
     "resp": "Menu de projetos (serviços 6 a 10). Lê e valida identificador, nome, descrição, "
             "início e término, chama `IServicoProjeto` e apresenta o resultado; quando o "
             "serviço recusa (por exemplo, usuário que não é GESTOR ou projeto com quadros), "
             "exibe mensagem ao usuário."},
    {"id": "apr_quadro", "pacote": "apresentacao", "nivel": 1, "coluna": "quadro",
     "nome": "Quadro", "classe": "CntrApresentacaoQuadro",
     "resp": "Menu de quadros (serviços 11 a 15). Lê e valida o projeto ao qual o quadro "
             "pertence, identificador, nome e limite de WIP, chama `IServicoQuadro` e lista "
             "os quadros de um projeto."},
    {"id": "apr_cartao", "pacote": "apresentacao", "nivel": 1, "coluna": "cartao",
     "nome": "Cartão", "classe": "CntrApresentacaoCartao",
     "resp": "Menu de cartões de atividade (serviços 16 a 26): criar, ler, atualizar, excluir, "
             "listar e mover cartões, e consultar as métricas (tempo de ciclo, lead time e "
             "respectivas médias, em minutos, e WIP do quadro). Valida os dados de entrada e "
             "formata os resultados de `IServicoCartao`."},

    # ---------------- camada de servico ----------------
    {"id": "srv_aut", "pacote": "servico", "nivel": 2, "coluna": "autenticacao",
     "nome": "Autenticação", "classe": "CntrServicoAutenticacao",
     "resp": "Verifica se existe pessoa com o email e a senha informados, consultando o "
             "contêiner de pessoas. Não mantém sessão: o Email do usuário autenticado fica "
             "com a camada de apresentação e é repassado nas chamadas seguintes."},
    {"id": "srv_pessoa", "pacote": "servico", "nivel": 2, "coluna": "pessoa",
     "nome": "Pessoa", "classe": "CntrServicoPessoa",
     "resp": "Mantém as pessoas (serviços 1 a 5) e garante a unicidade do Email. `criar` "
             "funciona sem sessão; `atualizar` não altera a chave. Recusa excluir quem é gestor "
             "de projeto ou desenvolvedor de cartão e recusa a troca de papel que deixaria um "
             "projeto sem GESTOR ou um cartão sem DESENVOLVEDOR."},
    {"id": "srv_projeto", "pacote": "servico", "nivel": 2, "coluna": "projeto",
     "nome": "Projeto", "classe": "CntrServicoProjeto",
     "resp": "Mantém os projetos (serviços 6 a 10). Somente GESTOR cria, atualiza e exclui; "
             "o gestor do projeto é o usuário autenticado que o criou. Garante a unicidade do "
             "Identificador e recusa excluir projeto que possui quadros (sem cascata)."},
    {"id": "srv_quadro", "pacote": "servico", "nivel": 2, "coluna": "quadro",
     "nome": "Quadro", "classe": "CntrServicoQuadro",
     "resp": "Mantém os quadros (serviços 11 a 15), sempre vinculados a um projeto existente. "
             "Somente GESTOR cria, atualiza e exclui. Recusa novo limite menor que o WIP atual "
             "e recusa excluir quadro que possui cartões; lista os quadros de um projeto."},
    {"id": "srv_cartao", "pacote": "servico", "nivel": 2, "coluna": "cartao",
     "nome": "Cartão", "classe": "CntrServicoCartao",
     "resp": "Mantém os cartões (serviços 16 a 26). Cria o cartão em A FAZER com entrada = "
             "momento atual, atribuído a uma pessoa com papel DESENVOLVEDOR. `mover` faz "
             "A FAZER → FAZENDO (registra início) e FAZENDO → FEITO (registra término), "
             "respeitando o limite de WIP do quadro. Calcula tempo de ciclo (término − início), "
             "lead time (término − entrada), as médias por quadro e o WIP."},

    # ---------------- armazenamento (dentro da camada de servico) ----------------
    {"id": "arm_pessoas", "pacote": "armazenamento", "nivel": 3,
     "nome": "Pessoas", "descricao": "contêiner em memória",
     "resp": "Contêiner em memória das pessoas cadastradas, indexado pelo Email (chave "
             "primária)."},
    {"id": "arm_projetos", "pacote": "armazenamento", "nivel": 3,
     "nome": "Projetos", "descricao": "contêiner em memória",
     "resp": "Contêiner em memória dos projetos, indexado pelo Identificador (chave "
             "primária)."},
    {"id": "arm_quadros", "pacote": "armazenamento", "nivel": 3,
     "nome": "Quadros", "descricao": "contêiner em memória",
     "resp": "Contêiner em memória dos quadros, indexado pelo Identificador (chave "
             "primária)."},
    {"id": "arm_cartoes", "pacote": "armazenamento", "nivel": 3,
     "nome": "Cartões de Atividade", "descricao": "contêiner em memória",
     "resp": "Contêiner em memória dos cartões de atividade, indexado pelo Identificador "
             "(chave primária)."},
]

# Frase comum a todo o armazenamento (aparece na secao de responsabilidades).
ARMAZENAMENTO_COMUM = (
    "Cada contêiner guarda as entidades em memória, indexadas pela chave primária, e é "
    "acessado somente pela camada de serviço: a apresentação nunca o acessa. A forma de "
    "compartilhamento dos contêineres entre as controladoras de serviço será definida na "
    "implementação."
)

# --------------------------------------------------------------------------
# Interfaces.  metodos: nome, id (numero do servico do enunciado ou None),
# rotulo (opcional, texto da tabela) e assinatura (sem "virtual" e "= 0").
# servicos_texto: usado quando nenhum metodo tem id.
# --------------------------------------------------------------------------
INTERFACES = [
    {"nome": "IServicoAutenticacao", "provida_por": "srv_aut", "requerida_por": ["apr_aut"],
     "servicos_texto": "autenticação (sem nº)",
     "metodos": [
         {"nome": "autenticar", "id": None,
          "assinatura": "bool autenticar(const Email& email, const Senha& senha)"},
     ]},
    {"nome": "IServicoPessoa", "provida_por": "srv_pessoa", "requerida_por": ["apr_pessoa"],
     "metodos": [
         {"nome": "criar", "id": 1, "assinatura": "bool criar(const Pessoa& pessoa)"},
         {"nome": "ler", "id": 2,
          "assinatura": "bool ler(const Email& usuario, const Email& email, Pessoa& pessoa)"},
         {"nome": "atualizar", "id": 3,
          "assinatura": "bool atualizar(const Email& usuario, const Pessoa& pessoa)"},
         {"nome": "excluir", "id": 4,
          "assinatura": "bool excluir(const Email& usuario, const Email& email)"},
         {"nome": "listar", "id": 5,
          "assinatura": "bool listar(const Email& usuario, std::vector<Email>& lista)"},
     ]},
    {"nome": "IServicoProjeto", "provida_por": "srv_projeto", "requerida_por": ["apr_projeto"],
     "metodos": [
         {"nome": "criar", "id": 6,
          "assinatura": "bool criar(const Email& usuario, const Projeto& projeto)"},
         {"nome": "ler", "id": 7,
          "assinatura": "bool ler(const Email& usuario, const Identificador& identificador, Projeto& projeto)"},
         {"nome": "atualizar", "id": 8,
          "assinatura": "bool atualizar(const Email& usuario, const Projeto& projeto)"},
         {"nome": "excluir", "id": 9,
          "assinatura": "bool excluir(const Email& usuario, const Identificador& identificador)"},
         {"nome": "listar", "id": 10,
          "assinatura": "bool listar(const Email& usuario, std::vector<Identificador>& lista)"},
     ]},
    {"nome": "IServicoQuadro", "provida_por": "srv_quadro", "requerida_por": ["apr_quadro"],
     "metodos": [
         {"nome": "criar", "id": 11,
          "assinatura": "bool criar(const Email& usuario, const Identificador& projeto, const Quadro& quadro)"},
         {"nome": "ler", "id": 12,
          "assinatura": "bool ler(const Email& usuario, const Identificador& identificador, Quadro& quadro)"},
         {"nome": "atualizar", "id": 13,
          "assinatura": "bool atualizar(const Email& usuario, const Quadro& quadro)"},
         {"nome": "excluir", "id": 14,
          "assinatura": "bool excluir(const Email& usuario, const Identificador& identificador)"},
         {"nome": "listar", "id": 15,
          "assinatura": "bool listar(const Email& usuario, const Identificador& projeto, std::vector<Identificador>& lista)"},
     ]},
    {"nome": "IServicoCartao", "provida_por": "srv_cartao", "requerida_por": ["apr_cartao"],
     "metodos": [
         {"nome": "criar", "id": 16,
          "assinatura": "bool criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor, const CartaoAtividade& cartao)"},
         {"nome": "ler", "id": 17,
          "assinatura": "bool ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao)"},
         {"nome": "atualizar", "id": 18,
          "assinatura": "bool atualizar(const Email& usuario, const CartaoAtividade& cartao)"},
         {"nome": "excluir", "id": 19,
          "assinatura": "bool excluir(const Email& usuario, const Identificador& identificador)"},
         {"nome": "listar", "id": 20,
          "assinatura": "bool listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista)"},
         {"nome": "mover", "id": 21,
          "assinatura": "bool mover(const Email& usuario, const Identificador& identificador)"},
         {"nome": "lerTempoCiclo", "id": 22,
          "assinatura": "bool lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos)"},
         {"nome": "lerTempoCicloMedio", "id": 23,
          "assinatura": "bool lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos)"},
         {"nome": "lerLeadTime", "id": 24,
          "assinatura": "bool lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos)"},
         {"nome": "lerLeadTimeMedio", "id": 25,
          "assinatura": "bool lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos)"},
         {"nome": "lerWIP", "id": 26,
          "assinatura": "bool lerWIP(const Email& usuario, const Identificador& quadro, int& wip)"},
     ]},

    # As IApresentacao* nao tem ids proprios: a faixa de servicos e herdada da
    # IServico* que o modulo provedor requer (calculado pelo gerar.py).
    {"nome": "IApresentacaoAutenticacao", "provida_por": "apr_aut", "requerida_por": ["apr_controle"],
     "metodos": [
         {"nome": "executar", "id": None, "rotulo": "executar(Email&)",
          "assinatura": "bool executar(Email& email)"},
         {"nome": "setCntrServicoAutenticacao", "id": None,
          "assinatura": "void setCntrServicoAutenticacao(IServicoAutenticacao* cntr)"},
     ]},
    {"nome": "IApresentacaoPessoa", "provida_por": "apr_pessoa", "requerida_por": ["apr_controle"],
     "servicos_obs": "cadastrar = 1, sem sessão",
     "metodos": [
         {"nome": "executar", "id": None, "rotulo": "executar(const Email&)",
          "assinatura": "void executar(const Email& usuario)"},
         {"nome": "cadastrar", "id": None, "rotulo": "cadastrar()",
          "assinatura": "bool cadastrar()"},
         {"nome": "setCntrServicoPessoa", "id": None,
          "assinatura": "void setCntrServicoPessoa(IServicoPessoa* cntr)"},
     ]},
    {"nome": "IApresentacaoProjeto", "provida_por": "apr_projeto", "requerida_por": ["apr_controle"],
     "metodos": [
         {"nome": "executar", "id": None, "rotulo": "executar(const Email&)",
          "assinatura": "void executar(const Email& usuario)"},
         {"nome": "setCntrServicoProjeto", "id": None,
          "assinatura": "void setCntrServicoProjeto(IServicoProjeto* cntr)"},
     ]},
    {"nome": "IApresentacaoQuadro", "provida_por": "apr_quadro", "requerida_por": ["apr_controle"],
     "metodos": [
         {"nome": "executar", "id": None, "rotulo": "executar(const Email&)",
          "assinatura": "void executar(const Email& usuario)"},
         {"nome": "setCntrServicoQuadro", "id": None,
          "assinatura": "void setCntrServicoQuadro(IServicoQuadro* cntr)"},
     ]},
    {"nome": "IApresentacaoCartao", "provida_por": "apr_cartao", "requerida_por": ["apr_controle"],
     "metodos": [
         {"nome": "executar", "id": None, "rotulo": "executar(const Email&)",
          "assinatura": "void executar(const Email& usuario)"},
         {"nome": "setCntrServicoCartao", "id": None,
          "assinatura": "void setCntrServicoCartao(IServicoCartao* cntr)"},
     ]},
]

# --------------------------------------------------------------------------
# Dependencias de uso da camada de servico para o armazenamento.
# primaria   -> desenhada no diagrama (tracejado com seta aberta, «use»)
# secundaria -> consulta de consistencia; so na tabela (motivo obrigatorio)
# --------------------------------------------------------------------------
DEPENDENCIAS = [
    {"de": "srv_aut", "para": "arm_pessoas", "tipo": "primaria"},
    {"de": "srv_pessoa", "para": "arm_pessoas", "tipo": "primaria"},
    {"de": "srv_projeto", "para": "arm_projetos", "tipo": "primaria"},
    {"de": "srv_quadro", "para": "arm_quadros", "tipo": "primaria"},
    {"de": "srv_cartao", "para": "arm_cartoes", "tipo": "primaria"},

    {"de": "srv_pessoa", "para": "arm_projetos", "tipo": "secundaria",
     "motivo": "Recusar exclusão ou troca de papel de quem é gestor de algum projeto."},
    {"de": "srv_pessoa", "para": "arm_cartoes", "tipo": "secundaria",
     "motivo": "Recusar exclusão ou troca de papel de quem é desenvolvedor de algum cartão."},
    {"de": "srv_projeto", "para": "arm_pessoas", "tipo": "secundaria",
     "motivo": "Conferir se o usuário tem papel GESTOR (criar, atualizar, excluir)."},
    {"de": "srv_projeto", "para": "arm_quadros", "tipo": "secundaria",
     "motivo": "Recusar exclusão de projeto que possui quadros."},
    {"de": "srv_quadro", "para": "arm_pessoas", "tipo": "secundaria",
     "motivo": "Conferir o papel do usuário (somente GESTOR altera)."},
    {"de": "srv_quadro", "para": "arm_projetos", "tipo": "secundaria",
     "motivo": "Conferir se o projeto do quadro existe."},
    {"de": "srv_quadro", "para": "arm_cartoes", "tipo": "secundaria",
     "motivo": "Recusar exclusão de quadro com cartões; exigir novo limite ≥ WIP atual."},
    {"de": "srv_cartao", "para": "arm_pessoas", "tipo": "secundaria",
     "motivo": "Conferir papel GESTOR do usuário; conferir se o desenvolvedor existe e tem papel DESENVOLVEDOR."},
    {"de": "srv_cartao", "para": "arm_quadros", "tipo": "secundaria",
     "motivo": "Conferir se o quadro existe; respeitar o limite de WIP ao mover para FAZENDO."},
]

# Nota UML exibida no diagrama. {secao} e {tabela} sao preenchidos pelo gerador.
NOTA_DIAGRAMA = {
    "pacote": "servico", "ancora": "armazenamento",
    "texto": "Dependências secundárias (consultas de consistência entre entidades, por exemplo "
             "Serviço Cartão → Quadros para o limite de WIP) foram omitidas para não poluir o "
             "diagrama. Estão documentadas na Seção {secao}, Tabela {tabela}.",
}

# --------------------------------------------------------------------------
# Arquitetura de referencia do enunciado -> modulos deste modelo
# --------------------------------------------------------------------------
REFERENCIA = [
    {"elemento": "APRESENTAÇÃO",
     "modulos": ["apr_controle", "apr_aut", "apr_pessoa", "apr_projeto", "apr_quadro", "apr_cartao"]},
    {"elemento": "SERVIÇO",
     "modulos": ["srv_aut", "srv_pessoa", "srv_projeto", "srv_quadro", "srv_cartao"]},
    {"elemento": "PESSOAS, PROJETOS, QUADROS, CARTÕES DE ATIVIDADES",
     "modulos": ["arm_pessoas", "arm_projetos", "arm_quadros", "arm_cartoes"]},
]

# --------------------------------------------------------------------------
# Textos da secao "Visao geral".  Blocos: ("p", texto) | ("ul", [itens]) |
# ("h3", titulo) | ("caixa", titulo, [itens]) | ("referencia",) -> tabela
# gerada a partir de REFERENCIA | ("fluxo", titulo, [passos]).
# --------------------------------------------------------------------------
VISAO_GERAL = [
    ("p", "O sistema de gestão de projetos pelo método Kanban adota o **estilo arquitetural em camadas**, com duas "
          "camadas: a **camada de apresentação** e a **camada de serviço**. Cada camada é "
          "decomposta em módulos, e os módulos interagem exclusivamente por meio de serviços "
          "especificados em interfaces: classes abstratas C++ declaradas em "
          "`headers/interfaces.hpp`, compostas somente por métodos virtuais puros."),
    ("ul", [
        "**Camada de apresentação** — interface com o usuário (menus, leitura de dados, "
        "exibição de resultados e mensagens) e **validação dos dados de entrada**. A validação "
        "acontece ao montar os objetos domínio com `setValor`: quando o formato é inválido, o "
        "domínio lança `std::invalid_argument`, tratada nesta camada.",
        "**Camada de serviço** — **lógica de negócio** (papéis GESTOR e DESENVOLVEDOR, "
        "unicidade das chaves, exclusões sem inconsistência, transições de estado dos cartões, "
        "limite de WIP e métricas) e **armazenamento dos dados** em contêineres em memória, "
        "agrupados no subpacote Armazenamento.",
    ]),
    ("caixa", "Regra de dependência", [
        "A apresentação depende **somente** das interfaces `IServico*`; nunca das controladoras "
        "concretas `CntrServico*` nem do armazenamento.",
        "A camada de serviço **não conhece** a apresentação: nenhuma dependência aponta do "
        "serviço para a apresentação.",
        "O módulo Controle depende apenas das interfaces `IApresentacao*`.",
        "O `main` monta as camadas: instancia as controladoras concretas, injeta cada "
        "controladora de serviço na apresentação correspondente com `setCntrServico*` e entrega "
        "as controladoras de apresentação ao Controle.",
    ]),
    ("p", "A arquitetura de referência do enunciado (APRESENTAÇÃO → SERVIÇO → PESSOAS, PROJETOS, "
          "QUADROS, CARTÕES DE ATIVIDADES) foi refinada em módulos, um por entidade, mantendo a "
          "mesma direção das dependências:"),
    ("referencia",),
    ("fluxo", "Exemplo de interação: mover um cartão (serviço 21)", [
        "O Controle chama `IApresentacaoCartao::executar(usuario)`.",
        "A apresentação de cartões lê o identificador, monta o domínio `Identificador` "
        "(validação) e chama `IServicoCartao::mover(usuario, identificador)`.",
        "O serviço de cartões consulta o quadro (limite de WIP), atualiza estado e "
        "timestamps no contêiner de cartões e retorna `true` ou `false`.",
        "A apresentação exibe o resultado ou a mensagem de recusa.",
    ]),
]

# --------------------------------------------------------------------------
# Convencoes das interfaces (topo de headers/interfaces.hpp)
# --------------------------------------------------------------------------
CONVENCOES = [
    ("Retorno bool",
     "Todo método de serviço retorna `bool`: `true` = operação realizada; `false` = operação "
     "recusada (registro inexistente ou duplicado, permissão negada, regra de negócio violada "
     "ou exclusão que causaria inconsistência). Se retornar `false`, nada é alterado. O serviço "
     "**não lança exceções**; `std::invalid_argument` só aparece na apresentação, ao montar "
     "domínios com `setValor`."),
    ("Usuário autenticado",
     "Todo método que exige sessão recebe, como **primeiro parâmetro**, o `Email` do usuário "
     "autenticado (`usuario`), usado para consultar o papel (GESTOR ou DESENVOLVEDOR) e para "
     "definir o gestor de um projeto. Exceções: `IServicoAutenticacao::autenticar` e "
     "`IServicoPessoa::criar`, que funcionam sem sessão (criação de conta)."),
    ("Entrada",
     "Parâmetros de entrada são referências constantes (`const &`). Associações são informadas "
     "pela chave primária da outra entidade (ex.: criar quadro recebe o `Identificador` do "
     "projeto)."),
    ("Saída",
     "Parâmetros de saída são referências **não constantes** (`&`), sempre os últimos da lista, "
     "e só são preenchidos quando o método retorna `true`."),
    ("Listagens",
     "Preenchem um `std::vector` com as chaves primárias (`Email` ou `Identificador`). Retorno "
     "`true` com vetor vazio significa que não há itens."),
    ("Chave primária",
     "`atualizar` localiza o registro pela chave primária contida na entidade recebida e altera "
     "somente os demais atributos; a chave nunca é alterada."),
    ("Métricas de tempo",
     "Tempo de ciclo, lead time e médias são `double`, em **minutos**. Retornam `false` se o "
     "cartão não está em FEITO ou se o quadro não possui cartão em FEITO."),
    ("Exclusão sem cascata",
     "Exclusões que gerariam inconsistência são recusadas, nunca propagadas: projeto com "
     "quadros, quadro com cartões e pessoa vinculada a projeto ou cartão. A ordem de exclusão "
     "é cartões → quadros → projeto → pessoa."),
    ("Destrutor virtual puro",
     "Cada interface declara destrutor virtual **puro**, definido vazio (`inline`) no final de "
     "`interfaces.hpp`. Assim a interface contém somente métodos virtuais puros e o `delete` "
     "por ponteiro de interface é seguro."),
]
