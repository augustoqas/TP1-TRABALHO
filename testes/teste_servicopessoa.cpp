// Testes da camada de servico de Pessoa e Autenticacao (issue #15 e revisao da main).
// Conversa com os servicos so pelas interfaces; projetos e cartoes sao incluidos
// direto nos conteineres para simular vinculos.
#include <string>
#include <vector>

#include "cntrservicoautenticacao.hpp"
#include "cntrservicopessoa.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "teste.hpp"

static ContainerProjeto* projetos = nullptr;
static ContainerCartao* cartoes = nullptr;

static Identificador identificador(const std::string& valor) {
    Identificador id;
    id.setValor(valor);
    return id;
}

static void cadastrarProjeto(const std::string& id, const Email& gestor) {
    Projeto projeto;
    projeto.setIdentificador(identificador(id));
    projetos->incluir(projeto, gestor);
}

static void removerProjeto(const std::string& id) {
    projetos->remover(identificador(id));
}

static void cadastrarCartao(const std::string& id, const Email& desenvolvedor) {
    CartaoAtividade cartao;
    cartao.setIdentificador(identificador(id));
    cartoes->incluir(cartao, identificador("QDR001"), desenvolvedor);
}

static void removerCartao(const std::string& id) {
    cartoes->remover(identificador(id));
}

static Email email(const std::string& valor) {
    Email e;
    e.setValor(valor);
    return e;
}

static Senha senha(const std::string& valor) {
    Senha s;
    s.setValor(valor);
    return s;
}

static Pessoa pessoa(const std::string& e, const std::string& n, const std::string& s, const std::string& p) {
    Nome nome;
    nome.setValor(n);
    Papel papel;
    papel.setValor(p);
    Pessoa pessoa;
    pessoa.setEmail(email(e));
    pessoa.setNome(nome);
    pessoa.setSenha(senha(s));
    pessoa.setPapel(papel);
    return pessoa;
}

static std::string papelDe(const Pessoa& p) {
    return p.getPapel().getValor();
}

static bool contem(const std::vector<Email>& lista, const std::string& valor) {
    for (std::vector<Email>::size_type i = 0; i < lista.size(); i++) {
        if (lista[i].getValor() == valor) {
            return true;
        }
    }
    return false;
}

// Pessoa possivelmente incompleta: campo vazio = dominio nao preenchido.
static Pessoa incompleta(const std::string& e, const std::string& n, const std::string& s, const std::string& p) {
    Pessoa pessoa;
    if (!e.empty()) {
        pessoa.setEmail(email(e));
    }
    if (!n.empty()) {
        Nome nome;
        nome.setValor(n);
        pessoa.setNome(nome);
    }
    if (!s.empty()) {
        pessoa.setSenha(senha(s));
    }
    // Papel invalido (ex.: "CHEFE") e recusado pelo dominio; a pessoa fica sem papel.
    Papel papel;
    try {
        papel.setValor(p);
    } catch (const std::invalid_argument&) {
    }
    pessoa.setPapel(papel);
    return pessoa;
}

