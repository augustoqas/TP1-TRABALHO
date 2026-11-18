#include "cntrservicoprojeto.hpp"

#include <string>

void CntrServicoProjeto::setContainerPessoa(ContainerPessoa* containerPessoa) {
    this->containerPessoa = containerPessoa;
}

void CntrServicoProjeto::setContainerProjeto(ContainerProjeto* containerProjeto) {
    this->containerProjeto = containerProjeto;
}

void CntrServicoProjeto::setContainerQuadro(ContainerQuadro* containerQuadro) {
    this->containerQuadro = containerQuadro;
}

bool CntrServicoProjeto::usuarioCadastrado(const Email& usuario) const {
    if (this->containerPessoa == nullptr || this->containerProjeto == nullptr) {
        return false;
    }
    Pessoa pessoa;
    return this->containerPessoa->pesquisar(usuario, pessoa);
}

bool CntrServicoProjeto::usuarioGestor(const Email& usuario) const {
    if (this->containerPessoa == nullptr || this->containerProjeto == nullptr) {
        return false;
    }
    Pessoa pessoa;
    if (!this->containerPessoa->pesquisar(usuario, pessoa)) {
        return false;
    }
    // Copia nao const: Papel::getValor() ainda nao e const.
    Papel papel = pessoa.getPapel();
    return papel.getValor() == "GESTOR";
}

bool CntrServicoProjeto::gestorDoProjeto(const Email& usuario, const Identificador& projeto) const {
    if (!this->usuarioGestor(usuario)) {
        return false;
    }
    Email gestor;
    if (!this->containerProjeto->pesquisarGestor(projeto, gestor)) {
        return false;
    }
    return gestor.getValor() == usuario.getValor();
}

bool CntrServicoProjeto::projetoCompleto(const Projeto& projeto) {
    return !projeto.getIdentificador().getValor().empty() &&
           !projeto.getNome().getValor().empty() &&
           !projeto.getDescricao().getValor().empty() &&
           !projeto.getInicio().getValor().empty() &&
           !projeto.getTermino().getValor().empty();
}

bool CntrServicoProjeto::criar(const Email& usuario, const Projeto& projeto) {
    if (!this->usuarioGestor(usuario)) {
        return false;
    }
    if (!projetoCompleto(projeto)) {
        return false;
    }
    // O conteiner ja recusa identificador repetido.
    return this->containerProjeto->incluir(projeto, usuario);
}

bool CntrServicoProjeto::ler(const Email& usuario, const Identificador& identificador, Projeto& projeto) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    return this->containerProjeto->pesquisar(identificador, projeto);
}

bool CntrServicoProjeto::atualizar(const Email& usuario, const Projeto& projeto) {
    // Tambem recusa projeto inexistente (sem gestor).
    if (!this->gestorDoProjeto(usuario, projeto.getIdentificador())) {
        return false;
    }
    if (!projetoCompleto(projeto)) {
        return false;
    }
    // O conteiner ja recusa projeto inexistente e mantem o gestor.
    return this->containerProjeto->atualizar(projeto);
}

bool CntrServicoProjeto::excluir(const Email& usuario, const Identificador& identificador) {
    if (!this->gestorDoProjeto(usuario, identificador)) {
        return false;
    }
    if (this->containerQuadro == nullptr) {
        return false;
    }
    // Sem cascata: recusa se o projeto tiver quadros (os cartoes ficam nos quadros).
    if (this->containerQuadro->existeQuadroNoProjeto(identificador)) {
        return false;
    }
    // O conteiner ja recusa projeto inexistente.
    return this->containerProjeto->remover(identificador);
}

bool CntrServicoProjeto::listar(const Email& usuario, std::vector<Identificador>& lista) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    this->containerProjeto->listar(lista);
    return true;
}
