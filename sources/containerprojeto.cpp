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

bool ContainerProjeto::pesquisar(const Identificador& identificador, Projeto& projeto) const {
    std::map<std::string, Projeto>::const_iterator it = this->projetos.find(identificador.getValor());
    if (it == this->projetos.end()) {
        return false;
    }
    projeto = it->second;
    return true;
}

bool ContainerProjeto::atualizar(const Projeto& projeto) {
    std::map<std::string, Projeto>::iterator it = this->projetos.find(projeto.getIdentificador().getValor());
    if (it == this->projetos.end()) {
        return false;
    }
    it->second = projeto;
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

void ContainerProjeto::listar(std::vector<Identificador>& lista) const {
    lista.clear();
    std::map<std::string, Projeto>::const_iterator it;
    for (it = this->projetos.begin(); it != this->projetos.end(); it++) {
        lista.push_back(it->second.getIdentificador());
    }
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
