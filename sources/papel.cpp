#include "papel.hpp"

#include <stdexcept>

const std::string Papel::GESTOR = "GESTOR";
const std::string Papel::DESENVOLVEDOR = "DESENVOLVEDOR";

void Papel::validar(std::string valor) {
    if (valor != GESTOR && valor != DESENVOLVEDOR) {
        throw std::invalid_argument("Papel invalido: deve ser GESTOR ou DESENVOLVEDOR.");
    }
}

void Papel::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
