#include "nome.hpp"

#include <stdexcept>

// Retorna true se c for letra de A a Z ou de a a z.
// Comparacao explicita de faixa: nao depende de locale (como isalpha).
bool Nome::ehLetra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void Nome::validar(std::string valor) {
    // Regra 1: texto com 1 a 15 caracteres.
    if (valor.empty() || valor.size() > TAMANHO_MAXIMO) {
        throw std::invalid_argument("Nome invalido: deve ter de 1 a 15 caracteres.");
    }

    // Regras 3 e 4: primeiro e ultimo caracteres nao sao espaco.
    if (valor[0] == ' ' || valor[valor.size() - 1] == ' ') {
        throw std::invalid_argument("Nome invalido: nao pode comecar nem terminar com espaco.");
    }

    // Regra 2: cada caractere e letra ou espaco; espaco e seguido por letra.
    for (std::string::size_type i = 0; i < valor.size(); i++) {
        if (valor[i] == ' ') {
            // valor[i + 1] existe, pois o ultimo caractere nao e espaco.
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
