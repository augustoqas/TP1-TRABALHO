#include "containercartao.hpp"

bool ContainerCartao::incluir(const CartaoAtividade& cartao, const Identificador& quadro, const Email& desenvolvedor) {
    std::string chave = cartao.getIdentificador().getValor();
    if (this->cartoes.count(chave) > 0) {
        return false;
    }
    this->cartoes[chave] = cartao;
    this->quadros[chave] = quadro;
    this->desenvolvedores[chave] = desenvolvedor;
    return true;
}

bool ContainerCartao::remover(const Identificador& identificador) {
    std::string chave = identificador.getValor();
    if (this->cartoes.erase(chave) == 0) {
        return false;
    }
    this->quadros.erase(chave);
    this->desenvolvedores.erase(chave);
    return true;
}

bool ContainerCartao::pesquisar(const Identificador& identificador, CartaoAtividade& cartao) const {
    std::map<std::string, CartaoAtividade>::const_iterator it = this->cartoes.find(identificador.getValor());
    if (it == this->cartoes.end()) {
        return false;
    }
    cartao = it->second;
    return true;
}

bool ContainerCartao::atualizar(const CartaoAtividade& cartao) {
    std::map<std::string, CartaoAtividade>::iterator it = this->cartoes.find(cartao.getIdentificador().getValor());
    if (it == this->cartoes.end()) {
        return false;
    }
    it->second = cartao;
    return true;
}

bool ContainerCartao::getQuadro(const Identificador& identificador, Identificador& quadro) const {
    std::map<std::string, Identificador>::const_iterator it = this->quadros.find(identificador.getValor());
    if (it == this->quadros.end()) {
        return false;
    }
    quadro = it->second;
    return true;
}

bool ContainerCartao::getDesenvolvedor(const Identificador& identificador, Email& desenvolvedor) const {
    std::map<std::string, Email>::const_iterator it = this->desenvolvedores.find(identificador.getValor());
    if (it == this->desenvolvedores.end()) {
        return false;
    }
    desenvolvedor = it->second;
    return true;
}

bool ContainerCartao::existeDesenvolvedor(const Email& desenvolvedor) const {
    std::map<std::string, Email>::const_iterator it;
    for (it = this->desenvolvedores.begin(); it != this->desenvolvedores.end(); it++) {
        if (it->second.getValor() == desenvolvedor.getValor()) {
            return true;
        }
    }
    return false;
}

bool ContainerCartao::existeQuadro(const Identificador& quadro) const {
    std::map<std::string, Identificador>::const_iterator it;
    for (it = this->quadros.begin(); it != this->quadros.end(); it++) {
        if (it->second.getValor() == quadro.getValor()) {
            return true;
        }
    }
    return false;
}

void ContainerCartao::listar(const Identificador& quadro, std::vector<CartaoAtividade>& lista) const {
    lista.clear();
    std::map<std::string, Identificador>::const_iterator it;
    for (it = this->quadros.begin(); it != this->quadros.end(); it++) {
        if (it->second.getValor() == quadro.getValor()) {
            lista.push_back(this->cartoes.find(it->first)->second);
        }
    }
}
