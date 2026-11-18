// Testes do conteiner e do servico de cartoes (servicos 16 a 26).
// O servico de quadros e simulado por ServicoQuadroFalso (so ler funciona), e o
// relogio e controlado pelo teste para as metricas terem tempos conhecidos.
// QDR001 e QDR002 sao do projeto PRJ001, cujo gestor e gestor@unb.br (G1); o outro
// GESTOR (G2, outro@unb.br) nao cria, atualiza nem exclui cartoes nesses quadros.
#include <string>
#include <vector>

#include "cntrservicocartao.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "containerquadro.hpp"
#include "teste.hpp"

// --- Relogio controlado ---

static std::string instanteAtual = "20-OUT-2026-09:00";

static Timestamp relogioDeTeste() {
    Timestamp t;
    t.setValor(instanteAtual);
    return t;
}

// --- Servico de quadros falso: QDR001 (limite 2) e QDR002 (limite 1) ---

class ServicoQuadroFalso : public IServicoQuadro {
    public:
        bool criar(const Email&, const Identificador&, const Quadro&) override { return false; }
        bool atualizar(const Email&, const Quadro&) override { return false; }
        bool excluir(const Email&, const Identificador&) override { return false; }
        bool listar(const Email&, const Identificador&, std::vector<Identificador>&) override { return false; }

        bool ler(const Email&, const Identificador& identificador, Quadro& quadro) override {
            int limite = 0;
            if (identificador.getValor() == "QDR001") {
                limite = 2;
            } else if (identificador.getValor() == "QDR002") {
                limite = 1;
            } else {
                return false;
            }
            Limite l;
            l.setValor(limite);
            quadro.setIdentificador(identificador);
            quadro.setLimite(l);
            return true;
        }
};

// --- Auxiliares ---

static Identificador id(const std::string& valor) {
    Identificador i;
    i.setValor(valor);
    return i;
}

static Email email(const std::string& valor) {
    Email e;
    e.setValor(valor);
    return e;
}

static Pessoa pessoa(const std::string& e, const std::string& papel) {
    Nome nome;
    nome.setValor("Fulano");
    Senha senha;
    senha.setValor("abc12");
    Papel p;
    p.setValor(papel);
    Pessoa pessoa;
    pessoa.setEmail(email(e));
    pessoa.setNome(nome);
    pessoa.setSenha(senha);
    pessoa.setPapel(p);
    return pessoa;
}

static CartaoAtividade cartao(const std::string& identificador, const std::string& nome = "Tela de login",
                              const std::string& prioridade = "ALTA") {
    CartaoAtividade c;
    c.setIdentificador(id(identificador));
    Nome n;
    n.setValor(nome);
    c.setNome(n);
    Texto t;
    t.setValor("Implementar autenticacao.");
    c.setDescricao(t);
    Prioridade p;
    p.setValor(prioridade);
    c.setPrioridade(p);
    Tamanho tm;
    tm.setValor("MEDIO");
    c.setTamanho(tm);
    return c;
}

static std::string estadoDe(IServicoCartao* servico, const Email& usuario, const std::string& identificador) {
    CartaoAtividade lido;
    if (!servico->ler(usuario, id(identificador), lido)) {
        return "(nao encontrado)";
    }
    return lido.getEstado().getValor();
}

// --- Conteiner ---

