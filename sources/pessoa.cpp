#include "pessoa.hpp"

void Pessoa::setEmail(const Email& email) {
    this->email = email;
}

Email Pessoa::getEmail() const {
    return email;
}

void Pessoa::setNome(const Nome& nome) {
    this->nome = nome;
}

Nome Pessoa::getNome() const {
    return nome;
}

void Pessoa::setSenha(const Senha& senha) {
    this->senha = senha;
}

Senha Pessoa::getSenha() const {
    return senha;
}

void Pessoa::setPapel(const Papel& papel) {
    this->papel = papel;
}

Papel Pessoa::getPapel() const {
    return papel;
}
