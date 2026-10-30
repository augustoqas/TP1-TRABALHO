#include "containerpessoa.hpp"

bool ContainerPessoa::incluir(const Pessoa& pessoa) {
    std::string chave = pessoa.getEmail().getValor();
    if (this->pessoas.count(chave) > 0) {
        return false;
    }
    this->pessoas[chave] = pessoa;
    return true;
}

bool ContainerPessoa::pesquisar(const Email& email, Pessoa& pessoa) const {
    std::map<std::string, Pessoa>::const_iterator it = this->pessoas.find(email.getValor());
    if (it == this->pessoas.end()) {
        return false;
    }
    pessoa = it->second;
    return true;
}

bool ContainerPessoa::atualizar(const Pessoa& pessoa) {
    std::string chave = pessoa.getEmail().getValor();
    if (this->pessoas.count(chave) == 0) {
        return false;
    }
    this->pessoas[chave] = pessoa;
    return true;
}

bool ContainerPessoa::remover(const Email& email) {
    return this->pessoas.erase(email.getValor()) > 0;
}

void ContainerPessoa::listar(std::vector<Email>& lista) const {
    lista.clear();
    std::map<std::string, Pessoa>::const_iterator it;
    for (it = this->pessoas.begin(); it != this->pessoas.end(); it++) {
        lista.push_back(it->second.getEmail());
    }
}