static void testarConteiner() {
    ContainerCartao container;
    verificar(container.incluir(cartao("CRT001"), id("QDR001"), email("dev@unb.br")), "conteiner: incluir");
    verificar(!container.incluir(cartao("CRT001"), id("QDR002"), email("outro@unb.br")), "conteiner: incluir repetido e recusado");

    Identificador quadro;
    Email desenvolvedor;
    verificar(container.getQuadro(id("CRT001"), quadro) && quadro.getValor() == "QDR001", "conteiner: guarda o quadro");
    verificar(container.getDesenvolvedor(id("CRT001"), desenvolvedor) && desenvolvedor.getValor() == "dev@unb.br",
              "conteiner: guarda o desenvolvedor (o repetido nao sobrescreveu)");
    verificar(container.existeQuadro(id("QDR001")) && !container.existeQuadro(id("QDR002")), "conteiner: existeQuadro");
    verificar(container.existeDesenvolvedor(email("dev@unb.br")), "conteiner: existeDesenvolvedor");

    container.incluir(cartao("CRT003"), id("QDR001"), email("dev@unb.br"));
    container.incluir(cartao("CRT002"), id("QDR002"), email("dev@unb.br"));
    std::vector<CartaoAtividade> lista;
    container.listar(id("QDR001"), lista);
    verificar(lista.size() == 2 && lista[0].getIdentificador().getValor() == "CRT001" &&
              lista[1].getIdentificador().getValor() == "CRT003", "conteiner: listar so os cartoes do quadro, em ordem");

    verificar(container.atualizar(cartao("CRT001", "Novo nome")), "conteiner: atualizar existente");
    CartaoAtividade lido;
    verificar(container.pesquisar(id("CRT001"), lido) && lido.getNome().getValor() == "Novo nome", "conteiner: pesquisar ve a atualizacao");
    verificar(!container.atualizar(cartao("CRT999")), "conteiner: atualizar inexistente e recusado");

    verificar(container.remover(id("CRT001")), "conteiner: remover");
    verificar(!container.pesquisar(id("CRT001"), lido), "conteiner: removido nao e mais encontrado");
    verificar(!container.getQuadro(id("CRT001"), quadro), "conteiner: remover apaga tambem o quadro do cartao");
    verificar(!container.remover(id("CRT001")), "conteiner: remover inexistente e recusado");
}

// --- Servico ---

