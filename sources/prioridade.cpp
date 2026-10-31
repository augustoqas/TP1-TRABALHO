#include "prioridade.hpp"

#include <stdexcept>

const std::string Prioridade::ALTA = "ALTA";
const std::string Prioridade::MEDIA = "MEDIA";
const std::string Prioridade::BAIXA = "BAIXA";

void Prioridade::validar(std::string valor) {
    if (valor != ALTA && valor != MEDIA && valor != BAIXA) {
        throw std::invalid_argument("Prioridade invalida: deve ser ALTA, MEDIA ou BAIXA.");
    }
}

void Prioridade::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
