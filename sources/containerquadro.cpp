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

bool ContainerQuadro::pesquisar(const Identificador& identificador, Quadro& quadro) const {
    std::map<std::string, Quadro>::const_iterator it = this->quadros.find(identificador.getValor());
    if (it == this->quadros.end()) {
        return false;
    }
    quadro = it->second;
    return true;
}

bool ContainerQuadro::atualizar(const Quadro& quadro) {
    std::map<std::string, Quadro>::iterator it = this->quadros.find(quadro.getIdentificador().getValor());
    if (it == this->quadros.end()) {
        return false;
    }
    it->second = quadro;
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

void ContainerQuadro::listar(const Identificador& projeto, std::vector<Identificador>& lista) const {
    lista.clear();
    std::map<std::string, Identificador>::const_iterator it;
    for (it = this->projetos.begin(); it != this->projetos.end(); it++) {
        if (it->second.getValor() == projeto.getValor()) {
            lista.push_back(this->quadros.find(it->first)->second.getIdentificador());
        }
    }
}

bool ContainerQuadro::pesquisarProjeto(const Identificador& quadro, Identificador& projeto) const {
    std::map<std::string, Identificador>::const_iterator it = this->projetos.find(quadro.getValor());
    if (it == this->projetos.end()) {
        return false;
    }
    projeto = it->second;
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
