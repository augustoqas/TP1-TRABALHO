#include "email.hpp"

// Compara faixas em vez de usar islower/isdigit: essas dependem de locale e
// tem comportamento indefinido com char negativo. Aceita so minusculas.
bool Email::ehLetraOuDigito(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool Email::validarParteLocal(const std::string& parteLocal) {
    if (parteLocal.empty() || parteLocal.size() > MAXIMO_PARTE_LOCAL) {
        return false;
    }
    for (std::string::size_type i = 0; i < parteLocal.size(); i++) {
        char c = parteLocal[i];
        if (ehLetraOuDigito(c)) {
            continue;
        }
        if (c != '.' && c != '-') {
            return false;
        }
        if (i == 0) {
            return false;                       // comeca com ponto ou hifen
        }
        if (i + 1 == parteLocal.size() || !ehLetraOuDigito(parteLocal[i + 1])) {
            return false;                       // termina com ponto/hifen, ou o proximo nao e letra/digito
        }
    }
    return true;
}

bool Email::validarParteDominio(const std::string& parte) {
    if (parte.empty()) {
        return false;                           // ex.: "unb..br", ou ponto no inicio/fim
    }
    if (parte[0] == '-' || parte[parte.size() - 1] == '-') {
        return false;
    }
    for (std::string::size_type i = 0; i < parte.size(); i++) {
        if (!ehLetraOuDigito(parte[i]) && parte[i] != '-') {
            return false;
        }
    }
    return true;
}

bool Email::validarDominio(const std::string& dominio) {
    if (dominio.empty() || dominio.size() > MAXIMO_DOMINIO) {
        return false;
    }
    // O laco valida cada parte que termina em ponto; a ultima fica para o fim.
    std::string::size_type inicio = 0;
    std::string::size_type ponto = dominio.find('.');
    while (ponto != std::string::npos) {
        if (!validarParteDominio(dominio.substr(inicio, ponto - inicio))) {
            return false;
        }
        inicio = ponto + 1;
        ponto = dominio.find('.', inicio);
    }
    return validarParteDominio(dominio.substr(inicio));
}

void Email::validar(std::string valor) {
    // Exatamente um '@': achou o primeiro e nao ha outro depois dele.
    std::string::size_type arroba = valor.find('@');
    if (arroba == std::string::npos || valor.find('@', arroba + 1) != std::string::npos) {
        throw std::invalid_argument("Email invalido: deve conter exatamente um @.");
    }
    if (!validarParteLocal(valor.substr(0, arroba))) {
        throw std::invalid_argument("Email invalido: parte local fora do formato.");
    }
    if (!validarDominio(valor.substr(arroba + 1))) {
        throw std::invalid_argument("Email invalido: dominio fora do formato.");
    }
}

void Email::setValor(std::string valor) {
    validar(valor);
    this->valor = valor;
}