// Servico de pessoa: criar, autenticar, ler, listar, atualizar e excluir, com vinculos.
static void testarServico() {
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerCartao containerCartao;
    projetos = &containerProjeto;
    cartoes = &containerCartao;
    CntrServicoAutenticacao cntrAutenticacao;
    cntrAutenticacao.setContainerPessoa(&containerPessoa);
    CntrServicoPessoa cntrPessoa;
    cntrPessoa.setContainerPessoa(&containerPessoa);
    cntrPessoa.setContainerProjeto(&containerProjeto);
    cntrPessoa.setContainerCartao(&containerCartao);
    IServicoAutenticacao* aut = &cntrAutenticacao;
    IServicoPessoa* svc = &cntrPessoa;

    const Email ana = email("ana@unb.br");          // GESTOR
    const Email bruno = email("bruno@unb.br");      // DESENVOLVEDOR
    const Email carla = email("carla@unb.br");      // DESENVOLVEDOR
    const Email fantasma = email("fantasma@unb.br"); // nunca cadastrado

    // ---------------- criar ----------------
    verificar(svc->criar(pessoa("ana@unb.br", "Ana", "Abc12", "GESTOR")), "criar: pessoa nova (ana) retorna true");
    verificar(svc->criar(pessoa("bruno@unb.br", "Bruno", "Xyz34", "DESENVOLVEDOR")), "criar: pessoa nova (bruno) retorna true");
    verificar(svc->criar(pessoa("carla@unb.br", "Carla", "Qwe56", "DESENVOLVEDOR")), "criar: pessoa nova (carla) retorna true");
    verificar(!svc->criar(pessoa("ana@unb.br", "Outra Ana", "Zzz99", "DESENVOLVEDOR")), "criar: email duplicado retorna false");
    {
        Pessoa lida;
        svc->ler(ana, ana, lida);
        verificar(lida.getNome().getValor() == "Ana" && papelDe(lida) == "GESTOR", "criar: duplicado nao sobrescreve a pessoa original");
    }

    // ---------------- autenticar ----------------
    verificar(aut->autenticar(ana, senha("Abc12")), "autenticar: email e senha corretos retorna true");
    verificar(!aut->autenticar(ana, senha("Xyz34")), "autenticar: senha errada retorna false");
    verificar(!aut->autenticar(fantasma, senha("Abc12")), "autenticar: email inexistente retorna false");

    // ---------------- ler ----------------
    {
        Pessoa lida;
        bool ok = svc->ler(ana, bruno, lida);
        verificar(ok, "ler: pessoa existente retorna true");
        verificar(lida.getEmail().getValor() == "bruno@unb.br" && lida.getNome().getValor() == "Bruno" &&
                  lida.getSenha().getValor() == "Xyz34" && papelDe(lida) == "DESENVOLVEDOR",
                  "ler: preenche todos os dados da pessoa");
    }
    {
        Pessoa lida = pessoa("sentinela@unb.br", "Sentinela", "Sen01", "GESTOR");
        bool ok = svc->ler(ana, fantasma, lida);
        verificar(!ok, "ler: pessoa inexistente retorna false");
        verificar(lida.getEmail().getValor() == "sentinela@unb.br", "ler: saida nao e alterada quando retorna false");
    }
    {
        Pessoa lida;
        verificar(!svc->ler(fantasma, ana, lida), "ler: usuario (sessao) inexistente retorna false");
    }
    {
        Pessoa lida;
        verificar(svc->ler(bruno, bruno, lida) && lida.getNome().getValor() == "Bruno", "ler: usuario le os proprios dados");
    }

    // ---------------- listar ----------------
    {
        std::vector<Email> lista;
        lista.push_back(fantasma); // lixo previo deve ser descartado
        bool ok = svc->listar(bruno, lista);
        verificar(ok, "listar: usuario cadastrado retorna true");
        verificar(lista.size() == 3 && contem(lista, "ana@unb.br") && contem(lista, "bruno@unb.br") && contem(lista, "carla@unb.br"),
                  "listar: devolve exatamente os 3 emails cadastrados");
    }
    {
        std::vector<Email> lista;
        lista.push_back(ana);
        verificar(!svc->listar(fantasma, lista), "listar: usuario inexistente retorna false");
        verificar(lista.size() == 1 && lista[0].getValor() == "ana@unb.br", "listar: saida nao e alterada quando retorna false");
    }

    // ---------------- atualizar ----------------
    verificar(svc->atualizar(ana, pessoa("carla@unb.br", "Carla Souza", "Nova1", "GESTOR")), "atualizar: pessoa existente (nome, senha, papel) retorna true");
    {
        Pessoa lida;
        svc->ler(ana, carla, lida);
        verificar(lida.getEmail().getValor() == "carla@unb.br" && lida.getNome().getValor() == "Carla Souza" &&
                  lida.getSenha().getValor() == "Nova1" && papelDe(lida) == "GESTOR",
                  "atualizar: novos dados persistidos e email (chave) mantido");
    }
    verificar(aut->autenticar(carla, senha("Nova1")) && !aut->autenticar(carla, senha("Qwe56")),
              "atualizar: autenticacao passa a usar a nova senha");
    verificar(!svc->atualizar(ana, pessoa("fantasma@unb.br", "Fantasma", "Boo12", "GESTOR")), "atualizar: pessoa inexistente retorna false");
    {
        std::vector<Email> lista;
        svc->listar(ana, lista);
        verificar(lista.size() == 3 && !contem(lista, "fantasma@unb.br"), "atualizar: inexistente nao cria a pessoa");
    }
    verificar(!svc->atualizar(fantasma, pessoa("bruno@unb.br", "Bruno", "Xyz34", "DESENVOLVEDOR")), "atualizar: usuario (sessao) inexistente retorna false");

    // Vinculos: ana e gestora de PRJ001; bruno e desenvolvedor de CRT001.
    cadastrarProjeto("PRJ001", ana);
    cadastrarCartao("CRT001", bruno);

    verificar(!svc->atualizar(ana, pessoa("ana@unb.br", "Ana Nova", "Abc12", "DESENVOLVEDOR")),
              "atualizar: gestor de projeto deixando de ser GESTOR retorna false");
    {
        Pessoa lida;
        svc->ler(ana, ana, lida);
        verificar(lida.getNome().getValor() == "Ana" && papelDe(lida) == "GESTOR", "atualizar: recusa nao altera nada (nome e papel mantidos)");
    }
    verificar(svc->atualizar(ana, pessoa("ana@unb.br", "Ana Lima", "Abc12", "GESTOR")),
              "atualizar: gestor de projeto mantendo GESTOR retorna true");
    verificar(!svc->atualizar(ana, pessoa("bruno@unb.br", "Bruno", "Xyz34", "GESTOR")),
              "atualizar: desenvolvedor de cartao deixando de ser DESENVOLVEDOR retorna false");
    verificar(svc->atualizar(bruno, pessoa("bruno@unb.br", "Bruno Melo", "Xyz34", "DESENVOLVEDOR")),
              "atualizar: desenvolvedor de cartao mantendo DESENVOLVEDOR retorna true");

    // ---------------- excluir ----------------
    verificar(!svc->excluir(bruno, ana), "excluir: gestor de projeto retorna false");
    verificar(!svc->excluir(ana, bruno), "excluir: desenvolvedor de cartao retorna false");
    verificar(!svc->excluir(ana, fantasma), "excluir: pessoa inexistente retorna false");
    verificar(!svc->excluir(fantasma, carla), "excluir: usuario (sessao) inexistente retorna false");
    {
        std::vector<Email> lista;
        svc->listar(ana, lista);
        verificar(lista.size() == 3, "excluir: recusas nao removem ninguem");
    }
    verificar(svc->excluir(ana, carla), "excluir: pessoa sem vinculos retorna true");
    {
        Pessoa lida;
        std::vector<Email> lista;
        svc->listar(ana, lista);
        verificar(!svc->ler(ana, carla, lida) && !aut->autenticar(carla, senha("Nova1")) && lista.size() == 2 && !contem(lista, "carla@unb.br"),
                  "excluir: pessoa some de ler, autenticar e listar");
    }

    // Desfaz os vinculos: as recusas deixam de valer (consulta e feita na hora).
    removerProjeto("PRJ001");
    removerCartao("CRT001");
    verificar(svc->atualizar(bruno, pessoa("bruno@unb.br", "Bruno Melo", "Xyz34", "GESTOR")),
              "atualizar: troca de papel liberada apos remover o cartao");
    verificar(svc->excluir(ana, ana), "excluir: usuario exclui a si mesmo (ex-gestor, sem projeto) retorna true");
    verificar(!aut->autenticar(ana, senha("Abc12")), "excluir: usuario excluido nao autentica mais");
    verificar(svc->criar(pessoa("ana@unb.br", "Ana", "Abc12", "DESENVOLVEDOR")), "criar: email excluido pode ser recadastrado");
    {
        std::vector<Email> lista;
        verificar(svc->listar(ana, lista) && lista.size() == 2, "listar: reflete exclusoes e recadastro");
    }
}

