#include "texto.hpp"

#include <stdexcept>

bool Texto::ehPontuacao(char c) {
    return c == ',' || c == ';' || c == '.' || c == ':' || c == '?' || c == '!';
}

// Compara faixas em vez de usar isalpha/isdigit: essas dependem de locale e
// tem comportamento indefinido com char negativo (ex.: letra acentuada).
void Texto::validar(std::string valor) {
    if (valor.empty()) {
        throw std::invalid_argument("Texto invalido: nao pode ser vazio.");
    }
    if (valor.size() > TAMANHO_MAXIMO) {
        throw std::invalid_argument("Texto invalido: deve ter no maximo 30 caracteres.");
    }
    if (!(valor[0] >= 'A' && valor[0] <= 'Z')) {
        throw std::invalid_argument("Texto invalido: deve comecar com letra maiuscula.");
    }
    if (valor[valor.size() - 1] != '.') {
        throw std::invalid_argument("Texto invalido: deve terminar com ponto.");
    }

    for (std::string::size_type i = 0; i < valor.size(); i++) {
        char c = valor[i];
        bool letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool digito = c >= '0' && c <= '9';
        if (!letra && !digito && c != ' ' && !ehPontuacao(c)) {
            throw std::invalid_argument("Texto invalido: use letras, digitos, espaco e , ; . : ? !");
        }
        if (ehPontuacao(c) && i + 1 < valor.size() && ehPontuacao(valor[i + 1])) {
            throw std::invalid_argument("Texto invalido: pontuacao nao pode vir seguida de pontuacao.");
        }
    }
}

void Texto::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
