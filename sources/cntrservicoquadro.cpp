#include "cntrservicoquadro.hpp"

#include "estado.hpp"

void CntrServicoQuadro::setContainerPessoa(ContainerPessoa* containerPessoa) {
    this->containerPessoa = containerPessoa;
}

void CntrServicoQuadro::setContainerProjeto(ContainerProjeto* containerProjeto) {
    this->containerProjeto = containerProjeto;
}

void CntrServicoQuadro::setContainerQuadro(ContainerQuadro* containerQuadro) {
    this->containerQuadro = containerQuadro;
}

void CntrServicoQuadro::setContainerCartao(ContainerCartao* containerCartao) {
    this->containerCartao = containerCartao;
}

bool CntrServicoQuadro::usuarioCadastrado(const Email& usuario) const {
    if (this->containerPessoa == nullptr || this->containerQuadro == nullptr) {
        return false;
    }
    Pessoa pessoa;
    return this->containerPessoa->pesquisar(usuario, pessoa);
}

bool CntrServicoQuadro::usuarioGestor(const Email& usuario) const {
    if (this->containerPessoa == nullptr || this->containerQuadro == nullptr) {
        return false;
    }
    Pessoa pessoa;
    if (!this->containerPessoa->pesquisar(usuario, pessoa)) {
        return false;
    }
    return pessoa.getPapel().getValor() == "GESTOR";
}

int CntrServicoQuadro::contarFazendo(const Identificador& quadro) const {
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);
    int quantidade = 0;
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        if (cartoes[i].getEstado().getValor() == Estado::FAZENDO) {
            quantidade++;
        }
    }
    return quantidade;
}

bool CntrServicoQuadro::quadroCompleto(const Quadro& quadro) {
    return !quadro.getIdentificador().getValor().empty() &&
           !quadro.getNome().getValor().empty() &&
           quadro.getLimite().getValor() != 0;
}

bool CntrServicoQuadro::criar(const Email& usuario, const Identificador& projeto, const Quadro& quadro) {
    if (!this->usuarioGestor(usuario)) {
        return false;
    }
    if (this->containerProjeto == nullptr) {
        return false;
    }
    Projeto lido;
    if (!this->containerProjeto->pesquisar(projeto, lido)) {
        return false;
    }
    if (!quadroCompleto(quadro)) {
        return false;
    }
    // O conteiner ja recusa identificador repetido.
    return this->containerQuadro->incluir(quadro, projeto);
}

bool CntrServicoQuadro::ler(const Email& usuario, const Identificador& identificador, Quadro& quadro) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    return this->containerQuadro->pesquisar(identificador, quadro);
}

bool CntrServicoQuadro::atualizar(const Email& usuario, const Quadro& quadro) {
    if (!this->usuarioGestor(usuario)) {
        return false;
    }
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!quadroCompleto(quadro)) {
        return false;
    }
    // O novo limite nao pode deixar mais cartoes em FAZENDO do que o permitido.
    if (quadro.getLimite().getValor() < this->contarFazendo(quadro.getIdentificador())) {
        return false;
    }
    // O conteiner ja recusa quadro inexistente e mantem o projeto.
    return this->containerQuadro->atualizar(quadro);
}

bool CntrServicoQuadro::excluir(const Email& usuario, const Identificador& identificador) {
    if (!this->usuarioGestor(usuario)) {
        return false;
    }
    if (this->containerCartao == nullptr) {
        return false;
    }
    // Sem cascata: recusa se o quadro tiver cartoes.
    if (this->containerCartao->existeQuadro(identificador)) {
        return false;
    }
    // O conteiner ja recusa quadro inexistente.
    return this->containerQuadro->remover(identificador);
}

bool CntrServicoQuadro::listar(const Email& usuario, const Identificador& projeto, std::vector<Identificador>& lista) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    if (this->containerProjeto == nullptr) {
        return false;
    }
    Projeto lido;
    if (!this->containerProjeto->pesquisar(projeto, lido)) {
        return false;
    }
    this->containerQuadro->listar(projeto, lista);
    return true;
}
