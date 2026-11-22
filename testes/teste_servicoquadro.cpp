// Testes da camada de servico de Quadro (issue #17).
// Conversa com o servico so pela interface; pessoas, projetos e cartoes sao incluidos
// direto nos conteineres para simular cadastro e vinculos. Dois gestores: G1 (gestor)
// e gestor de PRJ001 e PRJ002, G2 (outroGestor) de PRJ003; so o gestor do projeto
// cria, atualiza e exclui os quadros dele.
#include <string>
#include <vector>

#include "cntrservicoquadro.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "containerquadro.hpp"
#include "teste.hpp"

static Identificador identificador(const std::string& valor) {
    Identificador id;
    id.setValor(valor);
    return id;
}

static Email email(const std::string& valor) {
    Email e;
    e.setValor(valor);
    return e;
}

static Pessoa pessoa(const std::string& e, const std::string& p) {
    Nome nome;
    nome.setValor("Fulano");
    Senha senha;
    senha.setValor("Abc12");
    Papel papel;
    papel.setValor(p);
    Pessoa pessoa;
    pessoa.setEmail(email(e));
    pessoa.setNome(nome);
    pessoa.setSenha(senha);
    pessoa.setPapel(papel);
    return pessoa;
}

static Quadro quadro(const std::string& id, const std::string& n, int l) {
    Nome nome;
    nome.setValor(n);
    Limite limite;
    limite.setValor(l);
    Quadro quadro;
    quadro.setIdentificador(identificador(id));
    quadro.setNome(nome);
    quadro.setLimite(limite);
    return quadro;
}

static CartaoAtividade cartao(const std::string& id, const std::string& e) {
    Estado estado;
    estado.setValor(e);
    CartaoAtividade cartao;
    cartao.setIdentificador(identificador(id));
    cartao.setEstado(estado);
    return cartao;
}

static std::string resumo(const Quadro& q) {
    return q.getIdentificador().getValor() + "|" + q.getNome().getValor() + "|" +
           std::to_string(q.getLimite().getValor());
}

static bool contem(const std::vector<Identificador>& lista, const std::string& valor) {
    for (std::vector<Identificador>::size_type i = 0; i < lista.size(); i++) {
        if (lista[i].getValor() == valor) {
            return true;
        }
    }
    return false;
}

