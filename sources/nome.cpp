#include "nome.hpp"

#include <stdexcept>

// Compara faixas em vez de usar isalpha: essa depende de locale e tem
// comportamento indefinido com char negativo (ex.: letra acentuada).
bool Nome::ehLetra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void Nome::validar(std::string valor) {
    if (valor.empty() || valor.size() > TAMANHO_MAXIMO) {
        throw std::invalid_argument("Nome invalido: deve ter de 1 a 15 caracteres.");
    }

    if (valor[0] == ' ' || valor[valor.size() - 1] == ' ') {
        throw std::invalid_argument("Nome invalido: nao pode comecar nem terminar com espaco.");
    }

    for (std::string::size_type i = 0; i < valor.size(); i++) {
        if (valor[i] == ' ') {
            // valor[i + 1] existe: o ultimo caractere ja foi checado e nao e espaco.
            if (!ehLetra(valor[i + 1])) {
                throw std::invalid_argument("Nome invalido: espaco deve ser seguido por letra.");
            }
        } else if (!ehLetra(valor[i])) {
            throw std::invalid_argument("Nome invalido: use apenas letras (A-Z, a-z) e espaco.");
        }
    }
}

void Nome::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
