#include "projeto.hpp"

void Projeto::setIdentificador(const Identificador& identificador) {
    this->identificador = identificador;
}

Identificador Projeto::getIdentificador() const {
    return identificador;
}

void Projeto::setNome(const Nome& nome) {
    this->nome = nome;
}

Nome Projeto::getNome() const {
    return nome;
}

void Projeto::setDescricao(const Texto& descricao) {
    this->descricao = descricao;
}

Texto Projeto::getDescricao() const {
    return descricao;
}

void Projeto::setInicio(const Timestamp& inicio) {
    this->inicio = inicio;
}

Timestamp Projeto::getInicio() const {
    return inicio;
}

void Projeto::setTermino(const Timestamp& termino) {
    this->termino = termino;
}

Timestamp Projeto::getTermino() const {
    return termino;
}