static void testarServico() {
    ContainerPessoa pessoas;
    ContainerCartao cartoes;
    ServicoQuadroFalso quadros;
    pessoas.incluir(pessoa("gestor@unb.br", "GESTOR"));
    pessoas.incluir(pessoa("dev@unb.br", "DESENVOLVEDOR"));
    pessoas.incluir(pessoa("outro@unb.br", "GESTOR"));

    Email gestor = email("gestor@unb.br");
    Email dev = email("dev@unb.br");
    Email outro = email("outro@unb.br");
    Email estranho = email("ninguem@unb.br");

    // Associacoes quadro -> projeto -> gestor, usadas para saber quem e o gestor do projeto.
    ContainerProjeto containerProjeto;
    ContainerQuadro containerQuadro;
    Projeto projeto;
    projeto.setIdentificador(id("PRJ001"));
    containerProjeto.incluir(projeto, gestor);
    Quadro quadro1;
    quadro1.setIdentificador(id("QDR001"));
    containerQuadro.incluir(quadro1, id("PRJ001"));
    Quadro quadro2;
    quadro2.setIdentificador(id("QDR002"));
    containerQuadro.incluir(quadro2, id("PRJ001"));

    CntrServicoCartao cntr;
    cntr.setContainerCartao(&cartoes);
    cntr.setContainerPessoa(&pessoas);
    cntr.setContainerQuadro(&containerQuadro);
    cntr.setContainerProjeto(&containerProjeto);
    cntr.setCntrServicoQuadro(&quadros);
    cntr.setRelogio(relogioDeTeste);
    IServicoCartao* servico = &cntr;  // o teste usa o servico pela interface

    // Criar (16)
    instanteAtual = "20-OUT-2026-09:00";
    verificar(!servico->criar(dev, id("QDR001"), dev, cartao("CRT001")), "criar: DESENVOLVEDOR nao cria");
    verificar(!servico->criar(gestor, id("QDR999"), dev, cartao("CRT001")), "criar: quadro inexistente e recusado");
    verificar(!servico->criar(gestor, id("QDR001"), gestor, cartao("CRT001")), "criar: responsavel com papel GESTOR e recusado");
    verificar(!servico->criar(gestor, id("QDR001"), estranho, cartao("CRT001")), "criar: responsavel inexistente e recusado");
    CartaoAtividade incompleto;
    incompleto.setIdentificador(id("CRT009"));
    verificar(!servico->criar(gestor, id("QDR001"), dev, incompleto), "criar: cartao sem nome/descricao/prioridade/tamanho e recusado");

    CartaoAtividade comEstado = cartao("CRT001");
    Estado feito;
    feito.setValor("FEITO");
    comEstado.setEstado(feito);
    verificar(!servico->criar(outro, id("QDR001"), dev, comEstado), "criar: GESTOR que nao e o gestor do projeto nao cria");
    CartaoAtividade naoCriado;
    verificar(!cartoes.pesquisar(id("CRT001"), naoCriado), "criar recusado nao inclui o cartao");
    verificar(servico->criar(gestor, id("QDR001"), dev, comEstado), "criar: gestor do projeto cria o cartao");
    verificar(!servico->criar(gestor, id("QDR001"), dev, cartao("CRT001")), "criar: identificador repetido e recusado");

    CartaoAtividade lido;
    verificar(servico->ler(outro, id("CRT001"), lido), "ler: outro gestor le o cartao");
    verificar(servico->ler(dev, id("CRT001"), lido), "ler: desenvolvedor le o cartao");
    verificar(lido.getEstado().getValor() == "A FAZER", "criar: entra em A FAZER (estado vindo e ignorado)");
    verificar(lido.getEntrada().getValor() == "20-OUT-2026-09:00", "criar: entrada = agora");
    verificar(lido.getInicio().getValor().empty() && lido.getTermino().getValor().empty(), "criar: sem inicio nem termino");
    verificar(!servico->ler(estranho, id("CRT001"), lido), "ler: usuario nao cadastrado e recusado");
    verificar(!servico->ler(dev, id("CRT999"), lido), "ler: cartao inexistente retorna false");

    instanteAtual = "20-OUT-2026-10:00";
    verificar(servico->criar(gestor, id("QDR001"), dev, cartao("CRT002", "Relatorios", "MEDIA")), "criar: segundo cartao");
    instanteAtual = "20-OUT-2026-11:00";
    verificar(servico->criar(gestor, id("QDR001"), dev, cartao("CRT003", "Backup")), "criar: terceiro cartao");

    // Atualizar (18)
    verificar(!servico->atualizar(dev, cartao("CRT002", "Outro nome")), "atualizar: DESENVOLVEDOR nao atualiza");
    verificar(!servico->atualizar(gestor, cartao("CRT999")), "atualizar: cartao inexistente e recusado");
    verificar(!servico->atualizar(outro, cartao("CRT002", "Outro nome")), "atualizar: outro gestor nao atualiza");
    servico->ler(gestor, id("CRT002"), lido);
    verificar(lido.getNome().getValor() == "Relatorios" && lido.getPrioridade().getValor() == "MEDIA",
              "atualizar recusado nao muda nada");
    verificar(servico->atualizar(gestor, cartao("CRT002", "Relatorio final", "BAIXA")), "atualizar: gestor do projeto atualiza");
    servico->ler(gestor, id("CRT002"), lido);
    verificar(lido.getNome().getValor() == "Relatorio final" && lido.getPrioridade().getValor() == "BAIXA",
              "atualizar: muda nome e prioridade");
    verificar(lido.getEstado().getValor() == "A FAZER" && lido.getEntrada().getValor() == "20-OUT-2026-10:00",
              "atualizar: estado e entrada nao mudam");

    // Listar (20)
    std::vector<Identificador> lista;
    verificar(servico->listar(dev, id("QDR001"), lista) && lista.size() == 3, "listar: 3 cartoes no QDR001");
    verificar(servico->listar(dev, id("QDR002"), lista) && lista.empty(), "listar: quadro sem cartao devolve lista vazia");
    verificar(!servico->listar(dev, id("QDR999"), lista), "listar: quadro inexistente e recusado");
    verificar(servico->listar(outro, id("QDR001"), lista) && lista.size() == 3, "listar: outro gestor lista");

    // WIP (26) e mover (21)
    int wip = -1;
    verificar(servico->lerWIP(dev, id("QDR001"), wip) && wip == 0, "WIP: 0 antes de mover");
    verificar(!servico->lerWIP(dev, id("QDR999"), wip), "WIP: quadro inexistente e recusado");

    instanteAtual = "21-OUT-2026-10:30";
    verificar(servico->mover(dev, id("CRT001")), "mover: A FAZER -> FAZENDO");
    verificar(estadoDe(servico, dev, "CRT001") == "FAZENDO", "mover: cartao ficou em FAZENDO");
    instanteAtual = "21-OUT-2026-14:00";
    verificar(servico->mover(gestor, id("CRT002")), "mover: segundo cartao para FAZENDO");
    verificar(servico->lerWIP(dev, id("QDR001"), wip) && wip == 2, "WIP: 2 apos dois movimentos");
    verificar(!servico->mover(dev, id("CRT003")), "mover: limite de WIP 2 atingido, recusado");
    verificar(estadoDe(servico, dev, "CRT003") == "A FAZER", "mover recusado nao altera o cartao");
    verificar(!servico->mover(estranho, id("CRT001")), "mover: usuario nao cadastrado e recusado");
    verificar(!servico->mover(dev, id("CRT999")), "mover: cartao inexistente e recusado");

    double minutos = -1;
    verificar(!servico->lerTempoCiclo(dev, id("CRT001"), minutos), "ciclo: cartao em FAZENDO ainda nao tem tempo");
    verificar(!servico->lerTempoCicloMedio(dev, id("QDR001"), minutos), "ciclo medio: sem cartao em FEITO e recusado");

    instanteAtual = "22-OUT-2026-17:45";
    verificar(servico->mover(dev, id("CRT001")), "mover: FAZENDO -> FEITO");
    instanteAtual = "22-OUT-2026-12:00";
    verificar(servico->mover(dev, id("CRT002")), "mover: segundo cartao para FEITO");
    verificar(!servico->mover(dev, id("CRT001")), "mover: cartao em FEITO nao se move");
    verificar(servico->lerWIP(dev, id("QDR001"), wip) && wip == 0, "WIP: volta a 0");
    servico->ler(dev, id("CRT001"), lido);
    verificar(lido.getInicio().getValor() == "21-OUT-2026-10:30" && lido.getTermino().getValor() == "22-OUT-2026-17:45",
              "mover: grava inicio e termino");

    // Metricas (22 a 25). CRT001: entrada 20-OUT 09:00, inicio 21-OUT 10:30, termino 22-OUT 17:45.
    // CRT002: entrada 20-OUT 10:00, inicio 21-OUT 14:00, termino 22-OUT 12:00.
    verificar(servico->lerTempoCiclo(dev, id("CRT001"), minutos) && minutos == 1875, "ciclo CRT001 = 1875 min (31 h 15 min)");
    verificar(servico->lerLeadTime(dev, id("CRT001"), minutos) && minutos == 3405, "lead time CRT001 = 3405 min");
    verificar(servico->lerTempoCiclo(dev, id("CRT002"), minutos) && minutos == 1320, "ciclo CRT002 = 1320 min");
    verificar(servico->lerLeadTime(dev, id("CRT002"), minutos) && minutos == 3000, "lead time CRT002 = 3000 min");
    verificar(servico->lerTempoCicloMedio(dev, id("QDR001"), minutos) && minutos == 1597.5,
              "ciclo medio QDR001 = 1597.5 (CRT003 em A FAZER fica fora)");
    verificar(servico->lerLeadTimeMedio(dev, id("QDR001"), minutos) && minutos == 3202.5, "lead time medio QDR001 = 3202.5");
    verificar(servico->lerTempoCiclo(outro, id("CRT001"), minutos) && minutos == 1875, "outro gestor le tempo de ciclo");
    verificar(servico->lerTempoCicloMedio(outro, id("QDR001"), minutos) && minutos == 1597.5, "outro gestor le ciclo medio");
    verificar(servico->lerLeadTime(outro, id("CRT001"), minutos) && minutos == 3405, "outro gestor le lead time");
    verificar(servico->lerLeadTimeMedio(outro, id("QDR001"), minutos) && minutos == 3202.5, "outro gestor le lead time medio");
    verificar(servico->lerWIP(outro, id("QDR001"), wip) && wip == 0, "outro gestor le WIP");
    verificar(!servico->lerLeadTime(dev, id("CRT003"), minutos), "lead time: cartao em A FAZER e recusado");
    verificar(!servico->lerLeadTimeMedio(dev, id("QDR002"), minutos), "lead time medio: quadro sem FEITO e recusado");
    verificar(!servico->lerTempoCicloMedio(dev, id("QDR999"), minutos), "ciclo medio: quadro inexistente e recusado");

    // Limite 1 no QDR002.
    instanteAtual = "23-OUT-2026-08:00";
    servico->criar(gestor, id("QDR002"), dev, cartao("CRT004"));
    servico->criar(gestor, id("QDR002"), dev, cartao("CRT005"));
    verificar(servico->mover(outro, id("CRT004")) && !servico->mover(dev, id("CRT005")),
              "mover: outro gestor move; limite 1 deixa so um em FAZENDO");

    // Ano bissexto: 28-FEV-2028 23:00 -> 01-MAR-2028 01:00 = 26 h.
    instanteAtual = "28-FEV-2028-23:00";
    servico->criar(gestor, id("QDR002"), dev, cartao("CRT006"));
    servico->mover(dev, id("CRT004"));  // libera a vaga
    servico->mover(dev, id("CRT006"));
    instanteAtual = "01-MAR-2028-01:00";
    servico->mover(dev, id("CRT006"));
    verificar(servico->lerTempoCiclo(dev, id("CRT006"), minutos) && minutos == 26 * 60, "ciclo atravessa 29 de fevereiro");

    // Excluir (19)
    verificar(!servico->excluir(dev, id("CRT003")), "excluir: DESENVOLVEDOR nao exclui");
    verificar(!servico->excluir(outro, id("CRT003")), "excluir: outro gestor nao exclui");
    verificar(servico->ler(dev, id("CRT003"), lido), "excluir recusado mantem o cartao");
    verificar(servico->excluir(gestor, id("CRT003")), "excluir: gestor do projeto exclui");
    verificar(!servico->excluir(gestor, id("CRT003")), "excluir: cartao ja excluido e recusado");
    verificar(!cartoes.existeQuadro(id("QDR999")), "excluir: nao cria vinculo fantasma");

    // Sem servico de quadros: o que depende do quadro e recusado, o resto funciona.
    CntrServicoCartao semQuadro;
    semQuadro.setContainerCartao(&cartoes);
    semQuadro.setContainerPessoa(&pessoas);
    verificar(!semQuadro.criar(gestor, id("QDR001"), dev, cartao("CRT010")), "sem servico de quadros: criar e recusado");
    verificar(semQuadro.ler(dev, id("CRT001"), lido), "sem servico de quadros: ler funciona");

    // Sem os conteineres de quadros e projetos nao da para saber o gestor: alterar e recusado.
    CntrServicoCartao semGestor;
    semGestor.setContainerCartao(&cartoes);
    semGestor.setContainerPessoa(&pessoas);
    semGestor.setCntrServicoQuadro(&quadros);
    verificar(!semGestor.criar(gestor, id("QDR001"), dev, cartao("CRT010")), "sem conteineres de quadro e projeto: criar recusado");
    verificar(!semGestor.atualizar(gestor, cartao("CRT002", "Outro nome")), "sem conteineres de quadro e projeto: atualizar recusado");
    verificar(!semGestor.excluir(gestor, id("CRT002")), "sem conteineres de quadro e projeto: excluir recusado");
    verificar(semGestor.ler(dev, id("CRT002"), lido) && lido.getNome().getValor() == "Relatorio final",
              "sem conteineres de quadro e projeto: nada muda");
    verificar(semGestor.mover(dev, id("CRT005")), "sem conteineres de quadro e projeto: mover funciona");
}

int main() {
    testarConteiner();
    testarServico();
    return resultado();
}
