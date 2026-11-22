<div align="center">

# 📋 TP1 · Sistema Kanban

**Sistema de gestão de projetos pelo método Kanban, em C++ com arquitetura em camadas.**

Trabalho prático de **Técnicas de Programação 1 (CIC0197)** · Universidade de Brasília

![C++11](https://img.shields.io/badge/C%2B%2B-11-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Code::Blocks](https://img.shields.io/badge/IDE-Code%3A%3ABlocks-41AD48?style=for-the-badge)
![MinGW](https://img.shields.io/badge/compilador-MinGW%20GCC-555555?style=for-the-badge)
![Doxygen](https://img.shields.io/badge/docs-Doxygen-2C4AA8?style=for-the-badge)
![Status](https://img.shields.io/badge/status-vers%C3%A3o%20de%20entrega-2EA44F?style=for-the-badge)

[Sobre](#-sobre) ·
[Funcionalidades](#-funcionalidades) ·
[Arquitetura](#%EF%B8%8F-arquitetura) ·
[Como executar](#-como-executar) ·
[Estrutura](#-estrutura-do-projeto) ·
[Equipe](#-equipe)

</div>

---

## 📖 Sobre

O sistema apoia a gestão de projetos que adotam o método **Kanban**. Cada projeto possui quadros e cada quadro organiza **cartões de atividade** em três colunas:

```mermaid
flowchart LR
    A["📝 A FAZER"] -- mover --> B["⚙️ FAZENDO"] -- mover --> C["✅ FEITO"]
```

- Cada membro do projeto é **gestor** ou **desenvolvedor**. O gestor de um projeto é quem o criou.
- Para usar o sistema, o usuário **cria uma conta** e **se autentica**.
- Cada quadro tem um **limite de WIP**: o número máximo de cartões que podem estar em **FAZENDO** ao mesmo tempo.

### Métricas do quadro

| Métrica | Como é calculada |
|---|---|
| ⏱️ **Tempo de ciclo** | Da entrada do cartão em **FAZENDO** até sua chegada em **FEITO** |
| ⏳ **Lead time** | Da entrada do cartão em **A FAZER** até sua chegada em **FEITO** |
| 📊 **Médias** | Média dos tempos de ciclo e dos lead times dos cartões do quadro |
| 🚧 **WIP** | Número de cartões na coluna **FAZENDO** |

---

## ✨ Funcionalidades

O sistema oferece **26 serviços**, além de criar conta e autenticar:

| Entidade | Serviços | 👔 Gestor | 💻 Desenvolvedor |
|---|---|:---:|:---:|
| **Pessoa** | Criar · Ler · Atualizar · Excluir · Listar | ✅ | ✅ |
| **Projeto** | Ler · Listar | ✅ | ✅ |
| | Criar | ✅ | ❌ |
| | Atualizar · Excluir | ✅ só o gestor do projeto | ❌ |
| **Quadro** | Ler · Listar | ✅ | ✅ |
| | Criar · Atualizar · Excluir | ✅ só o gestor do projeto | ❌ |
| **Cartão de atividade** | Ler · Listar · Mover | ✅ | ✅ |
| | Ler tempo de ciclo (e médio) · Ler lead time (e médio) · Ler WIP | ✅ | ✅ |
| | Criar · Atualizar · Excluir | ✅ só o gestor do projeto | ❌ |

<details>
<summary><b>📐 Regras de negócio</b></summary>
<br>

- O **gestor de um projeto é quem o criou** (qualquer pessoa com papel GESTOR pode criar projetos). Só ele atualiza ou exclui o projeto e cria, atualiza ou exclui os quadros do projeto e os cartões desses quadros. Ler, listar, mover cartões e consultar métricas ficam abertos a qualquer usuário autenticado, gestor ou desenvolvedor.
- Um cartão só se move de **A FAZER → FAZENDO** e de **FAZENDO → FEITO**, respeitando o limite de WIP do quadro.
- A **chave primária** de uma entidade (email ou identificador) não pode ser editada.
- As **multiplicidades** do diagrama de classes são sempre respeitadas.
- **Exclusão não gera inconsistência.** O sistema recusa excluir projeto que tem quadros, quadro que tem cartões e pessoa vinculada a projeto ou cartão. A ordem de exclusão é cartões → quadros → projeto → pessoa.
- A listagem mostra o identificador de cada entidade. A leitura mostra todos os dados da entidade.

</details>

---

## 🏛️ Arquitetura

O software segue o estilo **em camadas**. Os módulos se comunicam apenas por **interfaces**, declaradas em [`headers/interfaces.hpp`](headers/interfaces.hpp) como classes abstratas.

```mermaid
flowchart TB
    U(["👤 Usuário"]) --> AP

    subgraph AP["🖥️ Camada de Apresentação · interface com o usuário e validação da entrada"]
        direction LR
        CTRL["Controle<br/>(menu principal)"] --- AUT["Autenticação"] --- APE["Pessoa"] --- APR["Projeto"] --- AQU["Quadro"] --- ACA["Cartão"]
    end

    AP -- "IServico*" --> SV

    subgraph SV["⚙️ Camada de Serviço · lógica de negócio e armazenamento"]
        direction LR
        SAU["Autenticação"] --- SPE["Pessoa"] --- SPR["Projeto"] --- SQU["Quadro"] --- SCA["Cartão"]
    end

    SV --> P[("Pessoas")] & PR[("Projetos")] & Q[("Quadros")] & C[("Cartões")]
```

### Modelo de dados

```mermaid
classDiagram
    direction LR
    class Pessoa {
        Email email 🔑
        Nome nome
        Senha senha
        Papel papel
    }
    class Projeto {
        Identificador identificador 🔑
        Nome nome
        Texto descricao
        Timestamp inicio
        Timestamp termino
    }
    class Quadro {
        Identificador identificador 🔑
        Nome nome
        Limite limite
    }
    class CartaoAtividade {
        Identificador identificador 🔑
        Nome nome
        Texto descricao
        Prioridade prioridade
        Estado estado
        Tamanho tamanho
        Timestamp entrada
        Timestamp inicio
        Timestamp termino
    }
    Pessoa "1" -- "0..*" Projeto : gestor
    Pessoa "1" -- "0..*" CartaoAtividade : desenvolvedor
    Projeto "1" -- "0..*" Quadro
    Quadro "1" -- "0..*" CartaoAtividade
```

<details>
<summary><b>🧩 Domínios e formatos válidos</b></summary>
<br>

Cada atributo é uma **classe domínio**. O `setValor` lança `std::invalid_argument` quando o formato é inválido.

| Domínio | Formato válido |
|---|---|
| `Email` | `parte-local@dominio`. Parte local com até 64 caracteres (a-z, 0-9, `.` e `-`). Domínio com até 255 caracteres, em partes separadas por `.` |
| `Estado` | `A FAZER`, `FAZENDO` ou `FEITO` |
| `Identificador` | 3 letras seguidas de 3 dígitos (ex.: `PRJ001`) |
| `Limite` | Inteiro de 1 a 25 |
| `Nome` | Até 15 caracteres: letras e espaços simples entre palavras |
| `Papel` | `GESTOR` ou `DESENVOLVEDOR` |
| `Prioridade` | `ALTA`, `MEDIA` ou `BAIXA` |
| `Senha` | 5 caracteres alfanuméricos, com pelo menos uma letra e um dígito |
| `Tamanho` | `GRANDE`, `MEDIO` ou `PEQUENO` |
| `Texto` | Até 30 caracteres. Começa com letra maiúscula e termina com ponto |
| `Timestamp` | `DIA-MES-ANO-HH:MM` (ex.: `15-MAR-2026-14:30` ou `5-MAR-2026-14:30`), com dia de 1 a 31 (1 ou 2 dígitos), ano entre 2000 e 2099 e anos bissextos considerados |

</details>

---

## 🚀 Como executar

> [!NOTE]
> Esta é a **versão de entrega** do trabalho: criar conta, autenticar e os 26 serviços estão implementados e cobertos pelos testes automatizados.

### Pré-requisitos

- [Code::Blocks](https://www.codeblocks.org/downloads/) com MinGW, ou qualquer `g++` com suporte a **C++11**
- [Doxygen](https://www.doxygen.nl/) (opcional, só para gerar a documentação de novo)

### Opção 1 · Code::Blocks

1. Clone o repositório:
   ```bash
   git clone https://github.com/augustoqas/TP1-TRABALHO.git
   ```
2. Abra o arquivo de projeto `TP1-Kanban.cbp` no Code::Blocks.
3. Clique em **Build and run** (`F9`).

O projeto já vem configurado com `-std=c++11` e com as bibliotecas do MinGW embutidas no executável (`-static-libgcc -static-libstdc++`), então o `.exe` gerado roda em um PC sem MinGW instalado.

### Opção 2 · Linha de comando

Equivalente ao build Release do Code::Blocks, a partir da raiz do repositório:

```bash
g++ -std=c++11 -Wall -Wextra -O2 -Iheaders main.cpp sources/*.cpp -static-libgcc -static-libstdc++ -s -o kanban
./kanban
```

No `cmd` do Windows, rode `kanban` em vez de `./kanban`.

### Testes

```bat
testes\rodar.bat
```

Compila e roda os testes de unidade, os roteiros de tela e o build real do sistema. A última linha deve ser `TUDO PASSOU`. Detalhes em [`testes/README.md`](testes/README.md).

### Documentação

A documentação Doxygen já gerada está no repositório: abra [`docs/doxygen/html/index.html`](docs/doxygen/html/index.html) no navegador. Ela é gerada a partir dos comentários das classes (`headers/`, `sources/` e `main.cpp`), com a página principal em [`docs/principal.md`](docs/principal.md).

Para gerar de novo, depois de mudar a documentação do código, rode na raiz do repositório:

```bash
doxygen Doxyfile
```

e faça commit da pasta `docs/doxygen/`. Membros sem documentação aparecem em `docs/doxygen/avisos.txt`, que deve ficar vazio.

---

## 📁 Estrutura do projeto

```text
TP1-TRABALHO/
├── headers/            # Declarações (.hpp)
│   ├── interfaces.hpp  #   Interfaces entre os módulos
│   ├── <dominio>.hpp   #   Classes domínio (Email, Nome, Senha, ...)
│   └── <entidade>.hpp  #   Classes entidade (Pessoa, Projeto, ...)
├── sources/            # Implementações (.cpp)
├── testes/             # Testes automatizados (ver testes/README.md)
│   ├── rodar.bat       #   Roda todos os testes
│   ├── teste_*.cpp     #   Testes de unidade
│   └── roteiros/       #   Roteiros de tela, incluindo o teste fumaça (91_fumaca)
├── docs/
│   ├── arquitetura/    #   Modelo de arquitetura (PDF, diagrama UML e gerador)
│   ├── doxygen/html/   #   Documentação Doxygen gerada (abra index.html)
│   ├── principal.md    #   Página principal da documentação Doxygen
│   └── teste_fumaca.md #   Guia do vídeo do teste fumaça
├── main.cpp            # Ponto de entrada: monta as camadas e inicia o menu
├── TP1-Kanban.cbp      # Projeto Code::Blocks
├── Doxyfile            # Configuração do Doxygen
├── MEMBROS.md          # Membros da equipe e artefatos de cada um
└── README.md
```

> [!WARNING]
> **Não adicione `testes/*.cpp` ao projeto do Code::Blocks.** Cada teste (e o `testes/main_teste.cpp`) tem o seu próprio `main`, o que quebraria o build do sistema. No `.cbp` entram só `main.cpp`, `headers/` e `sources/`. Os testes rodam à parte com `testes\rodar.bat` (detalhes em [`testes/README.md`](testes/README.md)).

---

## 🤝 Como contribuir

| | Convenção |
|---|---|
| 🌿 **Branches** | Uma por tarefa, a partir da `main`: `feat/email`, `feat/pessoa`, ... |
| 💬 **Commits** | Uma linha, no padrão `Feat: Implementa classe dominio Email` |
| 🔀 **Pull Requests** | Toda mudança entra na `main` por PR. Use `Closes #N` para fechar a issue |
| 📝 **Código** | Arquivos `.hpp`/`.cpp` em minúsculas, sem acentos e sem `using namespace std` nos headers |
| 📚 **Documentação** | Doxygen (`@brief`, `@param`, `@return`, `@throw`) em toda classe e membro público |

As tarefas e os responsáveis estão nas [issues](https://github.com/augustoqas/TP1-TRABALHO/issues). O padrão completo de código está na [issue #1](https://github.com/augustoqas/TP1-TRABALHO/issues/1).

---

## 👥 Equipe

<table align="center">
  <tr>
    <td align="center" width="220">
      <a href="https://github.com/augustoqas">
        <img src="https://github.com/augustoqas.png" width="96" alt="Augusto Queiroz"/><br/>
        <b>Augusto Queiroz</b>
      </a><br/>
      <sub>Projeto · Quadro · Timestamp<br/>Doxygen · Roteiro do teste fumaça</sub>
    </td>
    <td align="center" width="220">
      <a href="https://github.com/flavio-dematos">
        <img src="https://github.com/flavio-dematos.png" width="96" alt="Flávio de Matos"/><br/>
        <b>Flávio de Matos</b>
      </a><br/>
      <sub>Cartão de atividade · Métricas<br/>Limite · Papel · Prioridade · Tamanho</sub>
    </td>
    <td align="center" width="220">
      <a href="https://github.com/pedrobamaral">
        <img src="https://github.com/pedrobamaral.png" width="96" alt="Pedro Amaral"/><br/>
        <b>Pedro Amaral</b>
      </a><br/>
      <sub>Pessoa · Autenticação · Interfaces<br/>Arquitetura · Integração · Projeto Code::Blocks</sub>
    </td>
  </tr>
</table>

---

<div align="center">
<sub>Desenvolvido para a disciplina <b>Técnicas de Programação 1</b> · Universidade de Brasília · 2026</sub>
</div>
