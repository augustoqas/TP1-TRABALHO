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
1. compila cada `sources/*.cpp` numa biblioteca (quem não compila vira `AVISO` e fica de fora);
2. compila e roda cada `testes/teste_*.cpp` com essa biblioteca;
3. compila `testes/main_teste.cpp` (teste.exe) e `main.cpp` (kanban.exe) e confere os roteiros;
4. **build real**: compila `main.cpp` + todos os `sources/*.cpp` só com `-Iheaders`, como o
   Code::Blocks faria (só na rodada completa, sem filtro); se não compilar, é `FAIL` (o erro
   completo fica em `testes/bin/build_real.txt`).

Não existe lista de testes nem de fontes para editar: **criou `testes/teste_xxx.cpp`, ele roda**.

### Como ler a última linha

| Linha final | Significado |
|---|---|
| `TUDO PASSOU` | testes passaram e o build real compila |
| `TUDO PASSOU (ha AVISOS acima)` | idem, mas leia os `AVISO`s (ex.: `main_teste.cpp` desatualizado) |
| `testes passaram, MAS ha fontes em sources/ que nao compilam (ver AVISOS)` | rodada com filtro e algum `sources/*.cpp` quebrado |
| `HA FALHAS (procure FAIL acima)` | algum teste falhou (código de saída 1) |

## Adicionar teste de domínio

`TesteDominio` pressupõe a **convenção do projeto**, seguida por todos os domínios: `void
setValor(...)` lança `std::invalid_argument` para valor inválido e `getValor()` é `const`.

Modelo (`testes/teste_identificador.cpp`, copie e troque os nomes):

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
`std::invalid_argument` e o valor anterior fica. Domínio com `int`:
`TesteDominio<Limite, int> limite(5);`. Bytes especiais: `std::string("ab") + '\0' + "12"`; depois
de `\xHH` feche a string: `"\xE9" "b12"`.

## Adicionar teste de entidade ou serviço

Mesmo arquivo `testes/teste_xxx.cpp`, usando `verificar(condicao, "descricao")` e `return resultado();`.
Modelos: `teste_pessoa.cpp` (entidade) e `teste_servicopessoa.cpp` (serviço: contêineres locais no
`main`, serviço usado pela interface).

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
diferença. Ao mudar o `main.cpp`, copie a mudança para o `main_teste.cpp`.

Os roteiros criam projetos, quadros e cartões pelo menu; hoje só o `06_vinculada` usa
`@apenas_teste`. Sem `main_teste.cpp`, o rodar pula o `teste.exe` (e um roteiro que ainda tenha
`@apenas_teste` vira `FAIL`, para a marca não ficar esquecida).

### Demonstração para o vídeo

O teste fumaça da entrega é o roteiro `91_fumaca`: um caso de sucesso para cada funcionalidade
(criar conta, autenticar e os serviços 1 a 26), com o guia passo a passo do vídeo em
[`docs/teste_fumaca.md`](../docs/teste_fumaca.md). O `90_demo` é uma demonstração curta.

Depois de uma rodada do rodar, dá para repetir um roteiro à mão (da raiz do repositório, no cmd):

```
testes\bin\kanban.exe < testes\roteiros\90_demo.txt
```

A saída da última rodada automática está em `testes/bin/roteiros/kanban_90_demo.txt`. Para ver o
programa interativo, rode `testes\bin\kanban.exe` sem o `<`.

## Stubs

Recurso para classes que ainda não existem: um `testes/stubs/X.hpp` entra no lugar de `headers/X.hpp`
enquanto este estiver ausente ou em branco (hoje não há nenhum stub; ver comentário em `testes/rodar.cpp`).
