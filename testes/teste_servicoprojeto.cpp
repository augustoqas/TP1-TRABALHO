// Testes da camada de servico de Projeto (issue #16).
// Conversa com o servico so pela interface; pessoas e quadros sao incluidos direto
// nos conteineres para simular cadastro e vinculos.
#include <string>
#include <vector>

#include "cntrservicoprojeto.hpp"
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

static Projeto projeto(const std::string& id, const std::string& n, const std::string& d,
                       const std::string& inicio, const std::string& termino) {
    Nome nome;
    nome.setValor(n);
    Texto descricao;
    descricao.setValor(d);
    Timestamp tInicio;
    tInicio.setValor(inicio);
    Timestamp tTermino;
    tTermino.setValor(termino);
    Projeto projeto;
    projeto.setIdentificador(identificador(id));
    projeto.setNome(nome);
    projeto.setDescricao(descricao);
    projeto.setInicio(tInicio);
    projeto.setTermino(tTermino);
    return projeto;
}

static Projeto projetoPadrao(const std::string& id) {
    return projeto(id, "Kanban", "Trabalho de TP1.", "01-MAR-2026-08:00", "30-JUN-2026-18:00");
}

static std::string resumo(const Projeto& p) {
    return p.getIdentificador().getValor() + "|" + p.getNome().getValor() + "|" +
           p.getDescricao().getValor() + "|" + p.getInicio().getValor() + "|" +
           p.getTermino().getValor();
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

    pessoas.incluir(pessoa("gestor@unb.br", "GESTOR"));
    pessoas.incluir(pessoa("outro.gestor@unb.br", "GESTOR"));
    pessoas.incluir(pessoa("dev@unb.br", "DESENVOLVEDOR"));
    Email gestor = email("gestor@unb.br");
    Email outroGestor = email("outro.gestor@unb.br");
    Email dev = email("dev@unb.br");
    Email estranho = email("ninguem@unb.br");

    CntrServicoProjeto cntr;
    IServicoProjeto* servico = &cntr;

    // Sem conteineres: tudo recusado, sem quebrar.
    Projeto lido;
    std::vector<Identificador> lista;
    verificar(!servico->criar(gestor, projetoPadrao("PRJ001")), "sem conteineres: criar recusado");
    verificar(!servico->ler(gestor, identificador("PRJ001"), lido), "sem conteineres: ler recusado");
    verificar(!servico->listar(gestor, lista), "sem conteineres: listar recusado");

    cntr.setContainerPessoa(&pessoas);
    cntr.setContainerProjeto(&projetos);

    // 6. Criar.
    verificar(servico->criar(gestor, projetoPadrao("PRJ001")), "gestor cria projeto");
    verificar(projetos.existeGestor(gestor), "quem criou vira o gestor");
    verificar(!servico->criar(gestor, projetoPadrao("PRJ001")), "identificador repetido recusado");
    verificar(!servico->criar(dev, projetoPadrao("PRJ002")), "desenvolvedor nao cria projeto");
    verificar(!servico->criar(estranho, projetoPadrao("PRJ002")), "usuario nao cadastrado nao cria projeto");
    Projeto incompleto;
    incompleto.setIdentificador(identificador("PRJ003"));
    verificar(!servico->criar(gestor, incompleto), "projeto incompleto recusado");
    verificar(!servico->criar(gestor, Projeto()), "projeto vazio recusado");
    verificar(servico->criar(outroGestor, projetoPadrao("PRJ002")), "outro gestor cria o seu projeto");

    // 7. Ler.
    verificar(servico->ler(dev, identificador("PRJ001"), lido), "desenvolvedor le projeto");
    verificar(resumo(lido) == resumo(projetoPadrao("PRJ001")), "ler devolve todos os dados");
    Projeto intacto = projetoPadrao("XYZ999");
    verificar(!servico->ler(gestor, identificador("PRJ999"), intacto), "ler projeto inexistente recusado");
    verificar(intacto.getIdentificador().getValor() == "XYZ999", "saida nao muda quando ler falha");
    verificar(!servico->ler(estranho, identificador("PRJ001"), lido), "usuario nao cadastrado nao le");

    // 10. Listar.
    lista.push_back(identificador("LIX000"));
    verificar(servico->listar(dev, lista), "desenvolvedor lista projetos");
    verificar(lista.size() == 2 && contem(lista, "PRJ001") && contem(lista, "PRJ002"),
              "listar devolve so os identificadores cadastrados");
    verificar(!servico->listar(estranho, lista), "usuario nao cadastrado nao lista");

    // 8. Atualizar.
    Projeto novo = projeto("PRJ001", "Kanban Novo", "Versao dois.", "02-MAR-2026-09:00", "31-JUL-2026-17:00");
    verificar(!servico->atualizar(dev, novo), "desenvolvedor nao atualiza");
    verificar(servico->ler(gestor, identificador("PRJ001"), lido) && resumo(lido) == resumo(projetoPadrao("PRJ001")),
              "atualizacao recusada nao muda nada");
    verificar(servico->atualizar(gestor, novo), "gestor atualiza");
    verificar(servico->ler(gestor, identificador("PRJ001"), lido) && resumo(lido) == resumo(novo),
              "atualizar troca nome, descricao, inicio e termino");
    verificar(!servico->atualizar(gestor, projetoPadrao("PRJ999")), "atualizar projeto inexistente recusado");
    verificar(servico->listar(gestor, lista) && lista.size() == 2, "atualizar inexistente nao cria projeto");
    Projeto semNome = projetoPadrao("PRJ001");
    semNome.setNome(Nome());
    verificar(!servico->atualizar(gestor, semNome), "atualizar com projeto incompleto recusado");

    // 9. Excluir.
    verificar(!servico->excluir(gestor, identificador("PRJ001")), "sem conteiner de quadros: excluir recusado");
    cntr.setContainerQuadro(&quadros);
    Quadro quadro;
    quadro.setIdentificador(identificador("QDR001"));
    quadros.incluir(quadro, identificador("PRJ001"));
    verificar(!servico->excluir(gestor, identificador("PRJ001")), "projeto com quadro nao e excluido");
    verificar(servico->ler(gestor, identificador("PRJ001"), lido), "projeto recusado continua existindo");
    verificar(!servico->excluir(dev, identificador("PRJ002")), "desenvolvedor nao exclui");
    verificar(!servico->excluir(gestor, identificador("PRJ999")), "excluir projeto inexistente recusado");
    quadros.remover(identificador("QDR001"));
    verificar(servico->excluir(gestor, identificador("PRJ001")), "projeto sem quadros e excluido");
    verificar(!servico->ler(gestor, identificador("PRJ001"), lido), "projeto excluido nao existe mais");
    verificar(!projetos.existeGestor(gestor), "excluir remove o vinculo de gestor");
    verificar(servico->listar(gestor, lista) && lista.size() == 1 && contem(lista, "PRJ002"),
              "listar depois de excluir");

    return resultado();
}
