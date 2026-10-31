#include "limite.hpp"

#include <stdexcept>

void Limite::validar(int valor) {
    if (valor < MINIMO || valor > MAXIMO) {
        throw std::invalid_argument("Limite invalido: deve ser um inteiro de 1 a 25.");
    }
}

void Limite::setValor(int valor) {
    validar(valor);
    this->valor = valor;
}