int main() {
    ContainerPessoa pessoas;
    ContainerProjeto projetos;
    ContainerQuadro quadros;
    ContainerCartao cartoes;

    pessoas.incluir(pessoa("gestor@unb.br", "GESTOR"));
    pessoas.incluir(pessoa("outro.gestor@unb.br", "GESTOR"));
    pessoas.incluir(pessoa("dev@unb.br", "DESENVOLVEDOR"));
    Email gestor = email("gestor@unb.br");
    Email outroGestor = email("outro.gestor@unb.br");
    Email dev = email("dev@unb.br");
    Email estranho = email("ninguem@unb.br");

    Projeto projeto;
    projeto.setIdentificador(identificador("PRJ001"));
    projetos.incluir(projeto, gestor);
    Projeto outroProjeto;
    outroProjeto.setIdentificador(identificador("PRJ002"));
    projetos.incluir(outroProjeto, gestor);
    Projeto projetoDoOutro;
    projetoDoOutro.setIdentificador(identificador("PRJ003"));
    projetos.incluir(projetoDoOutro, outroGestor);
    Identificador prj1 = identificador("PRJ001");
    Identificador prj2 = identificador("PRJ002");
    Identificador prj3 = identificador("PRJ003");
    Identificador prjInexistente = identificador("PRJ999");

    CntrServicoQuadro cntr;
    IServicoQuadro* servico = &cntr;

    // Sem conteineres: tudo recusado, sem quebrar.
    Quadro lido;
    std::vector<Identificador> lista;
    verificar(!servico->criar(gestor, prj1, quadro("QDR001", "Sprint um", 2)), "sem conteineres: criar recusado");
    verificar(!servico->ler(gestor, identificador("QDR001"), lido), "sem conteineres: ler recusado");
    verificar(!servico->listar(gestor, prj1, lista), "sem conteineres: listar recusado");

    cntr.setContainerPessoa(&pessoas);
    cntr.setContainerQuadro(&quadros);
    verificar(!servico->criar(gestor, prj1, quadro("QDR001", "Sprint um", 2)), "sem conteiner de projetos: criar recusado");
    cntr.setContainerProjeto(&projetos);

    // 11. Criar.
    verificar(servico->criar(gestor, prj1, quadro("QDR001", "Sprint um", 2)), "gestor cria quadro no projeto");
    verificar(!servico->criar(gestor, prj1, quadro("QDR001", "Outro", 3)), "identificador repetido recusado");
    verificar(!servico->criar(gestor, prj2, quadro("QDR001", "Outro", 3)), "mesmo quadro em outro projeto recusado");
    verificar(!servico->criar(gestor, prjInexistente, quadro("QDR002", "Sprint dois", 3)), "projeto inexistente recusado");
    verificar(!servico->criar(dev, prj1, quadro("QDR002", "Sprint dois", 3)), "desenvolvedor nao cria quadro");
    verificar(!servico->criar(estranho, prj1, quadro("QDR002", "Sprint dois", 3)), "usuario nao cadastrado nao cria");
    verificar(!servico->criar(outroGestor, prj1, quadro("QDR002", "Sprint dois", 3)),
              "outro gestor nao cria quadro em projeto que nao e dele");
    verificar(!servico->ler(gestor, identificador("QDR002"), lido), "criacao recusada nao cria o quadro");
    verificar(servico->criar(outroGestor, prj3, quadro("QDR030", "Do outro", 2)), "outro gestor cria quadro no seu projeto");
    Quadro semLimite;
    semLimite.setIdentificador(identificador("QDR003"));
    Nome nome;
    nome.setValor("Sem limite");
    semLimite.setNome(nome);
    verificar(!servico->criar(gestor, prj1, semLimite), "quadro sem limite recusado");
    verificar(!servico->criar(gestor, prj1, Quadro()), "quadro vazio recusado");
    verificar(servico->criar(gestor, prj1, quadro("QDR002", "Sprint dois", 3)), "segundo quadro no mesmo projeto");
    verificar(servico->criar(gestor, prj2, quadro("QDR010", "Outro projeto", 5)), "quadro em outro projeto");

    // Conteiner: projeto de cada quadro.
    Identificador projetoLido;
    verificar(quadros.pesquisarProjeto(identificador("QDR030"), projetoLido) && projetoLido.getValor() == "PRJ003",
              "conteiner: pesquisarProjeto devolve o projeto do quadro");
    Identificador projetoIntacto = identificador("XYZ999");
    verificar(!quadros.pesquisarProjeto(identificador("QDR999"), projetoIntacto) && projetoIntacto.getValor() == "XYZ999",
              "conteiner: quadro inexistente nao tem projeto (saida intacta)");

    // 12. Ler.
    verificar(servico->ler(outroGestor, identificador("QDR001"), lido), "outro gestor le quadro de projeto alheio");
    verificar(servico->ler(dev, identificador("QDR001"), lido), "desenvolvedor le quadro");
    verificar(resumo(lido) == "QDR001|Sprint um|2", "ler devolve todos os dados");
    Quadro intacto = quadro("XYZ999", "Intacto", 9);
    verificar(!servico->ler(gestor, identificador("QDR999"), intacto), "ler quadro inexistente recusado");
    verificar(resumo(intacto) == "XYZ999|Intacto|9", "saida nao muda quando ler falha");
    verificar(!servico->ler(estranho, identificador("QDR001"), lido), "usuario nao cadastrado nao le");

    // 15. Listar.
    lista.push_back(identificador("LIX000"));
    verificar(servico->listar(dev, prj1, lista), "desenvolvedor lista quadros do projeto");
    verificar(lista.size() == 2 && contem(lista, "QDR001") && contem(lista, "QDR002"),
              "listar devolve so os quadros do projeto");
    verificar(servico->listar(dev, prj2, lista) && lista.size() == 1 && contem(lista, "QDR010"),
              "listar de outro projeto");
    verificar(servico->listar(outroGestor, prj1, lista) && lista.size() == 2, "outro gestor lista quadros de projeto alheio");
    verificar(!servico->listar(gestor, prjInexistente, lista), "listar de projeto inexistente recusado");
    verificar(!servico->listar(estranho, prj1, lista), "usuario nao cadastrado nao lista");

    // 13. Atualizar.
    verificar(!servico->atualizar(gestor, quadro("QDR001", "Novo nome", 4)), "sem conteiner de cartoes: atualizar recusado");
    cntr.setContainerCartao(&cartoes);
    verificar(!servico->atualizar(dev, quadro("QDR001", "Novo nome", 4)), "desenvolvedor nao atualiza");
    verificar(!servico->atualizar(outroGestor, quadro("QDR001", "Novo nome", 4)),
              "outro gestor nao atualiza quadro de projeto que nao e dele");
    verificar(servico->ler(gestor, identificador("QDR001"), lido) && resumo(lido) == "QDR001|Sprint um|2",
              "atualizacao do outro gestor nao muda nada");
    verificar(servico->atualizar(outroGestor, quadro("QDR030", "Do outro novo", 3)), "outro gestor atualiza quadro do seu projeto");
    verificar(servico->atualizar(gestor, quadro("QDR001", "Novo nome", 4)), "gestor do projeto atualiza nome e limite");
    verificar(servico->ler(gestor, identificador("QDR001"), lido) && resumo(lido) == "QDR001|Novo nome|4",
              "atualizar troca nome e limite");
    verificar(!servico->atualizar(gestor, quadro("QDR999", "Nada", 4)), "atualizar quadro inexistente recusado");
    verificar(servico->listar(gestor, prj1, lista) && lista.size() == 2, "atualizar nao muda o projeto do quadro");

    cartoes.incluir(cartao("CRT001", "FAZENDO"), identificador("QDR001"), dev);
    cartoes.incluir(cartao("CRT002", "FAZENDO"), identificador("QDR001"), dev);
    cartoes.incluir(cartao("CRT003", "FAZENDO"), identificador("QDR001"), dev);
    cartoes.incluir(cartao("CRT004", "A FAZER"), identificador("QDR001"), dev);
    cartoes.incluir(cartao("CRT005", "FEITO"), identificador("QDR001"), dev);
    cartoes.incluir(cartao("CRT006", "FAZENDO"), identificador("QDR002"), dev);
    verificar(!servico->atualizar(gestor, quadro("QDR001", "Novo nome", 2)), "limite menor que o WIP (3) recusado");
    verificar(servico->ler(gestor, identificador("QDR001"), lido) && resumo(lido) == "QDR001|Novo nome|4",
              "atualizacao recusada nao muda nada");
    verificar(servico->atualizar(gestor, quadro("QDR001", "Novo nome", 3)), "limite igual ao WIP aceito");

    // 14. Excluir.
    verificar(!servico->excluir(gestor, identificador("QDR001")), "quadro com cartoes nao e excluido");
    verificar(servico->ler(gestor, identificador("QDR001"), lido), "quadro recusado continua existindo");
    verificar(!servico->excluir(dev, identificador("QDR010")), "desenvolvedor nao exclui");
    verificar(!servico->excluir(gestor, identificador("QDR999")), "excluir quadro inexistente recusado");
    verificar(!servico->excluir(outroGestor, identificador("QDR010")), "outro gestor nao exclui quadro de projeto que nao e dele");
    verificar(servico->ler(gestor, identificador("QDR010"), lido), "quadro continua depois da exclusao recusada");
    verificar(!servico->excluir(gestor, identificador("QDR030")), "gestor nao exclui quadro do projeto do outro gestor");
    verificar(servico->excluir(outroGestor, identificador("QDR030")), "outro gestor exclui quadro do seu projeto");
    verificar(servico->excluir(gestor, identificador("QDR010")), "quadro sem cartoes e excluido");
    verificar(!servico->ler(gestor, identificador("QDR010"), lido), "quadro excluido nao existe mais");
    verificar(servico->listar(gestor, prj2, lista) && lista.empty(), "projeto fica sem quadros");
    verificar(!quadros.existeQuadroNoProjeto(prj2), "excluir remove o vinculo com o projeto");

    return resultado();
}
