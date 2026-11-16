#include "estado.hpp"

#include <stdexcept>

const std::string Estado::A_FAZER = "A FAZER";
const std::string Estado::FAZENDO = "FAZENDO";
const std::string Estado::FEITO = "FEITO";

void Estado::validar(std::string valor) {
    if (valor != A_FAZER && valor != FAZENDO && valor != FEITO) {
        throw std::invalid_argument("Estado invalido: use A FAZER, FAZENDO ou FEITO.");
    }
}

void Estado::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
