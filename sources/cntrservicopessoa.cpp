#include "cntrservicopessoa.hpp"

#include <string>

void CntrServicoPessoa::setContainerPessoa(ContainerPessoa* containerPessoa) {
    this->containerPessoa = containerPessoa;
}

void CntrServicoPessoa::setContainerProjeto(ContainerProjeto* containerProjeto) {
    this->containerProjeto = containerProjeto;
}

void CntrServicoPessoa::setContainerCartao(ContainerCartao* containerCartao) {
    this->containerCartao = containerCartao;
}

bool CntrServicoPessoa::usuarioCadastrado(const Email& usuario) const {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    return this->containerPessoa->pesquisar(usuario, pessoa);
}

bool CntrServicoPessoa::pessoaCompleta(const Pessoa& pessoa) {
    // Copia nao const: Papel::getValor() ainda nao e const.
    Papel papel = pessoa.getPapel();
    std::string valorPapel = papel.getValor();
    if (valorPapel != "GESTOR" && valorPapel != "DESENVOLVEDOR") {
        return false;
    }
    if (pessoa.getEmail().getValor().empty()) {
        return false;
    }
    if (pessoa.getNome().getValor().empty()) {
        return false;
    }
    return !pessoa.getSenha().getValor().empty();
}

bool CntrServicoPessoa::criar(const Pessoa& pessoa) {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    if (!pessoaCompleta(pessoa)) {
        return false;
    }
    return this->containerPessoa->incluir(pessoa);
}

bool CntrServicoPessoa::ler(const Email& usuario, const Email& email, Pessoa& pessoa) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    return this->containerPessoa->pesquisar(email, pessoa);
}

bool CntrServicoPessoa::atualizar(const Email& usuario, const Pessoa& pessoa) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    if (this->containerProjeto == nullptr || this->containerCartao == nullptr) {
        return false;
    }
    if (!pessoaCompleta(pessoa)) {
        return false;
    }

    // Copia nao const: Papel::getValor() ainda nao e const.
    Papel papel = pessoa.getPapel();
    std::string novoPapel = papel.getValor();
    Email email = pessoa.getEmail();

    // Gestor de projeto precisa continuar GESTOR.
    if (novoPapel != "GESTOR" && this->containerProjeto->existeGestor(email)) {
        return false;
    }
    // Desenvolvedor de cartao precisa continuar DESENVOLVEDOR.
    if (novoPapel != "DESENVOLVEDOR" && this->containerCartao->existeDesenvolvedor(email)) {
        return false;
    }
    // Retorna false se a pessoa nao existe (nada e criado).
    return this->containerPessoa->atualizar(pessoa);
}

bool CntrServicoPessoa::excluir(const Email& usuario, const Email& email) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    if (this->containerProjeto == nullptr || this->containerCartao == nullptr) {
        return false;
    }

    // Sem cascata: recusa se a pessoa ainda possui vinculos.
    if (this->containerProjeto->existeGestor(email)) {
        return false;
    }
    if (this->containerCartao->existeDesenvolvedor(email)) {
        return false;
    }
    // Retorna false se a pessoa nao existe.
    return this->containerPessoa->remover(email);
}

bool CntrServicoPessoa::listar(const Email& usuario, std::vector<Email>& lista) {
    if (!this->usuarioCadastrado(usuario)) {
        return false;
    }
    this->containerPessoa->listar(lista);
    return true;
}
