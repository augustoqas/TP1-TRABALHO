#include "cntrservicoautenticacao.hpp"

void CntrServicoAutenticacao::setContainerPessoa(ContainerPessoa* containerPessoa) {
    this->containerPessoa = containerPessoa;
}

bool CntrServicoAutenticacao::autenticar(const Email& email, const Senha& senha) {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    if (!this->containerPessoa->pesquisar(email, pessoa)) {
        return false;
    }
    return pessoa.getSenha().getValor() == senha.getValor();
}
