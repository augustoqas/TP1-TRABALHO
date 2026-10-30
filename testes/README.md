# Testes

## Como rodar

```
testes\rodar.bat                     (cmd, de qualquer pasta; também por duplo clique)
bash testes/rodar.sh                 (Git Bash)
testes\rodar.bat senha               (só testes/teste_senha.cpp)
testes\rodar.bat teste_Senha.cpp     (o mesmo: maiúsculas, "teste_" e ".cpp" tanto faz)
testes\rodar.bat roteiros            (só os roteiros de tela)
```

Precisa do `g++` no PATH (o `.bat` também procura em `C:\MinGW\bin` e no MinGW do Code::Blocks).
Aberto por duplo clique, o `.bat` pausa no fim para a janela não sumir; pelo terminal, não pausa.

A saída tem uma linha `PASS`/`FAIL` por teste, o motivo de cada falha, os `AVISO`s e o `TOTAL`.
O código de saída é 0 se nenhum teste falhou (`AVISO` não conta como falha). Arquivos gerados
ficam em `testes/bin/` (ignorado pelo git).

O que o rodar faz (tudo em `testes/rodar.cpp`):
1. copia os stubs que ainda são necessários (ver [Stubs](#stubs-testesstubs));
2. compila cada `sources/*.cpp` numa biblioteca (quem não compila vira `AVISO` e fica de fora);
3. compila e roda cada `testes/teste_*.cpp` com essa biblioteca;
4. compila `testes/main_teste.cpp` (teste.exe) e `main.cpp` (kanban.exe) e confere os roteiros;
5. **build real**: compila `main.cpp` + todos os `sources/*.cpp` só com `-Iheaders`, sem stubs,
   como o Code::Blocks faria (só na rodada completa, sem filtro).

Não existe lista de testes nem de fontes para editar: **criou `testes/teste_xxx.cpp`, ele roda**.

### Como ler a última linha

| Linha final | Significado |
|---|---|
| `TUDO PASSOU` | testes passaram e o build real compila |
| `TUDO PASSOU (ha AVISOS acima)` | idem, mas leia os `AVISO`s (stub a apagar, `main_teste.cpp` desatualizado...) |
| `testes passaram, MAS o build real ainda nao compila (ver AVISOS)` | os testes usam stubs; o projeto de verdade ainda não compila no Code::Blocks |
| `testes passaram, MAS ha fontes em sources/ que nao compilam (ver AVISOS)` | rodada com filtro e algum `sources/*.cpp` quebrado |
| `HA FALHAS (procure FAIL acima)` | algum teste falhou (código de saída 1) |

Enquanto houver stub em uso, a linha dos roteiros do programa principal sai como
`roteiros kanban.exe (com stubs)` e o build real que não compila é só `AVISO` (mostra o primeiro
erro; o completo fica em `testes/bin/build_real.txt`). Quando nenhum stub estiver em uso, build
real quebrado vira `FAIL`.

## Adicionar teste de domínio

`TesteDominio` pressupõe a **convenção do projeto**: `void setValor(...)` lança
`std::invalid_argument` para valor inválido e `getValor()` é `const`. `Email`, `Nome` e `Senha` já
seguem. `Limite`, `Papel`, `Prioridade`, `Tamanho` e `Estado` ainda **não** (o `setValor` devolve
`bool` e o `getValor` não é `const`): com eles, `TesteDominio` não compila. Até o domínio ser
convertido, ou converta-o primeiro (recomendado), ou teste com `verificar(...)` direto (forma B).

**Forma A: domínio que segue a convenção** (`testes/teste_identificador.cpp`, copie e troque os nomes):

```cpp
// Testes do dominio Identificador: 3 letras maiusculas seguidas de 3 digitos.
#include "identificador.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Identificador> identificador("ABC123");  // um valor valido qualquer

    identificador.valido("PRJ001", "letras e digitos");
    identificador.invalido("PRJ01", "so 2 digitos");
    identificador.invalido("prj001", "minusculas");

    return resultado();
}
```

`valido`: `setValor` não lança e `getValor` devolve a entrada. `invalido`: `setValor` lança
`std::invalid_argument` e o valor anterior fica. Domínio com `int` (depois de convertido):
`TesteDominio<Limite, int> limite(5);`. Bytes especiais: `std::string("ab") + '\0' + "12"`; depois
de `\xHH` feche a string: `"\xE9" "b12"`.

**Forma B: domínio ainda no formato antigo** (`setValor` devolve `bool`), ex. `testes/teste_limite.cpp`:

```cpp
// Testes do dominio Limite (formato antigo: setValor devolve bool). Inteiro de 1 a 25.
#include "limite.hpp"
#include "teste.hpp"

int main() {
    Limite limite;

    verificar(limite.setValor(5) && limite.getValor() == 5, "V 5");
    verificar(limite.setValor(25) && limite.getValor() == 25, "V 25 (maximo)");
    verificar(!limite.setValor(0) && limite.getValor() == 25, "I 0 (valor anterior fica)");
    verificar(!limite.setValor(26) && limite.getValor() == 25, "I 26 (valor anterior fica)");

    return resultado();
}
```

Quando o domínio for convertido, a forma B deixa de compilar (`!` em `void`): troque pela forma A.

## Adicionar teste de entidade ou serviço

Mesmo arquivo `testes/teste_xxx.cpp`, usando `verificar(condicao, "descricao")` e `return resultado();`.
Modelos: `teste_pessoa.cpp` (entidade) e `teste_servicopessoa.cpp` (serviço: contêineres locais no
`main`, serviço usado pela interface).

## API mínima que os testes assumem das classes reais

Os stubs imitam só isto; a classe real de cada colega precisa oferecer pelo menos o mesmo, senão
`teste_servicopessoa.cpp`, `main_teste.cpp` e os contêineres deixam de compilar:

| Classe | O que é usado |
|---|---|
| `Identificador` | construtor padrão, cópia, `void setValor(std::string)` (aceita `"PRJ001"` e `"CRT001"`; pode lançar para inválido), `std::string getValor() const` |
| `Projeto` | construtor padrão, cópia/atribuição (fica em `std::map`), `void setIdentificador(const Identificador&)`, `getIdentificador() const` devolvendo `Identificador` (ou `const Identificador&`) |
| `CartaoAtividade` | igual a `Projeto`: construtor padrão, cópia/atribuição, `setIdentificador(const Identificador&)`, `getIdentificador() const` |
| `Quadro` | só precisa existir (`interfaces.hpp` o usa em parâmetros) |

Os domínios testados com `TesteDominio` seguem a convenção da seção anterior.

## Adicionar teste de tela (roteiro)

Crie `testes/roteiros/NN_nome.txt` (o que o usuário digita, uma resposta por linha) e
`testes/roteiros/NN_nome.esperado`. Exemplo (pode copiar como está):

```
# Criar conta e sair. Anotacoes vao sempre em linhas proprias comecando com #.
# "> trecho": deve aparecer, nesta ordem (maiusculas tanto faz).
> Conta criada com sucesso
> Sistema encerrado
# "! trecho": nao pode aparecer em lugar nenhum da saida.
! Pw123
# Marcas opcionais, sozinhas na linha: @apenas_teste e @sem_trechos.
```

Regras do `.esperado` (cada linha é aparada antes de ser lida):
- vale só linha vazia, `# ...`, `> trecho`, `! trecho`, `@apenas_teste` e `@sem_trechos`;
  **qualquer outra linha é `FAIL`** (`linha invalida no .esperado: ...`), inclusive anotação no fim
  de uma linha `>` (ela viraria parte do trecho procurado);
- precisa ter pelo menos um `>`; um roteiro sem trechos de propósito (ex.: `11_eof_vazio`) leva
  `@sem_trechos`;
- `@apenas_teste`: só roda no `teste.exe`, que já tem PRJ001 e CRT001 (ver abaixo).

Cada roteiro roda no `kanban.exe` (`main.cpp`) e no `teste.exe` (`testes/main_teste.cpp`), com limite
de 5 s e 1 MB de saída (pega laço infinito). A saída de cada execução fica em
`testes/bin/roteiros/<programa>_<roteiro>.txt`. Espaço no fim de linha e arquivo sem quebra final
são de propósito (o `.gitattributes` impede o git de mexer nos `.txt` dos roteiros).

### `main_teste.cpp`

É uma cópia do `main.cpp` com a pré-carga de PRJ001 e CRT001 entre `// INICIO DADOS DE TESTE` e
`// FIM DADOS DE TESTE` (pode haver mais de um bloco). O rodar compara os dois arquivos **fora
desses blocos** (ignorando `\r`, linhas em branco, espaços nas pontas e o comentário de cabeçalho)
e dá `AVISO ... difere do main.cpp ... Atualize o testes/main_teste.cpp` mostrando a primeira
diferença. Ao plugar Projeto/Quadro/Cartão no `main.cpp`, copie a mudança para o `main_teste.cpp`.

Quando os serviços (e telas) de Projeto e Cartão existirem, PRJ001/CRT001 passam a ser criados pelo
menu dentro dos próprios roteiros (como já se faz com as pessoas). Aí `testes/main_teste.cpp` e as
marcas `@apenas_teste` podem ser apagados: sem `main_teste.cpp`, o rodar pula o `teste.exe` (e um
roteiro que ainda tenha `@apenas_teste` vira `FAIL`, para a marca não ficar esquecida).

### Demonstração para o vídeo

O roteiro `90_demo` é o passo a passo da demonstração. Depois de uma rodada do rodar, dá para
repeti-lo à mão (da raiz do repositório, no cmd):

```
testes\bin\kanban.exe < testes\roteiros\90_demo.txt
```

A saída da última rodada automática está em `testes/bin/roteiros/kanban_90_demo.txt`. Para ver o
programa interativo, rode `testes\bin\kanban.exe` sem o `<`.

## Stubs (`testes/stubs/`)

`identificador`, `projeto`, `quadro` e `cartaoatividade` existem só para o serviço e as telas de
Pessoa compilarem enquanto as classes reais não existem. Regra: **o stub de X é desligado sempre que
`headers/X.hpp` existir com qualquer conteúdo além de espaços e linhas em branco**. Aí o rodar usa a
classe real (e `sources/X.cpp`) e avisa: `AVISO stub desligado ... Apague testes/stubs/X.hpp`.
Então apague o arquivo. O rodar recria `testes/bin/stubs` a cada rodada, então nenhuma cópia velha
esconde a classe real.

Os stubs usam a guarda `STUB_<NOME>_HPP_INCLUDED` (diferente da classe real): se stub e classe real
entrarem juntos por acidente, o g++ acusa `redefinition` em vez de usar um deles calado.

Por que a regra olha o arquivo inteiro: os stubs vêm antes de `headers/` no `-I`, mas isso só vale
para `#include "x.hpp"` escrito **fora** de `headers/`. Dentro de `headers/`, o g++ procura primeiro
na própria pasta, então um `headers/X.hpp` com conteúdo sempre ganharia do stub nesses includes.

Não há stub de `estado`, `texto` nem `timestamp`: ninguém os usa ainda; quando forem usados,
precisam ser reais.
