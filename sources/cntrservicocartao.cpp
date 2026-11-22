#include "cntrservicocartao.hpp"

#include <string>

namespace {

const std::string A_FAZER = "A FAZER";
const std::string FAZENDO = "FAZENDO";
const std::string FEITO = "FEITO";

}  // namespace

// --- Tempo ---

Timestamp CntrServicoCartao::agora() const {
    // Nos testes, o relogio simulado; no sistema, a data e a hora do computador.
    if (this->relogio != nullptr) {
        return this->relogio();
    }
    return Timestamp::agora();
}

// --- Auxiliares ---

bool CntrServicoCartao::usuarioCadastrado(const Email& usuario) const {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    return this->containerPessoa->pesquisar(usuario, pessoa);
}

bool CntrServicoCartao::temPapel(const Email& email, const std::string& papel) const {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    if (!this->containerPessoa->pesquisar(email, pessoa)) {
        return false;
    }
    return pessoa.getPapel().getValor() == papel;
}

bool CntrServicoCartao::gestorDoQuadro(const Email& usuario, const Identificador& quadro) const {
    if (this->containerQuadro == nullptr || this->containerProjeto == nullptr || !temPapel(usuario, "GESTOR")) {
        return false;
    }
    // Quadro -> projeto -> gestor (quem criou o projeto).
    Identificador projeto;
    Email gestor;
    if (!this->containerQuadro->pesquisarProjeto(quadro, projeto) ||
        !this->containerProjeto->pesquisarGestor(projeto, gestor)) {
        return false;
    }
    return gestor.getValor() == usuario.getValor();
}

bool CntrServicoCartao::gestorDoCartao(const Email& usuario, const Identificador& cartao) const {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Identificador quadro;
    if (!this->containerCartao->getQuadro(cartao, quadro)) {
        return false;
    }
    return gestorDoQuadro(usuario, quadro);
}

bool CntrServicoCartao::lerQuadro(const Email& usuario, const Identificador& identificador, Quadro& quadro) {
    if (this->cntrServicoQuadro == nullptr) {
        return false;
    }
    return this->cntrServicoQuadro->ler(usuario, identificador, quadro);
}

int CntrServicoCartao::contarFazendo(const Identificador& quadro) const {
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);
    int quantidade = 0;
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        if (cartoes[i].getEstado().getValor() == FAZENDO) {
            quantidade++;
        }
    }
    return quantidade;
}

bool CntrServicoCartao::cartaoCompleto(const CartaoAtividade& cartao) {
    return !cartao.getIdentificador().getValor().empty() && !cartao.getNome().getValor().empty() &&
           !cartao.getDescricao().getValor().empty() && !cartao.getPrioridade().getValor().empty() &&
           !cartao.getTamanho().getValor().empty();
}

// --- Servicos 16 a 20 ---

bool CntrServicoCartao::criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor,
                              const CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!gestorDoQuadro(usuario, quadro)) {
        return false;
    }
    if (!temPapel(desenvolvedor, "DESENVOLVEDOR")) {
        return false;
    }
    if (!cartaoCompleto(cartao)) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }

    // Estado e instantes vindos em cartao sao ignorados: todo cartao novo entra em A FAZER agora.
    CartaoAtividade novo;
    novo.setIdentificador(cartao.getIdentificador());
    novo.setNome(cartao.getNome());
    novo.setDescricao(cartao.getDescricao());
    novo.setPrioridade(cartao.getPrioridade());
    novo.setTamanho(cartao.getTamanho());
    Estado estado;
    estado.setValor(A_FAZER);
    novo.setEstado(estado);
    novo.setEntrada(agora());

    // O conteiner ja recusa identificador repetido.
    return this->containerCartao->incluir(novo, quadro, desenvolvedor);
}

bool CntrServicoCartao::ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    return this->containerCartao->pesquisar(identificador, cartao);
}

bool CntrServicoCartao::atualizar(const Email& usuario, const CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    // Tambem recusa cartao inexistente (sem quadro).
    if (!gestorDoCartao(usuario, cartao.getIdentificador())) {
        return false;
    }
    if (!cartaoCompleto(cartao)) {
        return false;
    }
    CartaoAtividade atual;
    if (!this->containerCartao->pesquisar(cartao.getIdentificador(), atual)) {
        return false;
    }
    // Chave, estado e instantes ficam como estao; so mudam os dados editaveis.
    atual.setNome(cartao.getNome());
    atual.setDescricao(cartao.getDescricao());
    atual.setPrioridade(cartao.getPrioridade());
    atual.setTamanho(cartao.getTamanho());
    return this->containerCartao->atualizar(atual);
}

bool CntrServicoCartao::excluir(const Email& usuario, const Identificador& identificador) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!gestorDoCartao(usuario, identificador)) {
        return false;
    }
    // O conteiner ja recusa cartao inexistente.
    return this->containerCartao->remover(identificador);
}

bool CntrServicoCartao::listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);
    lista.clear();
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        lista.push_back(cartoes[i].getIdentificador());
    }
    return true;
}

// --- Servicos 21 a 26 ---

bool CntrServicoCartao::mover(const Email& usuario, const Identificador& identificador) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao)) {
        return false;
    }

    std::string estadoAtual = cartao.getEstado().getValor();
    Estado novoEstado;
    if (estadoAtual == A_FAZER) {
        // Entrar em FAZENDO nao pode estourar o limite de WIP do quadro.
        Identificador idQuadro;
        Quadro quadro;
        if (!this->containerCartao->getQuadro(identificador, idQuadro) || !lerQuadro(usuario, idQuadro, quadro)) {
            return false;
        }
        if (contarFazendo(idQuadro) >= quadro.getLimite().getValor()) {
            return false;
        }
        novoEstado.setValor(FAZENDO);
        cartao.setInicio(agora());
    } else if (estadoAtual == FAZENDO) {
        novoEstado.setValor(FEITO);
        cartao.setTermino(agora());
    } else {
        return false;  // Ja esta em FEITO.
    }
    cartao.setEstado(novoEstado);
    return this->containerCartao->atualizar(cartao);
}

bool CntrServicoCartao::lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao) || cartao.getEstado().getValor() != FEITO) {
        return false;
    }
    minutos = static_cast<double>(cartao.getTermino().getMinutos() - cartao.getInicio().getMinutos());
    return true;
}

bool CntrServicoCartao::lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao) || cartao.getEstado().getValor() != FEITO) {
        return false;
    }
    minutos = static_cast<double>(cartao.getTermino().getMinutos() - cartao.getEntrada().getMinutos());
    return true;
}

bool CntrServicoCartao::calcularMedia(const Email& usuario, const Identificador& quadro, bool ciclo, double& minutos) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);

    long soma = 0;
    int concluidos = 0;
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        if (cartoes[i].getEstado().getValor() != FEITO) {
            continue;
        }
        long comeco = (ciclo ? cartoes[i].getInicio() : cartoes[i].getEntrada()).getMinutos();
        soma += cartoes[i].getTermino().getMinutos() - comeco;
        concluidos++;
    }
    if (concluidos == 0) {
        return false;
    }
    minutos = static_cast<double>(soma) / concluidos;
    return true;
}

bool CntrServicoCartao::lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos) {
    return calcularMedia(usuario, quadro, true, minutos);
}

bool CntrServicoCartao::lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos) {
    return calcularMedia(usuario, quadro, false, minutos);
}

bool CntrServicoCartao::lerWIP(const Email& usuario, const Identificador& quadro, int& wip) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    wip = contarFazendo(quadro);
    return true;
}
