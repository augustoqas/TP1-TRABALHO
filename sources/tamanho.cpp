#include "tamanho.hpp"

#include <stdexcept>

const std::string Tamanho::GRANDE = "GRANDE";
const std::string Tamanho::MEDIO = "MEDIO";
const std::string Tamanho::PEQUENO = "PEQUENO";

void Tamanho::validar(std::string valor) {
    if (valor != GRANDE && valor != MEDIO && valor != PEQUENO) {
        throw std::invalid_argument("Tamanho invalido: deve ser GRANDE, MEDIO ou PEQUENO.");
    }
}

void Tamanho::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
