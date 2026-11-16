#include "identificador.hpp"

#include <stdexcept>

// Compara faixas em vez de usar isalpha/isdigit: essas dependem de locale e
// tem comportamento indefinido com char negativo (ex.: letra acentuada).
void Identificador::validar(std::string valor) {
    if (valor.size() != TAMANHO) {
        throw std::invalid_argument("Identificador invalido: deve ter exatamente 6 caracteres.");
    }

    for (std::string::size_type i = 0; i < LETRAS; i++) {
        char c = valor[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            throw std::invalid_argument("Identificador invalido: os 3 primeiros caracteres devem ser letras (a-z, A-Z).");
        }
    }

    for (std::string::size_type i = LETRAS; i < TAMANHO; i++) {
        char c = valor[i];
        if (!(c >= '0' && c <= '9')) {
            throw std::invalid_argument("Identificador invalido: os 3 ultimos caracteres devem ser digitos (0-9).");
        }
    }
}

void Identificador::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
