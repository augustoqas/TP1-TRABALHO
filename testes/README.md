# Testes

## Como rodar

```
testes\rodar.bat            (cmd, de qualquer pasta)
bash testes/rodar.sh        (Git Bash)
testes\rodar.bat senha      (so testes/teste_senha.cpp; "roteiros" roda so os roteiros)
```

Precisa do `g++` no PATH (o `.bat` tambem procura em `C:\MinGW\bin` e no MinGW do Code::Blocks).
A saida tem uma linha `PASS`/`FAIL` por teste, o motivo de cada falha e o `TOTAL`.
O codigo de saida e 0 so se tudo passou. Arquivos gerados ficam em `testes/bin/` (ignorado pelo git).

O que o rodar faz (tudo em `testes/rodar.cpp`):
1. compila cada `sources/*.cpp` numa biblioteca (quem nao compila vira `AVISO` e fica de fora);
2. compila e roda cada `testes/teste_*.cpp` com essa biblioteca;
3. compila `main.cpp` (kanban.exe) e `testes/main_teste.cpp` (teste.exe) e confere os roteiros.

Nao existe lista de testes nem de fontes para editar: **criou `testes/teste_xxx.cpp`, ele roda**.

## Adicionar teste de dominio (copie e troque os nomes)

`testes/teste_identificador.cpp`:

```cpp
// Testes do dominio Identificador: 3 letras seguidas de 3 digitos.
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

`valido`: `setValor` nao lanca e `getValor` devolve a entrada. `invalido`: `setValor` lanca
`std::invalid_argument` e o valor anterior fica. Dominio com `int`: `TesteDominio<Limite, int> limite(5);`.
Bytes especiais: `std::string("ab") + '\0' + "12"`; depois de `\xHH` feche a string: `"\xE9" "b12"`.

## Adicionar teste de entidade ou servico

Mesmo arquivo `testes/teste_xxx.cpp`, usando `verificar(condicao, "descricao")` e `return resultado();`.
Modelos: `teste_pessoa.cpp` (entidade) e `teste_servicopessoa.cpp` (servico: conteineres locais no
`main`, servico usado pela interface).

## Adicionar teste de tela (roteiro)

Crie `testes/roteiros/NN_nome.txt` (o que o usuario digita, uma resposta por linha) e
`testes/roteiros/NN_nome.esperado`:

```
# comentario
> Conta criada com sucesso      (deve aparecer, nesta ordem; maiusculas tanto faz)
> Sistema encerrado
! Pw123                         (nao pode aparecer)
@apenas_teste                   (opcional: so roda no teste.exe, que ja tem PRJ001 e CRT001)
```

Cada roteiro roda no `kanban.exe` (`main.cpp`) e no `teste.exe` (`testes/main_teste.cpp`), com limite
de 5 s e 1 MB de saida (pega laco infinito). Ao plugar Projeto/Quadro/Cartao no `main.cpp`, faca o
mesmo em `testes/main_teste.cpp`. Espaco no fim de linha e arquivo sem quebra final sao de proposito
(o `.gitattributes` impede o git de mexer nos `.txt` dos roteiros).

## Stubs (`testes/stubs/`)

`identificador`, `projeto`, `quadro` e `cartaoatividade` existem so para o servico e as telas de
Pessoa compilarem enquanto as classes reais nao existem. Quando `headers/X.hpp` passa a ter
`class X`, o rodar **desliga o stub sozinho**, usa a classe real (e `sources/X.cpp`) e avisa:
`AVISO stub desligado ... Apague testes/stubs/X.hpp`. Entao apague o arquivo (o rodar recria
`testes/bin/stubs` a cada rodada, entao nenhuma copia velha esconde a classe real). Nao ha stub de
`estado`, `texto` nem `timestamp`: ninguem os usa ainda; quando forem usados, precisam ser reais.
