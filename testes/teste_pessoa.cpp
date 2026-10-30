// Testes da entidade Pessoa: guarda copias dos dominios e funciona via const Pessoa&.
#include "pessoa.hpp"
#include "teste.hpp"

// Usa a entidade somente por referencia constante (como a camada de servico).
static std::string resumo(const Pessoa& pessoa) {
    return pessoa.getEmail().getValor() + "|" + pessoa.getNome().getValor() + "|" +
           pessoa.getSenha().getValor() + "|" + pessoa.getPapel().getValor();
}

// Simula parametro de saida (Pessoa&), como em IServicoPessoa::ler.
static void preencher(Pessoa& saida, const Pessoa& origem) {
    saida = origem;
}

int main() {
    // 1. Construtor padrao.
    Pessoa pessoa;

    // 2. Monta cada dominio com valor valido.
    Email email;
    email.setValor("ana.silva@unb.br");
    Nome nome;
    nome.setValor("Ana Silva");
    Senha senha;
    senha.setValor("Abc12");
    Papel papel;
    papel.setValor("GESTOR");  // Papel atual retorna bool; o futuro, void.

    // 3. Setters da entidade.
    pessoa.setEmail(email);
    pessoa.setNome(nome);
    pessoa.setSenha(senha);
    pessoa.setPapel(papel);

    // 4. Getters da entidade.
    verificar(pessoa.getEmail().getValor() == "ana.silva@unb.br", "getEmail retorna o email armazenado");
    verificar(pessoa.getNome().getValor() == "Ana Silva", "getNome retorna o nome armazenado");
    verificar(pessoa.getSenha().getValor() == "Abc12", "getSenha retorna a senha armazenada");
    verificar(pessoa.getPapel().getValor() == "GESTOR", "getPapel retorna o papel armazenado");

    // 5. A entidade guarda copia: alterar o dominio original depois nao afeta a entidade.
    nome.setValor("Bruno");
    verificar(pessoa.getNome().getValor() == "Ana Silva", "entidade guarda copia do dominio");

    // 6. Copia da entidade (construtor de copia e atribuicao).
    Pessoa copia(pessoa);
    verificar(resumo(copia) == resumo(pessoa), "construtor de copia preserva os atributos");

    Pessoa atribuida;
    atribuida = pessoa;
    verificar(resumo(atribuida) == resumo(pessoa), "atribuicao preserva os atributos");

    Papel desenvolvedor;
    desenvolvedor.setValor("DESENVOLVEDOR");
    copia.setPapel(desenvolvedor);
    verificar(copia.getPapel().getValor() == "DESENVOLVEDOR", "copia pode ser alterada");
    verificar(pessoa.getPapel().getValor() == "GESTOR", "alterar a copia nao altera o original");

    // 7. Uso via const Pessoa& e via Pessoa& de saida.
    const Pessoa& referencia = pessoa;
    verificar(resumo(referencia) == "ana.silva@unb.br|Ana Silva|Abc12|GESTOR", "getters funcionam em const Pessoa&");

    Pessoa saida;
    preencher(saida, pessoa);
    verificar(resumo(saida) == resumo(pessoa), "Pessoa& de saida recebe os dados");

    // 8. Dominio invalido lanca excecao antes de chegar a entidade.
    bool lancou = false;
    try {
        Email invalido;
        invalido.setValor("Ana@unb.br");  // maiuscula na parte local
        pessoa.setEmail(invalido);
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    verificar(lancou, "Email invalido lanca std::invalid_argument");

    lancou = false;
    try {
        Nome invalido;
        invalido.setValor("Ana1");
        pessoa.setNome(invalido);
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    verificar(lancou, "Nome invalido lanca std::invalid_argument");

    lancou = false;
    try {
        Senha invalida;
        invalida.setValor("abcde");  // sem digito
        pessoa.setSenha(invalida);
    } catch (const std::invalid_argument&) {
        lancou = true;
    }
    verificar(lancou, "Senha invalida lanca std::invalid_argument");

    verificar(resumo(pessoa) == "ana.silva@unb.br|Ana Silva|Abc12|GESTOR", "entidade nao muda quando o dominio e invalido");

    return resultado();
}
