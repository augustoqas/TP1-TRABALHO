#include "containerprojeto.hpp"

bool ContainerProjeto::incluir(const Projeto& projeto, const Email& gestor) {
    std::string chave = projeto.getIdentificador().getValor();
    if (this->projetos.count(chave) > 0) {
        return false;
    }
    this->projetos[chave] = projeto;
    this->gestores[chave] = gestor;
    return true;
}

bool ContainerProjeto::remover(const Identificador& identificador) {
    std::string chave = identificador.getValor();
    if (this->projetos.erase(chave) == 0) {
        return false;
    }
    this->gestores.erase(chave);
    return true;
}

bool ContainerProjeto::existeGestor(const Email& gestor) const {
    std::map<std::string, Email>::const_iterator it;
    for (it = this->gestores.begin(); it != this->gestores.end(); it++) {
        if (it->second.getValor() == gestor.getValor()) {
            return true;
        }
    }
    return false;
}
