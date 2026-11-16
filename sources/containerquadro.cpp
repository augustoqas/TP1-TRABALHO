#include "containerquadro.hpp"

bool ContainerQuadro::incluir(const Quadro& quadro, const Identificador& projeto) {
    std::string chave = quadro.getIdentificador().getValor();
    if (this->quadros.count(chave) > 0) {
        return false;
    }
    this->quadros[chave] = quadro;
    this->projetos[chave] = projeto;
    return true;
}

bool ContainerQuadro::remover(const Identificador& identificador) {
    std::string chave = identificador.getValor();
    if (this->quadros.erase(chave) == 0) {
        return false;
    }
    this->projetos.erase(chave);
    return true;
}

bool ContainerQuadro::existeQuadroNoProjeto(const Identificador& projeto) const {
    std::map<std::string, Identificador>::const_iterator it;
    for (it = this->projetos.begin(); it != this->projetos.end(); it++) {
        if (it->second.getValor() == projeto.getValor()) {
            return true;
        }
    }
    return false;
}