// Revisao da main: o servico recusa Pessoa incompleta ou com papel invalido.
static void testarPessoaIncompleta() {
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerCartao containerCartao;
    CntrServicoPessoa cntrPessoa;
    cntrPessoa.setContainerPessoa(&containerPessoa);
    cntrPessoa.setContainerProjeto(&containerProjeto);
    cntrPessoa.setContainerCartao(&containerCartao);
    CntrServicoAutenticacao cntrAutenticacao;
    cntrAutenticacao.setContainerPessoa(&containerPessoa);
    IServicoPessoa* servico = &cntrPessoa;
    IServicoAutenticacao* aut = &cntrAutenticacao;

    verificar(!servico->criar(Pessoa()), "criar Pessoa() vazia e recusado");
    verificar(!servico->criar(incompleta("", "Ana", "abc12", "GESTOR")), "criar sem email e recusado");
    verificar(!servico->criar(incompleta("ana@unb.br", "Ana", "", "GESTOR")), "criar sem senha e recusado");
    verificar(!servico->criar(incompleta("ana@unb.br", "Ana", "abc12", "CHEFE")), "criar com papel invalido e recusado");
    verificar(!servico->criar(incompleta("ana@unb.br", "", "abc12", "GESTOR")), "criar sem nome e recusado");
    verificar(!aut->autenticar(Email(), Senha()), "autenticar com email e senha vazios e recusado");

    verificar(servico->criar(incompleta("ana@unb.br", "Ana", "abc12", "GESTOR")), "criar pessoa completa funciona");
    Email ana;
    ana.setValor("ana@unb.br");

    verificar(!servico->atualizar(ana, incompleta("ana@unb.br", "Ana", "abc12", "CHEFE")), "atualizar com papel invalido e recusado");
    verificar(!servico->atualizar(ana, incompleta("ana@unb.br", "Ana", "", "GESTOR")), "atualizar sem senha e recusado");
    verificar(!servico->atualizar(ana, incompleta("ana@unb.br", "", "abc12", "GESTOR")), "atualizar sem nome e recusado");

    Pessoa lida;
    servico->ler(ana, ana, lida);
    Papel papelLido = lida.getPapel();
    verificar(papelLido.getValor() == "GESTOR", "recusas nao alteraram o papel gravado");
    verificar(servico->atualizar(ana, incompleta("ana@unb.br", "Ana Maria", "xyz98", "DESENVOLVEDOR")), "atualizar completo funciona");
}

int main() {
    testarServico();
    testarPessoaIncompleta();
    return resultado();
}
