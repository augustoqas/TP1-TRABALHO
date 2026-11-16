#include "quadro.hpp"

void Quadro::setIdentificador(const Identificador& identificador) {
    this->identificador = identificador;
}

Identificador Quadro::getIdentificador() const {
    return identificador;
}

void Quadro::setNome(const Nome& nome) {
    this->nome = nome;
}

Nome Quadro::getNome() const {
    return nome;
}

void Quadro::setLimite(const Limite& limite) {
    this->limite = limite;
}

Limite Quadro::getLimite() const {
    return limite;
}
