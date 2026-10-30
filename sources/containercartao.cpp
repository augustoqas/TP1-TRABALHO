#include "containercartao.hpp"

bool ContainerCartao::incluir(const CartaoAtividade& cartao, const Email& desenvolvedor) {
    std::string chave = cartao.getIdentificador().getValor();
    if (this->cartoes.count(chave) > 0) {
        return false;
    }
    this->cartoes[chave] = cartao;
    this->desenvolvedores[chave] = desenvolvedor;
    return true;
}

bool ContainerCartao::remover(const Identificador& identificador) {
    std::string chave = identificador.getValor();
    if (this->cartoes.erase(chave) == 0) {
        return false;
    }
    this->desenvolvedores.erase(chave);
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
