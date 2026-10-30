#include "senha.hpp"

#include <stdexcept>

// Percorre a senha uma unica vez, contando letras e digitos.
// Usa comparacoes explicitas de faixa (e nao isalpha/isdigit), pois essas
// funcoes dependem de locale e tem comportamento indefinido com char negativo
// (bytes acima de 127, como letras acentuadas).
void Senha::validar(std::string valor) {
    if (valor.size() != TAMANHO) {
        throw std::invalid_argument("Senha invalida: deve ter exatamente 5 caracteres.");
    }

    int letras = 0;
    int digitos = 0;

    for (std::string::size_type i = 0; i < valor.size(); i++) {
        char c = valor[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            letras++;
        } else if (c >= '0' && c <= '9') {
            digitos++;
        } else {
            throw std::invalid_argument("Senha invalida: use apenas letras (a-z, A-Z) e digitos (0-9).");
        }
    }

    if (letras == 0) {
        throw std::invalid_argument("Senha invalida: deve ter pelo menos uma letra.");
    }
    if (digitos == 0) {
        throw std::invalid_argument("Senha invalida: deve ter pelo menos um digito.");
    }
}

void Senha::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
